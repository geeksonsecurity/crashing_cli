#include <iostream>
#include <intrin.h>
#include <Windows.h>
#include <DbgHelp.h>
#include <Dbgeng.h>
#include <inttypes.h>

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
  UWOP_SAVE_XMM128 = 8, /* info == XMM reg number, offset in next slot */
  UWOP_SAVE_XMM128_FAR, /* info == XMM reg number, offset in next 2 slots */
  UWOP_PUSH_MACHFRAME   /* info == 0: no error-code, 1: error-code */
} UNWIND_CODE_OPS;

typedef unsigned char UBYTE;

typedef union _UNWIND_CODE {
  struct {
    UBYTE CodeOffset;
    UBYTE UnwindOp : 4;
    UBYTE OpInfo   : 4;
  };
  USHORT FrameOffset;
} UNWIND_CODE, *PUNWIND_CODE;

#define UNW_FLAG_EHANDLER  0x01
#define UNW_FLAG_UHANDLER  0x02
#define UNW_FLAG_CHAININFO 0x04

typedef struct _UNWIND_INFO {
  UBYTE Version       : 3;
  UBYTE Flags         : 5;
  UBYTE SizeOfProlog;
  UBYTE CountOfCodes;
  UBYTE FrameRegister : 4;
  UBYTE FrameOffset   : 4;
  UNWIND_CODE UnwindCode[1];
/*  UNWIND_CODE MoreUnwindCode[((CountOfCodes + 1) & ~1) - 1];
*   union {
*       OPTIONAL ULONG ExceptionHandler;
*       OPTIONAL ULONG FunctionEntry;
*   };
*   OPTIONAL ULONG ExceptionData[]; */
} UNWIND_INFO, *PUNWIND_INFO;

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


void ResolveFunctionEntry(PVOID addr, ULONG64 ImageBase) {
  PVOID moduleBase = GetModuleHandle(NULL);
  UNWIND_HISTORY_TABLE table;
  RUNTIME_FUNCTION *res = RtlLookupFunctionEntry(reinterpret_cast<DWORD64>(addr), &ImageBase, &table);
  if (res != NULL) {
    printf("\tRtlLookupFunctionEntry   (0x%" PRIx64 ") @ %p (base %p) -> 0x%lx - 0x%lx: %lx (%lx)\n", addr, res, ImageBase, res->BeginAddress, res->EndAddress, res->UnwindData, res->UnwindInfoAddress);
    UNWIND_INFO* ui = (UNWIND_INFO*)((char*)ImageBase + res->UnwindData);
    printf("\t\tCodes: %d\n", ui->CountOfCodes);
    for(int i=0;i<ui->CountOfCodes; i++){
      UNWIND_CODE uc = GetUnwindCodeEntry(ui, i);
      printf("\t\t\t[0x%02x] %x %x\n", i, uc.OpInfo, uc.UnwindOp);
    }
  }
  res = (RUNTIME_FUNCTION *) SymFunctionTableAccess64(GetCurrentProcess(), DWORD64(addr));
  if (res != NULL) {
    printf("\tSymFunctionTableAccess64 (0x%" PRIx64 ") @ %p -> 0x%lx - 0x%lx: %lx (%lx)\n", addr, res, res->BeginAddress, res->EndAddress, res->UnwindData, res->UnwindInfoAddress);
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

    DWORD64  dwDisplacement = 0;
    DWORD64  dwAddress = stack.AddrPC.Offset;

    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = MAX_SYM_NAME;

    BOOL symbolResolved = SymFromAddr(process, dwAddress, &dwDisplacement, pSymbol);
    printf
        (
            "%02lu: IP: 0x%" PRIx64 " (%s) RET: 0x%" PRIx64 " \n",
            frame,
            stack.AddrPC.Offset,
            symbolResolved ? pSymbol->Name : "n.a.",
            stack.AddrReturn.Offset
        );
    ResolveFunctionEntry((PVOID) stack.AddrPC.Offset, pSymbol->ModBase);

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

  CONTEXT lpContext = {0};
  bool res = GetThreadContext(hThread, &lpContext);
  control4->GetStackTrace(lpContext.Rsp, lpContext.Rbp, lpContext.Rip, &frames[0], 10, &filled);
  return;
}

void Func3() {
  printf("%s: %p\n", __FUNCTION__, _ReturnAddress());
  PrintCallStack();
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
