/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

.arm
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

.macro DEFINE_OUT00_SVC id name
    SVC_BEGIN \name
        svc \id
        bx lr
    SVC_END
.endm

.macro DEFINE_OUT32_SVC id, name
    SVC_BEGIN \name
        str r0, [sp, #-0x4]!
        svc \id
        ldr r2, [sp]
        str r1, [r2]
        add sp, sp, #0x4
        bx lr
    SVC_END
.endm

.macro DEFINE_OUT32_ARG2_SVC id, name
    SVC_BEGIN \name
        str r1, [sp, #-0x4]!
        svc \id
        ldr r2, [sp]
        str r1, [r2]
        add sp, sp, #0x4
        bx lr
    SVC_END
.endm


DEFINE_OUT32_SVC      0x1 _ZN2hs3svc7aarch3211SetHeapSizeEPjj
DEFINE_OUT00_SVC      0x2 _ZN2hs3svc7aarch3219SetMemoryPermissionEjjNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x3 _ZN2hs3svc7aarch3218SetMemoryAttributeEjjjj
DEFINE_OUT00_SVC      0x4 _ZN2hs3svc7aarch329MapMemoryEjjj
DEFINE_OUT00_SVC      0x5 _ZN2hs3svc7aarch3211UnmapMemoryEjjj
DEFINE_OUT32_ARG2_SVC 0x6 _ZN2hs3svc7aarch3211QueryMemoryEPNS0_10MemoryInfoEPjj
DEFINE_OUT00_SVC      0x7 _ZN2hs3svc7aarch3211ExitProcessEv

SVC_BEGIN _ZN2hs3svc7aarch3212CreateThreadEPNS0_6HandleEjjjii
    stmfd sp!, {r0, r4}
    ldr r0, [sp, #0x8]
    ldr r4, [sp, #0xC]
    svc 0x8
    ldr r2, [sp]
    str r1, [r2]
    add sp, sp, #0x4
    pop {r4}
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x9 _ZN2hs3svc7aarch3211StartThreadENS0_6HandleE
DEFINE_OUT00_SVC      0xA _ZN2hs3svc7aarch3210ExitThreadEv
DEFINE_OUT00_SVC      0xB _ZN2hs3svc7aarch3211SleepThreadEx
DEFINE_OUT32_SVC      0xC _ZN2hs3svc7aarch3217GetThreadPriorityEPiNS0_6HandleE
DEFINE_OUT00_SVC      0xD _ZN2hs3svc7aarch3217SetThreadPriorityENS0_6HandleEi

SVC_BEGIN _ZN2hs3svc7aarch3217GetThreadCoreMaskEPiPxNS0_6HandleE
    stmfd sp!, {r0, r1, r4}
    svc 0xE
    ldr r4, [sp]
    str r1, [r4]
    ldr r4, [sp, #0x4]
    str r2, [r4]
    str r3, [r4, #0x4]
    add sp, sp, #0x4
    pop {r4}
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x0F _ZN2hs3svc7aarch3217SetThreadCoreMaskENS0_6HandleEix

DEFINE_OUT00_SVC      0x10 _ZN2hs3svc7aarch3225GetCurrentProcessorNumberEv
DEFINE_OUT00_SVC      0x11 _ZN2hs3svc7aarch3211SignalEventENS0_6HandleE
DEFINE_OUT00_SVC      0x12 _ZN2hs3svc7aarch3210ClearEventENS0_6HandleE
DEFINE_OUT00_SVC      0x13 _ZN2hs3svc7aarch3215MapSharedMemoryENS0_6HandleEjjNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x14 _ZN2hs3svc7aarch3217UnmapSharedMemoryENS0_6HandleEjj
DEFINE_OUT32_SVC      0x15 _ZN2hs3svc7aarch3220CreateTransferMemoryEPNS0_6HandleEjjNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x16 _ZN2hs3svc7aarch3211CloseHandleENS0_6HandleE
DEFINE_OUT00_SVC      0x17 _ZN2hs3svc7aarch3211ResetSignalENS0_6HandleE

SVC_BEGIN _ZN2hs3svc7aarch3219WaitSynchronizationEPiPKNS0_6HandleEiy
    str r0, [sp, #-0x4]!
    ldr r0, [sp, #0x4]
    ldr r3, [sp, #0x8]
    svc 0x18
    ldr r2, [sp]
    str r1, [r2]
    add sp, sp, #0x4
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x19 _ZN2hs3svc7aarch3221CancelSynchronizationENS0_6HandleE
DEFINE_OUT00_SVC      0x1A _ZN2hs3svc7aarch3213ArbitrateLockENS0_6HandleEjS2_
DEFINE_OUT00_SVC      0x1B _ZN2hs3svc7aarch3215ArbitrateUnlockEj

SVC_BEGIN _ZN2hs3svc7aarch3224WaitProcessWideKeyAtomicEjjNS0_6HandleEy
    str r4, [sp, #-0x4]!
    ldr r3, [sp, #0x4]
    ldr r4, [sp, #0x8]
    svc 0x1C
    pop {r4}
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x1D _ZN2hs3svc7aarch3220SignalProcessWideKeyEji
DEFINE_OUT00_SVC      0x1E _ZN2hs3svc7aarch3213GetSystemTickEv
DEFINE_OUT32_SVC      0x1F _ZN2hs3svc7aarch3218ConnectToNamedPortEPNS0_6HandleEPKc

DEFINE_OUT00_SVC      0x20 _ZN2hs3svc7aarch3220SendSyncRequestLightENS0_6HandleE
DEFINE_OUT00_SVC      0x21 _ZN2hs3svc7aarch3215SendSyncRequestENS0_6HandleE
DEFINE_OUT00_SVC      0x22 _ZN2hs3svc7aarch3229SendSyncRequestWithUserBufferEjjNS0_6HandleE
DEFINE_OUT32_SVC      0x23 _ZN2hs3svc7aarch3230SendAsyncRequestWithUserBufferEPNS0_6HandleEjjS2_

SVC_BEGIN _ZN2hs3svc7aarch3212GetProcessIdEPyNS0_6HandleE
    str r0, [sp, #-0x4]!
    svc 0x24
    ldr r3, [sp]
    str r1, [r3]
    str r2, [r3, #0x4]
    add sp, sp, #0x4
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3211GetThreadIdEPyNS0_6HandleE
    str r0, [sp, #-0x4]!
    svc 0x25
    ldr r3, [sp]
    str r1, [r3]
    str r2, [r3, #0x4]
    add sp, sp, #0x4
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x26 _ZN2hs3svc7aarch325BreakEjjj
DEFINE_OUT00_SVC      0x27 _ZN2hs3svc7aarch3217OutputDebugStringEPKcj
DEFINE_OUT00_SVC      0x28 _ZN2hs3svc7aarch3219ReturnFromExceptionENS_6ResultE

SVC_BEGIN _ZN2hs3svc7aarch327GetInfoEPyNS0_8InfoTypeENS0_6HandleEy
    str r0, [sp, #-0x4]!
    ldr r0, [sp, #0x4]
    ldr r3, [sp, #0x8]
    svc 0x29
    ldr r3, [sp]
    str r1, [r3]
    str r2, [r3, #0x4]
    add sp, sp, #0x4
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x2A _ZN2hs3svc7aarch3220FlushEntireDataCacheEv
DEFINE_OUT00_SVC      0x2B _ZN2hs3svc7aarch3214FlushDataCacheEjj
# [3.0.0+] 0x2C - MapPhysicalMemory
# [3.0.0+] 0x2D - UnmapPhysicalMemory
# [5.0.0+] 0x2E - GetFutureThreadInfo
# TODO(Kaenbyō): [1.0.0+] 0x2F - GetLastThreadInfo

SVC_BEGIN _ZN2hs3svc7aarch3226GetResourceLimitLimitValueEPyNS0_6HandleENS0_17LimitableResourceE
    str r0, [sp, #-0x4]!
    svc 0x30
    ldr r3, [sp]
    str r1, [r3]
    str r2, [r3, #0x4]
    add sp, sp, #0x4
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3228GetResourceLimitCurrentValueEPyNS0_6HandleENS0_17LimitableResourceE
    str r0, [sp, #-0x4]!
    svc 0x31
    ldr r3, [sp]
    str r1, [r3]
    str r2, [r3, #0x4]
    add sp, sp, #0x4
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x32 _ZN2hs3svc7aarch3217SetThreadActivityENS0_6HandleENS0_14ThreadActivityE
# TODO(Kaenbyō): [1.0.0+] 0x33 - GetThreadContext3
# [4.0.0+] 0x34 - WaitForAddress
# [4.0.0+] 0x35 - SignalToAddress
# [8.0.0+] 0x36 - SynchronizePreemptionState
# [1.0.0+] 0x3C - DumpInfo (stubbed?)
# [4.0.0+] 0x3D - DumpInfoNew (subbed?)

SVC_BEGIN _ZN2hs3svc7aarch3213CreateSessionEPNS0_6HandleES3_bj
    stmfd sp!, {r0, r1}
    svc 0x40
    ldr r3, [sp]
    str r1, [r3]
    ldr r3, [sp, #0x4]
    str r2, [r3]
    add sp, sp, #0x8
    bx lr
SVC_END

DEFINE_OUT32_SVC      0x41 _ZN2hs3svc7aarch3213AcceptSessionEPNS0_6HandleES2_
DEFINE_OUT00_SVC      0x42 _ZN2hs3svc7aarch3220ReplyAndReceiveLightENS0_6HandleE
SVC_BEGIN _ZN2hs3svc7aarch3215ReplyAndReceiveEPiPKNS0_6HandleEiS3_y
    stmfd sp!, {r0, r4}
    ldr r0, [sp, #0x8]
    ldr r4, [sp, #0xC]
    svc 0x43
    ldr r2, [sp]
    str r1, [r2]
    add sp, sp, #0x4
    pop {r4}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3229ReplyAndReceiveWithUserBufferEPijjPKNS0_6HandleEiS3_y
    stmfd sp!, {r0, r4-r6}
    ldr r0, [sp, #0x10]
    ldr r4, [sp, #0x14]
    ldr r5, [sp, #0x18]
    ldr r6, [sp, #0x1C]
    svc 0x44
    ldr r2, [sp]
    str r1, [r2]
    add sp, sp, #4
    ldmfd sp!, {r4-r6}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3211CreateEventEPNS0_6HandleES3_
    stmfd sp!, {r0, r1}
    svc 0x45
    ldr r3, [sp]
    str r1, [r3]
    ldr r3, [sp, #0x4]
    str r2, [r3]
    add sp, sp, #0x8
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x4D _ZN2hs3svc7aarch3211SleepSystemEv

SVC_BEGIN _ZN2hs3svc7aarch3217ReadWriteRegisterEPjjjj
    str r0, [sp, #-0x4]!
    ldr r0, [sp, #0x4]
    ldr r1, [sp, #0x8]
    svc 0x4E
    ldr r2, [sp]
    str r1, [r2]
    add sp, sp, #0x4
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x4F _ZN2hs3svc7aarch3218SetProcessActivityENS0_6HandleENS0_15ProcessActivityE

DEFINE_OUT32_SVC      0x50 _ZN2hs3svc7aarch3218CreateSharedMemoryEPNS0_6HandleEjNS0_16MemoryPermissionES4_
DEFINE_OUT00_SVC      0x51 _ZN2hs3svc7aarch3217MapTransferMemoryENS0_6HandleEjjNS0_16MemoryPermissionE
DEFINE_OUT00_SVC      0x52 _ZN2hs3svc7aarch3219UnmapTransferMemoryENS0_6HandleEjj
DEFINE_OUT32_SVC      0x53 _ZN2hs3svc7aarch3220CreateInterruptEventEPNS0_6HandleENS0_9InterruptENS0_13InterruptTypeE
# [1.0.0+] 0x54 - QueryPhysicalAddress (leftover, never used in prod)

SVC_BEGIN _ZN2hs3svc7aarch3214QueryIoMappingEPjjj
    str r0, [sp, #-0x4]!
    ldr r0, [sp, #0x4]
    svc 0x55
    ldr r2, [sp]
    str r1, [r2]
    add sp, sp, #0x4
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3224CreateDeviceAddressSpaceEPNS0_6HandleEjj
    str r0, [sp, #-0x4]!
    ldr r0, [sp, #0x4]
    ldr r1, [sp, #0x8]
    svc 0x56
    ldr r2, [sp]
    str r1, [r2]
    add sp, sp, #0x4
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x57 _ZN2hs3svc7aarch3224AttachDeviceAddressSpaceENS0_10DeviceNameENS0_6HandleE
DEFINE_OUT00_SVC      0x58 _ZN2hs3svc7aarch3224DetachDeviceAddressSpaceENS0_10DeviceNameENS0_6HandleE

SVC_BEGIN _ZN2hs3svc7aarch3228MapDeviceAddressSpaceByForceENS0_6HandleES2_jjjNS0_16MemoryPermissionE
    stmfd sp!, {r4-r7}
    ldr r4, [sp, #0x10]
    ldr r5, [sp, #0x18]
    ldr r6, [sp, #0x1C]
    ldr r7, [sp, #0x20]
    svc 0x59
    ldmfd sp!, {r4-r7}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3228MapDeviceAddressSpaceAlignedENS0_6HandleES2_jjjNS0_16MemoryPermissionE
    stmfd sp!, {r4-r7}
    ldr r4, [sp, #0x10]
    ldr r5, [sp, #0x18]
    ldr r6, [sp, #0x1C]
    ldr r7, [sp, #0x20]
    svc 0x5A
    ldmfd sp!, {r4-r7}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3221MapDeviceAddressSpaceEPjNS0_6HandleES3_jjjNS0_16MemoryPermissionE
    stmfd sp!, {r0, r4-r7}
    ldr r0, [sp, #0x14]
    ldr r3, [sp, #0x18]
    ldr r4, [sp, #0x1C]
    ldr r5, [sp, #0x24]
    ldr r6, [sp, #0x28]
    ldr r7, [sp, #0x2C]
    svc 0x5B
    ldr r2, [sp]
    str r1, [r2]
    add sp, sp, #0x4
    ldmfd sp!, {r4-r7}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3223UnmapDeviceAddressSpaceENS0_6HandleES2_jjj
    stmfd sp!, {r4-r6}
    ldr r4, [sp, #0xC]
    ldr r5, [sp, #0x14]
    ldr r6, [sp, #0x18]
    svc 0x5C
    ldmfd sp!, {r4-r6}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3226InvalidateProcessDataCacheENS0_6HandleEjj
    str r4, [sp, #-0x4]!
    ldr r1, [sp, #0x4]
    ldr r4, [sp, #0x8]
    svc 0x5D
    pop {r4}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3221StoreProcessDataCacheENS0_6HandleEjj
    str r4, [sp, #-0x4]!
    ldr r1, [sp, #0x4]
    ldr r4, [sp, #0x8]
    svc 0x5E
    pop {r4}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3221FlushProcessDataCacheENS0_6HandleEjj
    str r4, [sp, #-0x4]!
    ldr r1, [sp, #0x4]
    ldr r4, [sp, #0x8]
    svc 0x5F
    pop {r4}
    bx lr
SVC_END

DEFINE_OUT32_SVC      0x60 _ZN2hs3svc7aarch3218DebugActiveProcessEPNS0_6HandleEj
DEFINE_OUT00_SVC      0x61 _ZN2hs3svc7aarch3217BreakDebugProcessENS0_6HandleE
DEFINE_OUT00_SVC      0x62 _ZN2hs3svc7aarch3221TerminateDebugProcessENS0_6HandleE
# TODO(Kaenbyō): [1.0.0+] 0x63 - GetDebugEvent
# TODO(Kaenbyō): [1.0.0+, changed in 3.0.0] 0x64 - ContinueDebugEvent
DEFINE_OUT32_SVC      0x65 _ZN2hs3svc7aarch3214GetProcessListEPiPyi
DEFINE_OUT32_SVC      0x66 _ZN2hs3svc7aarch3213GetThreadListEPiPyiNS0_6HandleE
# TODO(Kaenbyō): [1.0.0+] 0x67 - GetDebugThreadContext
# TODO(Kaenbyō): [1.0.0+] 0x68 - SetDebugThreadContext
# TODO(Kaenbyō): [1.0.0+] 0x69 - QueryDebugProcessMemory
# TODO(Kaenbyō): [1.0.0+] 0x6A - ReadDebugProcessMemory
# TODO(Kaenbyō): [1.0.0+] 0x6B - WriteDebugProcessMemory
# TODO(Kaenbyō): [1.0.0+] 0x6C - SetHardwareBreakPoint
# TODO(Kaenbyō): [1.0.0+] 0x6D - GetDebugThreadParam
# [5.0.0+] 0x6F - GetSystemInfo

SVC_BEGIN _ZN2hs3svc7aarch3210CreatePortEPNS0_6HandleES3_ibj
    stmfd sp!, {r0, r1}
    ldr r0, [sp, #0x8]
    svc 0x70
    ldr r3, [sp]
    str r1, [r3]
    ldr r3, [sp, #0x4]
    str r2, [r3]
    add sp, sp, #0x8
    bx lr
SVC_END

DEFINE_OUT32_SVC      0x71 _ZN2hs3svc7aarch3215ManageNamedPortEPNS0_6HandleEPKci
DEFINE_OUT32_SVC      0x72 _ZN2hs3svc7aarch3213ConnectToPortEPNS0_6HandleES2_

SVC_BEGIN _ZN2hs3svc7aarch3226SetProcessMemoryPermissionENS0_6HandleEjjNS0_16MemoryPermissionE
    stmfd sp!, {r4, r5}
    ldr r1, [sp, #0x8]
    ldr r4, [sp, #0xC]
    ldr r5, [sp, #0x10]
    svc 0x73
    ldmfd sp!, {r4, r5}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3216MapProcessMemoryEjNS0_6HandleEjj
    str r4, [sp, #-0x4]!
    ldr r4, [sp, #0x4]
    svc 0x74
    pop {r4}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3218UnmapProcessMemoryEjNS0_6HandleEjj
    str r4, [sp, #-0x4]!
    ldr r4, [sp, #0x4]
    svc 0x75
    pop {r4}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3218QueryProcessMemoryEPNS0_10MemoryInfoEPjNS0_6HandleEj
    str r1, [sp, #-0x4]!
    ldr r1, [sp, #0x4]
    ldr r3, [sp, #0x8]
    svc 0x76
    ldr r2, [sp]
    str r1, [r2]
    add sp, sp, #0x4
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3220MapProcessCodeMemoryENS0_6HandleEjjj
    stmfd sp!, {r4-r6}
    ldr r1, [sp, #0xC]
    ldr r4, [sp, #0x10]
    ldr r5, [sp, #0x14]
    ldr r6, [sp, #0x18]
    svc 0x77
    ldmfd sp!, {r4-r6}
    bx lr
SVC_END

SVC_BEGIN _ZN2hs3svc7aarch3222UnmapProcessCodeMemoryENS0_6HandleEjjj
    stmfd sp!, {r4-r6}
    ldr r1, [sp, #0xC]
    ldr r4, [sp, #0x10]
    ldr r5, [sp, #0x14]
    ldr r6, [sp, #0x18]
    svc 0x78
    ldmfd sp!, {r4-r6}
    bx lr
SVC_END

DEFINE_OUT32_SVC      0x79 _ZN2hs3svc7aarch3213CreateProcessEPNS0_6HandleEPvPKjj

SVC_BEGIN _ZN2hs3svc7aarch3212StartProcessENS0_6HandleEiij
    str r4, [sp, #-0x4]!
    ldr r3, [sp, #0x4]
    ldr r3, [sp, #0x8]
    svc 0x7A
    pop {r4}
    bx lr
SVC_END

DEFINE_OUT00_SVC      0x7B _ZN2hs3svc7aarch3216TerminateProcessENS0_6HandleE

SVC_BEGIN _ZN2hs3svc7aarch3214GetProcessInfoEPyNS0_6HandleENS0_15ProcessInfoTypeE
    str r0, [sp, #-0x4]!
    svc 0x7C
    ldr r3, [sp]
    str r1, [r3]
    str r2, [r3, #0x4]
    add sp, sp, #0x4
    bx lr
SVC_END


DEFINE_OUT32_SVC      0x7D _ZN2hs3svc7aarch3219CreateResourceLimitEPNS0_6HandleE
DEFINE_OUT00_SVC      0x7E _ZN2hs3svc7aarch3226SetResourceLimitLimitValueENS0_6HandleENS0_17LimitableResourceEy
DEFINE_OUT00_SVC      0x7F _ZN2hs3svc7aarch3217CallSecureMonitorEv