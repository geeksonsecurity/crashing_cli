#include <iostream>
#include <intrin.h>
#include <Windows.h>
#include <DbgHelp.h>
#include <Dbgeng.h>
#include <inttypes.h>
#include <vector>
#include <Zydis/Zydis.h>
#include <psapi.h>

#define PE_IMAGE_UWP_EPILOG_AT_THE_END		1

#pragma comment (lib, "dbgeng")
#pragma comment(lib, "dbghelp.lib")
#pragma intrinsic(_ReturnAddress)

typedef enum _UNWIND_OP_CODES {
  UWOP_PUSH_NONVOL = 0, /* info == register number */
  UWOP_ALLOC_LARGE,     /* no info, alloc size in next 2 slots */
  UWOP_ALLOC_SMALL,     /* info == size of allocation / 8 - 1 */
  UWOP_SET_FPREG,       /* no info, FP = RSP + UNWIND_INFO.FPRegOffset*16 */
  UWOP_SAVE_NONVOL,     /* info == register number, offset in next slot */
  UWOP_SAVE_NONVOL_FAR, /* info == register number, offset in next 2 slots */
  UWOP_EPILOG, // only on version 2
  UWOP_SAVE_XMM128 = 8, /* info == XMM reg number, offset in next slot */
  UWOP_SAVE_XMM128_FAR, /* info == XMM reg number, offset in next 2 slots */
  UWOP_PUSH_MACHFRAME   /* info == 0: no error-code, 1: error-code */
} UNWIND_CODE_OPS;

typedef unsigned char UBYTE;

typedef union _UNWIND_CODE {
  struct {
    UBYTE CodeOffset_OffsetLowOrSize;
    UBYTE UnwindOp: 4;
    UBYTE OpInfo_OffsetHighOrFlags: 4;
  };
  USHORT FrameOffset;
} UNWIND_CODE, *PUNWIND_CODE;

typedef struct _UNWIND_INFO {
  UBYTE Version: 3;
  UBYTE Flags: 5;
  UBYTE SizeOfProlog;
  UBYTE CountOfCodes;
  UBYTE FrameRegister: 4;
  UBYTE FrameOffset: 4;
  UNWIND_CODE UnwindCode[1];
/*  UNWIND_CODE MoreUnwindCode[((CountOfCodes + 1) & ~1) - 1];
*   union {
*       OPTIONAL ULONG ExceptionHandler;
*       OPTIONAL ULONG FunctionEntry;
*   };
*   OPTIONAL ULONG ExceptionData[]; */
} UNWIND_INFO, *PUNWIND_INFO;

char *GetUnwindOpNameForCode(UBYTE op);
size_t GetComputedOffsetForAllocation(UNWIND_CODE codes[], int *p_int);
#define GetUnwindCodeEntry(info, index) \
    ((info)->UnwindCode[index])

#define GetLanguageSpecificDataPtr(info) \
    ((PVOID)&GetUnwindCodeEntry((info),((info)->CountOfCodes + 1) & ~1))

#define GetExceptionHandler(base, info) \
    ((PEXCEPTION_HANDLER)((base) + *(PULONG)GetLanguageSpecificDataPtr(info)))

#define GetChainedFunctionEntry(base, info) \
    ((PRUNTIME_FUNCTION)((base) + *(PULONG)GetLanguageSpecificDataPtr(info)))

#define GetExceptionDataPtr(info) \
    ((PVOID)((PULONG)GetLanguageSpecificData(info) + 1))

void hexdump(const void *mem, unsigned int len, const char* title="", int show_base=1){
  // `title` will be printed at the beginning of the dump
  // If `show_base` is true, the address of `mem` will be printed at the beginning of each line

  const int HEXDUMP_COLS = 16;

  unsigned int i, j;

  if(title && strcmp(title, "") != 0){
    printf("%s:\n", title);
  }

  for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
  {
    // print (base +) offset
    if(i % HEXDUMP_COLS == 0){
      if(show_base){
        printf("%p + ", mem);
      }
      printf("0x%06x: ", i);
    }

    // print hex data
    if(i % 8 == 0){
      putchar(' ');
    }
    if(i < len){
      printf("%02x ", 0xFF & ((char*)mem)[i]);
    }else{
      // end of block, just aligning for ASCII dump
      printf("   ");
    }

    // print ASCII dump
    if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1)){
      for(j = i - (HEXDUMP_COLS - 1); j <= i; j++){
        if(j % 8 == 0){
          putchar(' ');
        }

        if(j >= len){
          // end of block, not really printing
          putchar(' ');
        }else if(isprint(((char*)mem)[j])){
          // printable char
          putchar(0xFF & ((char*)mem)[j]);
        }else{
          // other char
          putchar('.');
        }
      }
      putchar('\n');
    }
  }
}

void DecodeAndPrint(ZyanU8 *data, ZyanUSize length) {
  DWORD64 ImageBase;
  UNWIND_HISTORY_TABLE table;

  printf("[+] Decoding 0x%" PRIx64 " - 0x%" PRIx64 " (%llu bytes)\n", data, data + length, length);

  // Initialize decoder context
  ZydisDecoder decoder;
  ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

  // Initialize formatter. Only required when you actually plan to do instruction
  // formatting ("disassembling"), like we do here
  ZydisFormatter formatter;
  ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

  // Loop over the instructions in our buffer.
  // The runtime-address (instruction pointer) is chosen arbitrary here in order to better
  // visualize relative addressing
  ZyanU64 runtime_address = reinterpret_cast<uint64_t>(data);
  ZyanUSize offset = 0;
  ZydisDecodedInstruction instruction;
  ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT_VISIBLE];
  while (ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, data + offset, length - offset,
                                             &instruction, operands, ZYDIS_MAX_OPERAND_COUNT_VISIBLE,
                                             ZYDIS_DFLAG_VISIBLE_OPERANDS_ONLY))) {
    // Print current instruction pointer.
    printf("%016" PRIX64 "  ", runtime_address);

    // Format & print the binary instruction structure to human readable format
    char buffer[256];
    ZydisFormatterFormatInstruction(&formatter, &instruction, operands,
                                    instruction.operand_count_visible, buffer, sizeof(buffer), runtime_address);
    puts(buffer);

    offset += instruction.length;
    runtime_address += instruction.length;
  }
}
size_t GetEpilogueSize(UNWIND_INFO *UnwindInfo, size_t BeginAddr, size_t EndAddr, size_t Rip, int *idx){

  size_t EpilogSize = UnwindInfo->UnwindCode[0].CodeOffset_OffsetLowOrSize;

  // if this epilogue entry is valid then unwind it if rip is within
  if ( UnwindInfo->UnwindCode[0].OpInfo_OffsetHighOrFlags & PE_IMAGE_UWP_EPILOG_AT_THE_END ) {
    // is rip within epilogue?
    return EpilogSize;
  }
  size_t EpilogOffset;
  // go through the list of epilogues and try to find the one
  for ( uint32_t i = 1; i < UnwindInfo->CountOfCodes; i++ ) {
    // did we reach epilogue codes end?
    if ( UnwindInfo->UnwindCode[i].UnwindOp != UWOP_EPILOG ) {
      *idx += i;
      break;
    }

    EpilogOffset = UnwindInfo->UnwindCode[i].CodeOffset_OffsetLowOrSize + (UnwindInfo->UnwindCode[i].OpInfo_OffsetHighOrFlags << 8);
    if ( EpilogOffset == 0 ) {
      *idx += i;
      // it's last code entry
      break;
    }
    return EpilogOffset;
  }
  return EpilogSize;
}

void ResolveFunctionEntry(PVOID addr, ULONG64 ImageBase) {
  PVOID moduleBase = GetModuleHandle(NULL);
  ZyanU8 *data;
  ZyanUSize length;
  UNWIND_HISTORY_TABLE table;
  RUNTIME_FUNCTION *res = RtlLookupFunctionEntry(reinterpret_cast<DWORD64>(addr), &ImageBase, &table);

  if (res != NULL) {;
    printf("\tRtlLookupFunctionEntry   (0x%" PRIx64 ") @ %p (base %p) -> 0x%lx - 0x%lx: %lx (%lx)\n",
           addr,
           res,
           ImageBase,
           res->BeginAddress,
           res->EndAddress,
           res->UnwindData,
           res->UnwindInfoAddress);
    UNWIND_INFO *ui = (UNWIND_INFO *) ((char *) ImageBase + res->UnwindData);
    printf("\tUnwind info at 0x%" PRIx64 "\n", ui);
    printf("\tversion %x, flags %x, prolog %x, codes %x, frame reg %x, frame off %x\n",
           ui->Version, ui->Flags, ui->SizeOfProlog, ui->CountOfCodes, ui->FrameRegister, ui->FrameOffset);
    for (int i = 0; i < ui->CountOfCodes; i++) {
      UNWIND_CODE uc = GetUnwindCodeEntry(ui, i);
      size_t offset = GetComputedOffsetForAllocation(&ui->UnwindCode[i], &i);
      if(uc.UnwindOp == UWOP_EPILOG){
        offset = GetEpilogueSize(ui, res->BeginAddress, res->EndAddress, reinterpret_cast<DWORD64>(addr), &i);
      }

      printf("\t0x%02x: code-offs %x, frame-offs %x, unwind op %x, op info %x - %s (0x%x)\n",
             i,
             uc.CodeOffset_OffsetLowOrSize,
             uc.FrameOffset,
             uc.UnwindOp,
             uc.OpInfo_OffsetHighOrFlags,
             GetUnwindOpNameForCode(uc.UnwindOp),
             offset
             );
      data = reinterpret_cast<uint8_t *>(ImageBase + res->BeginAddress);
      length = ui->SizeOfProlog;
    }
  } else {
    data = reinterpret_cast<uint8_t *>(addr);
    length = 0x30;
  }
  DecodeAndPrint(data, length);

  res = (RUNTIME_FUNCTION *) SymFunctionTableAccess64(GetCurrentProcess(), DWORD64(addr));
  if (res != NULL) {
    printf("\tSymFunctionTableAccess64 (0x%" PRIx64 ") @ %p -> 0x%lx - 0x%lx: %lx (%lx)\n",
           addr,
           res,
           res->BeginAddress,
           res->EndAddress,
           res->UnwindData,
           res->UnwindInfoAddress);
  }
}


size_t GetComputedOffsetForAllocation(UNWIND_CODE codes[], int *idx) {
  UNWIND_CODE code = codes[0];
  if(code.UnwindOp == UWOP_ALLOC_SMALL){
    // Allocate a small-sized area on the stack. The size of the allocation is the operation info
    // field * 8 + 8, allowing allocations from 8 to 128 bytes.
    return code.OpInfo_OffsetHighOrFlags * 8 + 8;
  } else if(code.UnwindOp == UWOP_ALLOC_LARGE){
    // Allocate a large-sized area on the stack. There are two forms. If the operation info equals 0,
    // then the size of the allocation divided by 8 is recorded in the next slot, allowing an
    // allocation up to 512K - 8.
    if(code.OpInfo_OffsetHighOrFlags == 0) {
      *idx = *idx + 1;
      return codes[1].FrameOffset * 8;
    } else if(code.OpInfo_OffsetHighOrFlags == 1){
      // If the operation info equals 1, then the unscaled size of the
      // allocation is recorded in the next two slots in little-endian format, allowing allocations up to 4GB - 8.
      *idx = *idx + 2;
      return codes[1].FrameOffset + codes[2].FrameOffset;
    } else {
      return -1;
    }
  }
  return -1;
}
char *GetUnwindOpNameForCode(UBYTE op) {
  switch (op) {
    case UWOP_PUSH_NONVOL:return "UWOP_PUSH_NONVOL";
    case UWOP_ALLOC_LARGE:return "UWOP_ALLOC_LARGE";
    case UWOP_ALLOC_SMALL:return "UWOP_ALLOC_SMALL";
    case UWOP_SET_FPREG:return "UWOP_SET_FPREG";
    case UWOP_SAVE_NONVOL:return "UWOP_SAVE_NONVOL";
    case UWOP_SAVE_NONVOL_FAR:return "UWOP_SAVE_NONVOL_FAR";
    case UWOP_SAVE_XMM128:return "UWOP_SAVE_XMM128";
    case UWOP_SAVE_XMM128_FAR:return "UWOP_SAVE_XMM128_FAR";
    case UWOP_PUSH_MACHFRAME:return "UWOP_PUSH_MACHFRAME";
    case UWOP_EPILOG:return "UWOP_EPILOG";
    default:return "UWOP_UNKNOWN";
  }
}

void PrintCallStack(void) {
  BOOL result;
  HANDLE process;
  HANDLE thread;
  CONTEXT context;
  STACKFRAME64 stack;
  ULONG frame;
  SymInitialize(GetCurrentProcess(), NULL, TRUE);

  RtlCaptureContext(&context);
  memset(&stack, 0, sizeof(STACKFRAME64));

  process = GetCurrentProcess();
  thread = GetCurrentThread();
  stack.AddrPC.Offset = context.Rip;
  stack.AddrPC.Mode = AddrModeFlat;
  stack.AddrStack.Offset = context.Rsp;
  stack.AddrStack.Mode = AddrModeFlat;
  stack.AddrFrame.Offset = context.Rbp;
  stack.AddrFrame.Mode = AddrModeFlat;

  PVOID moduleBase = GetModuleHandle(NULL);
  printf("ImageBase 0x%" PRIx64 "\n", moduleBase);
  printf("Callstack:\n");
  for (frame = 0;; frame++) {
    result = StackWalk64
        (
            IMAGE_FILE_MACHINE_AMD64,
            process,
            thread,
            &stack,
            &context,
            NULL,
            SymFunctionTableAccess64,
            SymGetModuleBase64,
            NULL
        );
    if (!result) {
      break;
    }

    DWORD64 dwDisplacement = 0;
    DWORD64 dwAddress = stack.AddrPC.Offset;

    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    PSYMBOL_INFO pSymbol = (PSYMBOL_INFO) buffer;

    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = MAX_SYM_NAME;

    char moduleName[1024];
    DWORD64 moduleBase = SymGetModuleBase64(process, stack.AddrPC.Offset);
    GetModuleBaseNameA(process, (HMODULE)moduleBase, &moduleName[0], 1024);

    BOOL symbolResolved = SymFromAddr(process, dwAddress, &dwDisplacement, pSymbol);
    printf
        (
            "%02lu: IP: 0x%" PRIx64 " - %s - %s + 0x%x RET: 0x%" PRIx64 " RSP: 0x%" PRIx64 "  \n",
            frame,
            stack.AddrPC.Offset,
            symbolResolved ? pSymbol->Name : "n.a.",
            moduleName, (stack.AddrPC.Offset-moduleBase),
            stack.AddrReturn.Offset,
            stack.AddrStack.Offset
        );
    //hexdump(reinterpret_cast<void*>(stack.AddrStack.Offset), 0x40, "RSP", 1);
    //ResolveFunctionEntry((PVOID) stack.AddrPC.Offset, pSymbol->ModBase);

  }
}

void GetCallstackUnwind() {
  UINT64 ImageBase;
  PRUNTIME_FUNCTION pFunction;
  UINT64 establisherFrame;
  PVOID handlerData;
  CONTEXT context = {0};
  context.ContextFlags = CONTEXT_ALL;
  bool res = GetThreadContext(GetCurrentThread(), &context);
  while (context.Rip != 0) {
    pFunction = RtlLookupFunctionEntry(context.Rip, &ImageBase, NULL);
    if (pFunction == NULL) {
      context.Rip = *(PUINT64) context.Rsp;
      context.Rsp += sizeof(PUINT64);
    } else {
      establisherFrame = 0;
      handlerData = NULL;
      RtlVirtualUnwind(0, ImageBase, context.Rip, pFunction, &context, &handlerData, &establisherFrame, NULL);
    }
    printf("RSP: 0x%" PRIx64 " - RIP 0x%" PRIx64 "\n");
  }
}

void GetCallstack(DWORD tid) {
  IDebugClient4 *debugClient;
  IDebugControl4 *control4;
  DEBUG_STACK_FRAME frames[10];
  ULONG filled = 0;

  DebugCreate(__uuidof(IDebugClient4), (void **) &debugClient);

  debugClient->QueryInterface(__uuidof(IDebugControl4), (void **) &control4);

  debugClient->AttachProcess(0, GetCurrentProcessId(), DEBUG_ATTACH_NONINVASIVE | DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND);

  control4->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE);

  HANDLE hThread = OpenThread(THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME, false, tid);

  CONTEXT Context = {0};
  Context.ContextFlags = CONTEXT_ALL;
  bool res = GetThreadContext(hThread, &Context);

  ULONG filledFrames = 1024;
  std::vector<DEBUG_STACK_FRAME> dbgFrames(filledFrames);
  auto hres = control4->GetContextStackTrace(
      &Context,
      sizeof(Context),
      &dbgFrames[0],
      filledFrames,
      NULL,
      filledFrames * sizeof(Context),
      sizeof(Context),
      &filledFrames);
  return;
}

void Func3() {
  DWORD64 ImageBase;
  UNWIND_HISTORY_TABLE table;
  printf("%s: %p\n", __FUNCTION__, _ReturnAddress());
  PrintCallStack();
  //GetCallstackUnwind();
  //GetCallstack(GetCurrentThreadId());
  //CONTEXT context = {0};
  //context.ContextFlags = CONTEXT_ALL;
  //GetThreadContext(GetCurrentThread(), &context);
  //printf("IP: %p\n", context.Rip);
  //DecodeAndPrint(context.Rip);

  fflush(stdout);
  //__fastfail(1338);
  //__debugbreak();
  //int* c = NULL;
  //*c = 1;
}

void Func2() {
  printf("%s: %p\n", __FUNCTION__, _ReturnAddress());
  Func3();
}

void Func1() {
  printf("%s: %p\n", __FUNCTION__, _ReturnAddress());
  Func2();
}

int main() {
  printf("%s: %p\n", __FUNCTION__, _ReturnAddress());
  Func1();
  return 0;
}
