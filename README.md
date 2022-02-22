# Crashing CLI
Simple CLI application to troubleshoot function entry resolution.

## Without TinyInst
```
# crashing_cli.exe
main: 00007FF716BA2729
Func1: 00007FF716BA6586
Func2: 00007FF716BA6546
Func3: 00007FF716BA6506
ImageBase 0x7ff716ba0000
Callstack:
00: IP: 0x7ff716ba6081 (GetCallstack) RET: 0x7ff716ba64b6 
	RtlLookupFunctionEntry   (0x7ff716ba6081) @ 00007FF716BAF504 (base 00007FF716BA0000) -> 0x6030 - 0x62c6: cccc (cccc)
		Codes: 3
			[0x00] 0 1
			[0x01] 0 1
			[0x02] 7 0
	SymFunctionTableAccess64 (0x7ff716ba6081) @ 00000208A0AFE848 -> 0x6030 - 0x62c6: cccc (cccc)
01: IP: 0x7ff716ba64b6 (main) RET: 0x7ff716ba6506 
	RtlLookupFunctionEntry   (0x7ff716ba64b6) @ 00007FF716BAF51C (base 00007FF716BA0000) -> 0x6490 - 0x64d0: ccf4 (ccf4)
		Codes: 2
			[0x00] 3 2
			[0x01] 7 0
	SymFunctionTableAccess64 (0x7ff716ba64b6) @ 00000208A0AFE848 -> 0x6490 - 0x64d0: ccf4 (ccf4)
02: IP: 0x7ff716ba6506 (Func2) RET: 0x7ff716ba6546 
	RtlLookupFunctionEntry   (0x7ff716ba6506) @ 00007FF716BAF528 (base 00007FF716BA0000) -> 0x64e0 - 0x650c: ccfc (ccfc)
		Codes: 2
			[0x00] 3 2
			[0x01] 7 0
	SymFunctionTableAccess64 (0x7ff716ba6506) @ 00000208A0AFE848 -> 0x64e0 - 0x650c: ccfc (ccfc)
03: IP: 0x7ff716ba6546 (Func1) RET: 0x7ff716ba6586 
	RtlLookupFunctionEntry   (0x7ff716ba6546) @ 00007FF716BAF534 (base 00007FF716BA0000) -> 0x6520 - 0x654c: cd04 (cd04)
		Codes: 2
			[0x00] 3 2
			[0x01] 7 0
	SymFunctionTableAccess64 (0x7ff716ba6546) @ 00000208A0AFE848 -> 0x6520 - 0x654c: cd04 (cd04)
04: IP: 0x7ff716ba6586 (main) RET: 0x7ff716ba2729 
	RtlLookupFunctionEntry   (0x7ff716ba6586) @ 00007FF716BAF540 (base 00007FF716BA0000) -> 0x6560 - 0x658e: cd0c (cd0c)
		Codes: 2
			[0x00] 3 2
			[0x01] 7 0
	SymFunctionTableAccess64 (0x7ff716ba6586) @ 00000208A0AFE848 -> 0x6560 - 0x658e: cd0c (cd0c)
05: IP: 0x7ff716ba2729 (invoke_main) RET: 0x7ff716ba25ce 
	RtlLookupFunctionEntry   (0x7ff716ba2729) @ 00007FF716BAF174 (base 00007FF716BA0000) -> 0x26f0 - 0x272e: ce7c (ce7c)
		Codes: 1
			[0x00] 8 2
	SymFunctionTableAccess64 (0x7ff716ba2729) @ 00000208A0AFE848 -> 0x26f0 - 0x272e: ce7c (ce7c)
06: IP: 0x7ff716ba25ce (__scrt_common_main_seh) RET: 0x7ff716ba248e 
	RtlLookupFunctionEntry   (0x7ff716ba25ce) @ 00007FF716BAF150 (base 00007FF716BA0000) -> 0x24a0 - 0x263e: ce9c (ce9c)
		Codes: 1
			[0x00] c 2
	SymFunctionTableAccess64 (0x7ff716ba25ce) @ 00000208A0AFE848 -> 0x24a0 - 0x263e: ce9c (ce9c)
07: IP: 0x7ff716ba248e (__scrt_common_main) RET: 0x7ff716ba27be 
	RtlLookupFunctionEntry   (0x7ff716ba248e) @ 00007FF716BAF144 (base 00007FF716BA0000) -> 0x2480 - 0x2493: cec8 (cec8)
		Codes: 1
			[0x00] 4 2
	SymFunctionTableAccess64 (0x7ff716ba248e) @ 00000208A0AFE848 -> 0x2480 - 0x2493: cec8 (cec8)
08: IP: 0x7ff716ba27be (mainCRTStartup) RET: 0x7ffa6a367034 
	RtlLookupFunctionEntry   (0x7ff716ba27be) @ 00007FF716BAF1A4 (base 00007FF716BA0000) -> 0x27b0 - 0x27c3: ced0 (ced0)
		Codes: 1
			[0x00] 4 2
	SymFunctionTableAccess64 (0x7ff716ba27be) @ 00000208A0AFE848 -> 0x27b0 - 0x27c3: ced0 (ced0)
09: IP: 0x7ffa6a367034 (BaseThreadInitThunk) RET: 0x7ffa6b342651 
	RtlLookupFunctionEntry   (0x7ffa6a367034) @ 00007FFA6A405BAC (base 00007FFA6A350000) -> 0x17020 - 0x17064: 93fc0 (93fc0)
		Codes: 1
			[0x00] 4 2
	SymFunctionTableAccess64 (0x7ffa6a367034) @ 00000208A0AFE848 -> 0x17020 - 0x17064: 93fc0 (93fc0)
10: IP: 0x7ffa6b342651 (RtlUserThreadStart) RET: 0x0 
	RtlLookupFunctionEntry   (0x7ffa6b342651) @ 00007FFA6B462A24 (base 00007FFA6B2F0000) -> 0x52630 - 0x52679: 146630 (146630)
		Codes: 1
			[0x00] e 2
	SymFunctionTableAccess64 (0x7ffa6b342651) @ 00000208A0AFE848 -> 0x52630 - 0x52679: 146630 (146630)
```

## With TinyInst
```
# litecov.exe -generate_unwind -full_address_map -instrument_module crashing_cli.exe -generate-unwind -- C:\Users\wizche\CLionProjects\crashing-cli\cmake-build-debug\crashing_cli.exe
main: 00007FF716B70C79
Func1: 00007FF716B7496A
Func2: 00007FF716B749BE
Func3: 00007FF716B74A12
ImageBase 0x7ff716ba0000
Callstack:
00: IP: 0x7ff716b74b23 (n.a.) RET: 0x7ff716baaeb8 
	RtlLookupFunctionEntry   (0x7ff716b74b23) @ 00007FF716B757F8 (base 00007FF716B60000) -> 0x14a7c - 0x14b6b: 106fc (106fc)
		Codes: 5
			[0x00] 0 6
			[0x01] 0 6
			[0x02] 0 1
			[0x03] 0 1
			[0x04] 7 0
01: IP: 0x7ff716b74b23 (n.a.) RET: 0x7ff716b60000 
	RtlLookupFunctionEntry   (0x7ff716b74b23) @ 00007FF716B757F8 (base 00007FF716B60000) -> 0x14a7c - 0x14b6b: 106fc (106fc)
		Codes: 5
			[0x00] 0 6
			[0x01] 0 6
			[0x02] 0 1
			[0x03] 0 1
			[0x04] 7 0
02: IP: 0x7ff716b60000 (n.a.) RET: 0x7ff716b74b23 
03: IP: 0x7ff716b60000 (n.a.) RET: 0x7ff716baaec4 
04: IP: 0x7ff716baaec4 (__xt_z) RET: 0x53e36feb10 
05: IP: 0x53e36feb10 (n.a.) RET: 0x0 
--- crashing_cli.exe
Instrumented module crashing_cli.exe, code size: 36864
Registered function table @ 00007FF716B7545C - 00007FF716B8A8B8 (0x1545c)
Process finished normally
Found 323 new offsets in crashing_cli.exe
```

## UNWIND_INFO versions

Windows 11 Enterprise
Version	21H2, build	22000.438
3584: 1: 3868549, 2: 56237

Windows 10 Pro
Version	10.0.19044 Build 19044
3678: 1: 4290453, 2: 92231