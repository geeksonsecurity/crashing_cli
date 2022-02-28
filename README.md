# Crashing CLI
Simple CLI application to troubleshoot function entry resolution.

## Without TinyInst
```
crashing_cli.exe
main: 00007FF7D9F671B9
Func1: 00007FF7D9F525C6
Func2: 00007FF7D9F52586
Func3: 00007FF7D9F52546
ImageBase 0x7ff7d9f40000
Callstack:
00: IP: 0x7ff7d9f51e27 - PrintCallStack - crashing_cli.exe + 0x11e27 RET: 0x7ff7d9f524ed RSP: 0x7bf61ee400  
	RtlLookupFunctionEntry   (0x7ff7d9f51e27) @ 00007FF7DA003684 (base 00007FF7D9F40000) -> 0x11dd0 - 0x120e8: 9d3ec (9d3ec)
	Unwind info at 0x7ff7d9fdd3ec
	version 1, flags 3, prolog 32, codes 3, frame reg 0, frame off 0
	0x01: code-offs f, frame-offs 10f, unwind op 1, op info 0 - UWOP_ALLOC_LARGE (0x1340)
	0x02: code-offs 2, frame-offs 7002, unwind op 0, op info 7 - UWOP_PUSH_NONVOL (0xffffffff)
	SymFunctionTableAccess64 (0x7ff7d9f51e27) @ 000001912231D2C8 -> 0x11dd0 - 0x120e8: 9d3ec (9d3ec)
[+] Decoding 0x7ff7d9f51dd0 - 0x7ff7d9f51e02 (50 bytes)
00007FF7D9F51DD0  push rdi
00007FF7D9F51DD2  mov eax, 0x1340
00007FF7D9F51DD7  call 0x00007FF7D9F415FA
00007FF7D9F51DDC  sub rsp, rax
00007FF7D9F51DDF  lea rdi, [rsp+0x50]
00007FF7D9F51DE4  mov ecx, 0x4BC
00007FF7D9F51DE9  mov eax, 0xCCCCCCCC
00007FF7D9F51DEE  rep stosd
00007FF7D9F51DF0  mov rax, [0x00007FF7DA001940]
00007FF7D9F51DF7  xor rax, rsp
00007FF7D9F51DFA  mov [rsp+0x1338], rax
01: IP: 0x7ff7d9f524ed - Func3 - crashing_cli.exe + 0x124ed RET: 0x7ff7d9f52546 RSP: 0x7bf61ef750  
	RtlLookupFunctionEntry   (0x7ff7d9f524ed) @ 00007FF7DA0036A8 (base 00007FF7D9F40000) -> 0x124b0 - 0x12519: 9d450 (9d450)
	Unwind info at 0x7ff7d9fdd450
	version 1, flags 0, prolog 1a, codes 3, frame reg 0, frame off 0
	0x01: code-offs 9, frame-offs 109, unwind op 1, op info 0 - UWOP_ALLOC_LARGE (0x150)
	0x02: code-offs 2, frame-offs 7002, unwind op 0, op info 7 - UWOP_PUSH_NONVOL (0xffffffff)
	SymFunctionTableAccess64 (0x7ff7d9f524ed) @ 000001912231D2C8 -> 0x124b0 - 0x12519: 9d450 (9d450)
[+] Decoding 0x7ff7d9f524b0 - 0x7ff7d9f524ca (26 bytes)
00007FF7D9F524B0  push rdi
00007FF7D9F524B2  sub rsp, 0x150
00007FF7D9F524B9  lea rdi, [rsp+0x20]
00007FF7D9F524BE  mov ecx, 0x4C
00007FF7D9F524C3  mov eax, 0xCCCCCCCC
00007FF7D9F524C8  rep stosd
02: IP: 0x7ff7d9f52546 - Func2 - crashing_cli.exe + 0x12546 RET: 0x7ff7d9f52586 RSP: 0x7bf61ef8b0  
	RtlLookupFunctionEntry   (0x7ff7d9f52546) @ 00007FF7DA0036B4 (base 00007FF7D9F40000) -> 0x12520 - 0x1254c: 9d45c (9d45c)
	Unwind info at 0x7ff7d9fdd45c
	version 1, flags 0, prolog 6, codes 2, frame reg 0, frame off 0
	0x00: code-offs 6, frame-offs 3206, unwind op 2, op info 3 - UWOP_ALLOC_SMALL (0x20)
	0x01: code-offs 2, frame-offs 7002, unwind op 0, op info 7 - UWOP_PUSH_NONVOL (0xffffffff)
	SymFunctionTableAccess64 (0x7ff7d9f52546) @ 000001912231D2C8 -> 0x12520 - 0x1254c: 9d45c (9d45c)
[+] Decoding 0x7ff7d9f52520 - 0x7ff7d9f52526 (6 bytes)
00007FF7D9F52520  push rdi
00007FF7D9F52522  sub rsp, 0x20
03: IP: 0x7ff7d9f52586 - Func1 - crashing_cli.exe + 0x12586 RET: 0x7ff7d9f525c6 RSP: 0x7bf61ef8e0  
	RtlLookupFunctionEntry   (0x7ff7d9f52586) @ 00007FF7DA0036C0 (base 00007FF7D9F40000) -> 0x12560 - 0x1258c: 9d464 (9d464)
	Unwind info at 0x7ff7d9fdd464
	version 1, flags 0, prolog 6, codes 2, frame reg 0, frame off 0
	0x00: code-offs 6, frame-offs 3206, unwind op 2, op info 3 - UWOP_ALLOC_SMALL (0x20)
	0x01: code-offs 2, frame-offs 7002, unwind op 0, op info 7 - UWOP_PUSH_NONVOL (0xffffffff)
	SymFunctionTableAccess64 (0x7ff7d9f52586) @ 000001912231D2C8 -> 0x12560 - 0x1258c: 9d464 (9d464)
[+] Decoding 0x7ff7d9f52560 - 0x7ff7d9f52566 (6 bytes)
00007FF7D9F52560  push rdi
00007FF7D9F52562  sub rsp, 0x20
04: IP: 0x7ff7d9f525c6 - main - crashing_cli.exe + 0x125c6 RET: 0x7ff7d9f671b9 RSP: 0x7bf61ef910  
	RtlLookupFunctionEntry   (0x7ff7d9f525c6) @ 00007FF7DA0036CC (base 00007FF7D9F40000) -> 0x125a0 - 0x125ce: 9d46c (9d46c)
	Unwind info at 0x7ff7d9fdd46c
	version 1, flags 0, prolog 6, codes 2, frame reg 0, frame off 0
	0x00: code-offs 6, frame-offs 3206, unwind op 2, op info 3 - UWOP_ALLOC_SMALL (0x20)
	0x01: code-offs 2, frame-offs 7002, unwind op 0, op info 7 - UWOP_PUSH_NONVOL (0xffffffff)
	SymFunctionTableAccess64 (0x7ff7d9f525c6) @ 000001912231D2C8 -> 0x125a0 - 0x125ce: 9d46c (9d46c)
[+] Decoding 0x7ff7d9f525a0 - 0x7ff7d9f525a6 (6 bytes)
00007FF7D9F525A0  push rdi
00007FF7D9F525A2  sub rsp, 0x20
05: IP: 0x7ff7d9f671b9 - invoke_main - crashing_cli.exe + 0x271b9 RET: 0x7ff7d9f6705e RSP: 0x7bf61ef940  
	RtlLookupFunctionEntry   (0x7ff7d9f671b9) @ 00007FF7DA003C6C (base 00007FF7D9F40000) -> 0x27180 - 0x271be: 9e0c4 (9e0c4)
	Unwind info at 0x7ff7d9fde0c4
	version 1, flags 0, prolog 4, codes 1, frame reg 0, frame off 0
	0x00: code-offs 4, frame-offs 8204, unwind op 2, op info 8 - UWOP_ALLOC_SMALL (0x48)
	SymFunctionTableAccess64 (0x7ff7d9f671b9) @ 000001912231D2C8 -> 0x27180 - 0x271be: 9e0c4 (9e0c4)
[+] Decoding 0x7ff7d9f67180 - 0x7ff7d9f67184 (4 bytes)
00007FF7D9F67180  sub rsp, 0x48
06: IP: 0x7ff7d9f6705e - __scrt_common_main_seh - crashing_cli.exe + 0x2705e RET: 0x7ff7d9f66f1e RSP: 0x7bf61ef990  
	RtlLookupFunctionEntry   (0x7ff7d9f6705e) @ 00007FF7DA003C48 (base 00007FF7D9F40000) -> 0x26f30 - 0x270ce: 9e0e4 (9e0e4)
	Unwind info at 0x7ff7d9fde0e4
	version 1, flags 1, prolog 4, codes 1, frame reg 0, frame off 0
	0x00: code-offs 4, frame-offs c204, unwind op 2, op info c - UWOP_ALLOC_SMALL (0x68)
	SymFunctionTableAccess64 (0x7ff7d9f6705e) @ 000001912231D2C8 -> 0x26f30 - 0x270ce: 9e0e4 (9e0e4)
[+] Decoding 0x7ff7d9f66f30 - 0x7ff7d9f66f34 (4 bytes)
00007FF7D9F66F30  sub rsp, 0x68
07: IP: 0x7ff7d9f66f1e - __scrt_common_main - crashing_cli.exe + 0x26f1e RET: 0x7ff7d9f6724e RSP: 0x7bf61efa00  
	RtlLookupFunctionEntry   (0x7ff7d9f66f1e) @ 00007FF7DA003C3C (base 00007FF7D9F40000) -> 0x26f10 - 0x26f23: 9e110 (9e110)
	Unwind info at 0x7ff7d9fde110
	version 1, flags 0, prolog 4, codes 1, frame reg 0, frame off 0
	0x00: code-offs 4, frame-offs 4204, unwind op 2, op info 4 - UWOP_ALLOC_SMALL (0x28)
	SymFunctionTableAccess64 (0x7ff7d9f66f1e) @ 000001912231D2C8 -> 0x26f10 - 0x26f23: 9e110 (9e110)
[+] Decoding 0x7ff7d9f66f10 - 0x7ff7d9f66f14 (4 bytes)
00007FF7D9F66F10  sub rsp, 0x28
08: IP: 0x7ff7d9f6724e - mainCRTStartup - crashing_cli.exe + 0x2724e RET: 0x7ff867c57034 RSP: 0x7bf61efa30  
	RtlLookupFunctionEntry   (0x7ff7d9f6724e) @ 00007FF7DA003C9C (base 00007FF7D9F40000) -> 0x27240 - 0x27253: 9e118 (9e118)
	Unwind info at 0x7ff7d9fde118
	version 1, flags 0, prolog 9, codes 1, frame reg 0, frame off 0
	0x00: code-offs 9, frame-offs 4209, unwind op 2, op info 4 - UWOP_ALLOC_SMALL (0x28)
	SymFunctionTableAccess64 (0x7ff7d9f6724e) @ 000001912231D2C8 -> 0x27240 - 0x27253: 9e118 (9e118)
[+] Decoding 0x7ff7d9f67240 - 0x7ff7d9f67249 (9 bytes)
00007FF7D9F67240  mov [rsp+0x08], rcx
00007FF7D9F67245  sub rsp, 0x28
09: IP: 0x7ff867c57034 - BaseThreadInitThunk - KERNEL32.DLL + 0x17034 RET: 0x7ff869642651 RSP: 0x7bf61efa60  
	RtlLookupFunctionEntry   (0x7ff867c57034) @ 00007FF867CF5BAC (base 00007FF867C40000) -> 0x17020 - 0x17064: 93fc0 (93fc0)
	Unwind info at 0x7ff867cd3fc0
	version 1, flags 0, prolog 4, codes 1, frame reg 0, frame off 0
	0x00: code-offs 4, frame-offs 4204, unwind op 2, op info 4 - UWOP_ALLOC_SMALL (0x28)
	SymFunctionTableAccess64 (0x7ff867c57034) @ 000001912231D2C8 -> 0x17020 - 0x17064: 93fc0 (93fc0)
[+] Decoding 0x7ff867c57020 - 0x7ff867c57024 (4 bytes)
00007FF867C57020  sub rsp, 0x28
10: IP: 0x7ff869642651 - RtlUserThreadStart - ntdll.dll + 0x52651 RET: 0x0 RSP: 0x7bf61efa90  
	RtlLookupFunctionEntry   (0x7ff869642651) @ 00007FF869762A24 (base 00007FF8695F0000) -> 0x52630 - 0x52679: 146620 (146620)
	Unwind info at 0x7ff869736620
	version 1, flags 1, prolog 4, codes 1, frame reg 0, frame off 0
	0x00: code-offs 4, frame-offs e204, unwind op 2, op info e - UWOP_ALLOC_SMALL (0x78)
	SymFunctionTableAccess64 (0x7ff869642651) @ 000001912231D2C8 -> 0x52630 - 0x52679: 146620 (146620)
[+] Decoding 0x7ff869642630 - 0x7ff869642634 (4 bytes)
00007FF869642630  sub rsp, 0x78
```

## With TinyInst
```
# litecov.exe -generate_unwind -full_address_map -instrument_module crashing_cli.exe -generate-unwind -- C:\Users\wizche\CLionProjects\crashing-cli\cmake-build-debug\crashing_cli.exe
main: 00007FF7D9E817E9
Func1: 00007FF7D9E819D9
Func2: 00007FF7D9E81A2D
Func3: 00007FF7D9E81A81
ImageBase 0x7ff7d9f40000
Callstack:
00: IP: 0x7ff7d9e81c64 - n.a. - crashing_cli.exe + 0xd9e81c64 RET: 0x7ff7d9f6f080 RSP: 0xd14a0fe790  
	RtlLookupFunctionEntry   (0x7ff7d9e81c64) @ 00007FF7D9E936E0 (base 00007FF7D9E70000) -> 0x11bc3 - 0x11f61: 10af0 (10af0)
	Unwind info at 0x7ff7d9e80af0
	version 2, flags 3, prolog 0, codes 5, frame reg 0, frame off 0
	0x01: code-offs 0, frame-offs 600, unwind op 6, op info 0 - UWOP_EPILOG (0x0)
	0x03: code-offs 0, frame-offs 100, unwind op 1, op info 0 - UWOP_ALLOC_LARGE (0x1340)
	0x04: code-offs 0, frame-offs 7000, unwind op 0, op info 7 - UWOP_PUSH_NONVOL (0xffffffff)
[+] Decoding 0x7ff7d9e81bc3 - 0x7ff7d9e81bc3 (0 bytes)
01: IP: 0x7ff7d9e81c64 - n.a. - crashing_cli.exe + 0xd9e81c64 RET: 0x7ff7d9e70000 RSP: 0xd14a0fe798  
	RtlLookupFunctionEntry   (0x7ff7d9e81c64) @ 00007FF7D9E936E0 (base 00007FF7D9E70000) -> 0x11bc3 - 0x11f61: 10af0 (10af0)
	Unwind info at 0x7ff7d9e80af0
	version 2, flags 3, prolog 0, codes 5, frame reg 0, frame off 0
	0x01: code-offs 0, frame-offs 600, unwind op 6, op info 0 - UWOP_EPILOG (0x0)
	0x03: code-offs 0, frame-offs 100, unwind op 1, op info 0 - UWOP_ALLOC_LARGE (0x1340)
	0x04: code-offs 0, frame-offs 7000, unwind op 0, op info 7 - UWOP_PUSH_NONVOL (0xffffffff)
[+] Decoding 0x7ff7d9e81bc3 - 0x7ff7d9e81bc3 (0 bytes)
02: IP: 0x7ff7d9e70000 - n.a. - crashing_cli.exe + 0xd9e70000 RET: 0x7ff7d9e81c64 RSP: 0xd14a0fe7a0  
03: IP: 0x7ff7d9e70000 - n.a. - crashing_cli.exe + 0xd9e70000 RET: 0x7ff7d9f6f08c RSP: 0xd14a0fe7a8  
04: IP: 0x7ff7d9f6f08c - __xt_z - crashing_cli.exe + 0x2f08c RET: 0xd14a0fe810 RSP: 0xd14a0fe7b0  
05: IP: 0xd14a0fe810 - n.a. - crashing_cli.exe + 0x4a0fe810 RET: 0x0 RSP: 0xd14a0fe7b8  
--- crashing_cli.exe
Instrumented module crashing_cli.exe, code size: 184320
Registered function table @ 00007FF7D9E9359C - 00007FF7D9EB6B38 (0x2359c)
Process finished normally
Found 395 new offsets in crashing_cli.exe

Process finished with exit code 0
```

## UNWIND_INFO versions

Windows 11 Enterprise
Version	21H2, build	22000.438
3584: 1: 3868549, 2: 56237

Windows 10 Pro
Version	10.0.19044 Build 19044
3678: 1: 4290453, 2: 92231