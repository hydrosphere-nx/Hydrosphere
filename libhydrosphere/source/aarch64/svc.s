/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

.align 4

.macro SVC_BEGIN name
    .section .text.\name, "ax", %progbits
    .global \name
    .type \name, %function
    .align 2
    .cfi_startproc
\name:
.endm

.macro SVC_END
    .cfi_endproc
.endm

.macro DEFINE_OUT00_SVC id, name
    SVC_BEGIN \name
        svc \id
        ret
    SVC_END
.endm

.macro DEFINE_OUT32_SVC id, name
    SVC_BEGIN \name
        str x0, [sp, #-0x10]!
        svc \id
        ldr x2, [sp]
        str w1, [x2]
        add sp, sp, #0x10
        ret
    SVC_END
.endm

.macro DEFINE_OUT64_SVC id, name
    SVC_BEGIN \name
        str x0, [sp, #-0x10]!
        svc \id
        ldr x2, [sp]
        str x1, [x2]
        add sp, sp, #0x10
        ret
    SVC_END
.endm

.macro DEFINE_OUT32_ARG2_SVC id, name
    SVC_BEGIN \name
        str x1, [sp, #-0x10]!
        svc \id
        ldr x2, [sp]
        str w1, [x2]
        add sp, sp, #0x10
        ret
    SVC_END
.endm

.macro DEFINE_OUT32_PAIR_SVC id, name
    SVC_BEGIN \name
        stp x0, x1, [sp, #-0x10]!
        svc \id
        ldr x3, [sp]
        str w1, [x3]
        ldr x3, [sp, #8]
        str w2, [x3]
        add sp, sp, #0x10
        ret
    SVC_END
.endm

DEFINE_OUT64_SVC      0x1 _ZN2hs3svc7aarch6411SetHeapSizeEPmm
DEFINE_OUT00_SVC      0x2 _ZN2hs3svc7aarch6419SetMemoryPermissionEmmNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x3 _ZN2hs3svc7aarch6418SetMemoryAttributeEmmjj
DEFINE_OUT00_SVC      0x4 _ZN2hs3svc7aarch649MapMemoryEmmm
DEFINE_OUT00_SVC      0x5 _ZN2hs3svc7aarch6411UnmapMemoryEmmm
DEFINE_OUT32_ARG2_SVC 0x6 _ZN2hs3svc7aarch6411QueryMemoryEPNS0_10MemoryInfoEPjm
DEFINE_OUT00_SVC      0x7 _ZN2hs3svc7aarch6411ExitProcessEv
DEFINE_OUT32_SVC      0x8 _ZN2hs3svc7aarch6412CreateThreadEPNS0_6HandleEmmmii
DEFINE_OUT00_SVC      0x9 _ZN2hs3svc7aarch6411StartThreadENS0_6HandleE
DEFINE_OUT00_SVC      0xA _ZN2hs3svc7aarch6410ExitThreadEv
DEFINE_OUT00_SVC      0xB _ZN2hs3svc7aarch6411SleepThreadEl
DEFINE_OUT32_SVC      0xC _ZN2hs3svc7aarch6417GetThreadPriorityEPiNS0_6HandleE
DEFINE_OUT00_SVC      0xD _ZN2hs3svc7aarch6417SetThreadPriorityENS0_6HandleEi

SVC_BEGIN _ZN2hs3svc7aarch6417GetThreadCoreMaskEPiPlNS0_6HandleE
    stp x0, x1, [sp, #-0x10]!
    svc 0xE
    ldr x3, [sp]
    str w1, [x3]
    ldr x3, [sp, #8]
    str x2, [x3]
    add sp, sp, #0x10
    ret
SVC_END

DEFINE_OUT00_SVC      0x0F _ZN2hs3svc7aarch6417SetThreadCoreMaskENS0_6HandleEil

DEFINE_OUT00_SVC      0x10 _ZN2hs3svc7aarch6425GetCurrentProcessorNumberEv
DEFINE_OUT00_SVC      0x11 _ZN2hs3svc7aarch6411SignalEventENS0_6HandleE
DEFINE_OUT00_SVC      0x12 _ZN2hs3svc7aarch6410ClearEventENS0_6HandleE
DEFINE_OUT00_SVC      0x13 _ZN2hs3svc7aarch6415MapSharedMemoryENS0_6HandleEmmNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x14 _ZN2hs3svc7aarch6417UnmapSharedMemoryENS0_6HandleEmm
DEFINE_OUT32_SVC      0x15 _ZN2hs3svc7aarch6420CreateTransferMemoryEPNS0_6HandleEmmNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x16 _ZN2hs3svc7aarch6411CloseHandleENS0_6HandleE
DEFINE_OUT00_SVC      0x17 _ZN2hs3svc7aarch6411ResetSignalENS0_6HandleE
DEFINE_OUT32_SVC      0x18 _ZN2hs3svc7aarch6419WaitSynchronizationEPiPKNS0_6HandleEim
DEFINE_OUT00_SVC      0x19 _ZN2hs3svc7aarch6421CancelSynchronizationENS0_6HandleE
DEFINE_OUT00_SVC      0x1A _ZN2hs3svc7aarch6413ArbitrateLockENS0_6HandleEmS2_
DEFINE_OUT00_SVC      0x1B _ZN2hs3svc7aarch6415ArbitrateUnlockEm
DEFINE_OUT00_SVC      0x1C _ZN2hs3svc7aarch6424WaitProcessWideKeyAtomicEmmNS0_6HandleEm
DEFINE_OUT00_SVC      0x1D _ZN2hs3svc7aarch6420SignalProcessWideKeyEmi
DEFINE_OUT00_SVC      0x1E _ZN2hs3svc7aarch6413GetSystemTickEv
DEFINE_OUT32_SVC      0x1F _ZN2hs3svc7aarch6418ConnectToNamedPortEPNS0_6HandleEPKc

DEFINE_OUT00_SVC      0x20 _ZN2hs3svc7aarch6420SendSyncRequestLightENS0_6HandleE
DEFINE_OUT00_SVC      0x21 _ZN2hs3svc7aarch6415SendSyncRequestENS0_6HandleE
DEFINE_OUT00_SVC      0x22 _ZN2hs3svc7aarch6429SendSyncRequestWithUserBufferEmmNS0_6HandleE
DEFINE_OUT32_SVC      0x23 _ZN2hs3svc7aarch6430SendAsyncRequestWithUserBufferEPNS0_6HandleEmmS2_
DEFINE_OUT64_SVC      0x24 _ZN2hs3svc7aarch6412GetProcessIdEPmNS0_6HandleE
DEFINE_OUT64_SVC      0x25 _ZN2hs3svc7aarch6411GetThreadIdEPmNS0_6HandleE
DEFINE_OUT00_SVC      0x26 _ZN2hs3svc7aarch645BreakEjmm
DEFINE_OUT00_SVC      0x27 _ZN2hs3svc7aarch6417OutputDebugStringEPKcm
DEFINE_OUT00_SVC      0x28 _ZN2hs3svc7aarch6419ReturnFromExceptionENS_6ResultE
DEFINE_OUT64_SVC      0x29 _ZN2hs3svc7aarch647GetInfoEPmNS0_8InfoTypeENS0_6HandleEm
DEFINE_OUT00_SVC      0x2A _ZN2hs3svc7aarch6420FlushEntireDataCacheEv
DEFINE_OUT00_SVC      0x2B _ZN2hs3svc7aarch6414FlushDataCacheEmm
# [3.0.0+] 0x2C - MapPhysicalMemory
# [3.0.0+] 0x2D - UnmapPhysicalMemory
# [5.0.0+] 0x2E - GetFutureThreadInfo
# TODO(Kaenbyō): [1.0.0+] 0x2F - GetLastThreadInfo

DEFINE_OUT64_SVC      0x30 _ZN2hs3svc7aarch6426GetResourceLimitLimitValueEPmNS0_6HandleENS0_17LimitableResourceE
DEFINE_OUT64_SVC      0x31 _ZN2hs3svc7aarch6428GetResourceLimitCurrentValueEPmNS0_6HandleENS0_17LimitableResourceE
DEFINE_OUT00_SVC      0x32 _ZN2hs3svc7aarch6417SetThreadActivityENS0_6HandleENS0_14ThreadActivityE
# TODO(Kaenbyō): [1.0.0+] 0x33 - GetThreadContext3
# [4.0.0+] 0x34 - WaitForAddress
# [4.0.0+] 0x35 - SignalToAddress
# [8.0.0+] 0x36 - SynchronizePreemptionState
# [1.0.0+] 0x3C - DumpInfo (stubbed?)
# [4.0.0+] 0x3D - DumpInfoNew (subbed?)

DEFINE_OUT32_PAIR_SVC 0x40 _ZN2hs3svc7aarch6413CreateSessionEPNS0_6HandleES3_bm
DEFINE_OUT32_SVC      0x41 _ZN2hs3svc7aarch6413AcceptSessionEPNS0_6HandleES2_
DEFINE_OUT00_SVC      0x42 _ZN2hs3svc7aarch6420ReplyAndReceiveLightENS0_6HandleE
DEFINE_OUT32_SVC      0x43 _ZN2hs3svc7aarch6415ReplyAndReceiveEPiPKNS0_6HandleEiS3_m
DEFINE_OUT32_SVC      0x44 _ZN2hs3svc7aarch6429ReplyAndReceiveWithUserBufferEPimmPKNS0_6HandleEiS3_m
DEFINE_OUT32_PAIR_SVC 0x45 _ZN2hs3svc7aarch6411CreateEventEPNS0_6HandleES3_

# [5.0.0+] 0x48 - MapPhysicalMemoryUnsafe
# [5.0.0+] 0x49 - UnmapPhysicalMemoryUnsafe
# [5.0.0+] 0x4A - SetUnsafeLimit
# [4.0.0+] 0x4B - CreateCodeMemory
# [4.0.0+] 0x4C - ControlCodeMemory

DEFINE_OUT00_SVC      0x4D _ZN2hs3svc7aarch6411SleepSystemEv
DEFINE_OUT32_SVC      0x4E _ZN2hs3svc7aarch6417ReadWriteRegisterEPjmjj
DEFINE_OUT00_SVC      0x4F _ZN2hs3svc7aarch6418SetProcessActivityENS0_6HandleENS0_15ProcessActivityE



DEFINE_OUT32_SVC      0x50 _ZN2hs3svc7aarch6418CreateSharedMemoryEPNS0_6HandleEmNS0_16MemoryPermissionES4_
DEFINE_OUT00_SVC      0x51 _ZN2hs3svc7aarch6417MapTransferMemoryENS0_6HandleEmmNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x52 _ZN2hs3svc7aarch6419UnmapTransferMemoryENS0_6HandleEmm
DEFINE_OUT32_SVC      0x53 _ZN2hs3svc7aarch6420CreateInterruptEventEPNS0_6HandleENS0_9InterruptENS0_13InterruptTypeE
# [1.0.0+] 0x54 - QueryPhysicalAddress (leftover, never used in prod)
DEFINE_OUT64_SVC      0x55 _ZN2hs3svc7aarch6414QueryIoMappingEPmmm
DEFINE_OUT32_SVC      0x56 _ZN2hs3svc7aarch6424CreateDeviceAddressSpaceEPNS0_6HandleEmm
DEFINE_OUT00_SVC      0x57 _ZN2hs3svc7aarch6424AttachDeviceAddressSpaceENS0_10DeviceNameENS0_6HandleE
DEFINE_OUT00_SVC      0x58 _ZN2hs3svc7aarch6424DetachDeviceAddressSpaceENS0_10DeviceNameENS0_6HandleE
DEFINE_OUT00_SVC      0x59 _ZN2hs3svc7aarch6428MapDeviceAddressSpaceByForceENS0_6HandleES2_mmmNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x5A _ZN2hs3svc7aarch6428MapDeviceAddressSpaceAlignedENS0_6HandleES2_mmmNS0_16MemoryPermissionE
DEFINE_OUT64_SVC      0x5B _ZN2hs3svc7aarch6421MapDeviceAddressSpaceEPmNS0_6HandleES3_mmmNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x5C _ZN2hs3svc7aarch6423UnmapDeviceAddressSpaceENS0_6HandleES2_mmm
DEFINE_OUT00_SVC      0x5D _ZN2hs3svc7aarch6426InvalidateProcessDataCacheENS0_6HandleEmm
DEFINE_OUT00_SVC      0x5E _ZN2hs3svc7aarch6421StoreProcessDataCacheENS0_6HandleEmm
DEFINE_OUT00_SVC      0x5F _ZN2hs3svc7aarch6421FlushProcessDataCacheENS0_6HandleEmm


DEFINE_OUT32_SVC      0x60 _ZN2hs3svc7aarch6418DebugActiveProcessEPNS0_6HandleEm
DEFINE_OUT00_SVC      0x61 _ZN2hs3svc7aarch6417BreakDebugProcessENS0_6HandleE
DEFINE_OUT00_SVC      0x62 _ZN2hs3svc7aarch6421TerminateDebugProcessENS0_6HandleE
# TODO(Kaenbyō): [1.0.0+] 0x63 - GetDebugEvent
# TODO(Kaenbyō): [1.0.0+, changed in 3.0.0] 0x64 - ContinueDebugEvent
DEFINE_OUT32_SVC      0x65 _ZN2hs3svc7aarch6414GetProcessListEPiPmi
DEFINE_OUT32_SVC      0x66 _ZN2hs3svc7aarch6413GetThreadListEPiPmiNS0_6HandleE
# TODO(Kaenbyō): [1.0.0+] 0x67 - GetDebugThreadContext
# TODO(Kaenbyō): [1.0.0+] 0x68 - SetDebugThreadContext
# TODO(Kaenbyō): [1.0.0+] 0x69 - QueryDebugProcessMemory
# TODO(Kaenbyō): [1.0.0+] 0x6A - ReadDebugProcessMemory
# TODO(Kaenbyō): [1.0.0+] 0x6B - WriteDebugProcessMemory
# TODO(Kaenbyō): [1.0.0+] 0x6C - SetHardwareBreakPoint
# TODO(Kaenbyō): [1.0.0+] 0x6D - GetDebugThreadParam
# [5.0.0+] 0x6F - GetSystemInfo

DEFINE_OUT32_PAIR_SVC 0x70 _ZN2hs3svc7aarch6410CreatePortEPNS0_6HandleES3_ibm
DEFINE_OUT32_SVC      0x71 _ZN2hs3svc7aarch6415ManageNamedPortEPNS0_6HandleEPKci
DEFINE_OUT32_SVC      0x72 _ZN2hs3svc7aarch6413ConnectToPortEPNS0_6HandleES2_
DEFINE_OUT00_SVC      0x73 _ZN2hs3svc7aarch6426SetProcessMemoryPermissionENS0_6HandleEmmNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x74 _ZN2hs3svc7aarch6416MapProcessMemoryEmNS0_6HandleEmm
DEFINE_OUT00_SVC      0x75 _ZN2hs3svc7aarch6418UnmapProcessMemoryEmNS0_6HandleEmm
DEFINE_OUT32_ARG2_SVC 0x76 _ZN2hs3svc7aarch6418QueryProcessMemoryEPNS0_10MemoryInfoEPjNS0_6HandleEm
DEFINE_OUT00_SVC      0x77 _ZN2hs3svc7aarch6420MapProcessCodeMemoryENS0_6HandleEmmm
DEFINE_OUT00_SVC      0x78 _ZN2hs3svc7aarch6422UnmapProcessCodeMemoryENS0_6HandleEmmm
DEFINE_OUT32_SVC      0x79 _ZN2hs3svc7aarch6413CreateProcessEPNS0_6HandleEPvPKjj
DEFINE_OUT00_SVC      0x7A _ZN2hs3svc7aarch6412StartProcessENS0_6HandleEiim
DEFINE_OUT00_SVC      0x7B _ZN2hs3svc7aarch6416TerminateProcessENS0_6HandleE
DEFINE_OUT64_SVC      0x7C _ZN2hs3svc7aarch6414GetProcessInfoEPmNS0_6HandleENS0_15ProcessInfoTypeE
DEFINE_OUT32_SVC      0x7D _ZN2hs3svc7aarch6419CreateResourceLimitEPNS0_6HandleE
DEFINE_OUT00_SVC      0x7E _ZN2hs3svc7aarch6426SetResourceLimitLimitValueENS0_6HandleENS0_17LimitableResourceEm
DEFINE_OUT00_SVC      0x7F _ZN2hs3svc7aarch6417CallSecureMonitorEv
