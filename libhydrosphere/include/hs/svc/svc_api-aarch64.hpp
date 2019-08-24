/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#include <hs/hs_config.hpp>
#include <hs/hs_macro.hpp>
#include <hs/hs_result.hpp>
#include <hs/svc/svc_types.hpp>

#ifndef HYDROSPHERE_TARGET_AARCH64
#error "Importing AArch64 only header!"
#endif

namespace hs {
namespace svc {
namespace aarch64 {
hs::Result SetHeapSize(uintptr_t *out_address, size_t heap_size) noexcept;
hs::Result SetMemoryPermission(uintptr_t address, size_t size,
                               hs::svc::MemoryPermission permission) noexcept;
hs::Result SetMemoryAttribute(uintptr_t address, size_t size, uint32_t state0,
                              uint32_t state1) noexcept;
hs::Result MapMemory(uintptr_t dst_address, uintptr_t src_address,
                     uintptr_t size) noexcept;
hs::Result UnmapMemory(uintptr_t dst_address, uintptr_t src_address,
                       uintptr_t size) noexcept;
hs::Result QueryMemory(MemoryInfo *memory_info, uint32_t *page_info,
                       uintptr_t address) noexcept;
void ExitProcess(void) noexcept;
hs::Result CreateThread(hs::svc::Handle *out_thread_handle,
                        uintptr_t thread_entry_point, uintptr_t argument,
                        uintptr_t stack_top, int32_t priority,
                        int32_t cpu_id) noexcept;
hs::Result StartThread(hs::svc::Handle thread_handle) noexcept;
void ExitThread(void) noexcept __HS_ATTRIBUTE_NORETURN;
void SleepThread(int64_t nanoseconds) noexcept;
hs::Result GetThreadPriority(int32_t *priority,
                             hs::svc::Handle thread_handle) noexcept;
hs::Result SetThreadPriority(hs::svc::Handle thread_handle,
                             int32_t priority) noexcept;
hs::Result GetThreadCoreMask(int32_t *preferred_core, int64_t *affinity_mask,
                             hs::svc::Handle thread_handle) noexcept;
hs::Result SetThreadCoreMask(hs::svc::Handle thread_handle,
                             int32_t preferred_core,
                             int64_t affinity_mask) noexcept;
uint32_t GetCurrentProcessorNumber(void) noexcept;
hs::Result SignalEvent(hs::svc::Handle handle) noexcept;
hs::Result ClearEvent(hs::svc::Handle handle) noexcept;
hs::Result MapSharedMemory(hs::svc::Handle shared_memory_handle,
                           uintptr_t address, size_t size,
                           MemoryPermission permission) noexcept;
hs::Result UnmapSharedMemory(hs::svc::Handle shared_memory_handle,
                             uintptr_t address, size_t size) noexcept;
hs::Result CreateTransferMemory(hs::svc::Handle *out_transfer_memory_handle,
                                uintptr_t address, size_t size,
                                MemoryPermission permission) noexcept;
hs::Result CloseHandle(hs::svc::Handle handle) noexcept;
hs::Result ResetSignal(hs::svc::Handle handle) noexcept;
hs::Result WaitSynchronization(int32_t *index, const hs::svc::Handle *handles,
                               int32_t handle_count, uint64_t timeout) noexcept;
hs::Result CancelSynchronization(hs::svc::Handle thread_handle) noexcept;
hs::Result ArbitrateLock(hs::svc::Handle owner_thread_handle,
                         uintptr_t lock_address,
                         hs::svc::Handle requester_handle) noexcept;
hs::Result ArbitrateUnlock(uintptr_t lock_address) noexcept;
hs::Result WaitProcessWideKeyAtomic(uintptr_t mutex_address,
                                    uintptr_t condvar_address,
                                    hs::svc::Handle thread_handle,
                                    uint64_t timeout) noexcept;
hs::Result SignalProcessWideKey(uintptr_t address, int32_t count) noexcept;
uint64_t GetSystemTick(void) noexcept;
hs::Result ConnectToNamedPort(hs::svc::Handle *session_handle,
                              const char *port_name) noexcept;
hs::Result SendSyncRequestLight(hs::svc::Handle light_session_handle) noexcept;
hs::Result SendSyncRequest(hs::svc::Handle session_handle) noexcept;
hs::Result SendSyncRequestWithUserBuffer(
    uintptr_t user_buffer_address, size_t user_buffer_size,
    hs::svc::Handle session_handle) noexcept;
hs::Result SendAsyncRequestWithUserBuffer(
    hs::svc::Handle *out_readable_event, uintptr_t user_buffer_address,
    size_t user_buffer_size, hs::svc::Handle session_handle) noexcept;
hs::Result GetProcessId(uint64_t *out_process_id,
                        hs::svc::Handle process_handle) noexcept;
hs::Result GetThreadId(uint64_t *out_process_id,
                       hs::svc::Handle thread_handle) noexcept;
// FIXME: is that really a u32?
hs::Result Break(uint32_t break_reason, size_t input_value1,
                 size_t input_value2) noexcept;
hs::Result OutputDebugString(const char *str, size_t str_size) noexcept;
hs::Result ReturnFromException(
    hs::Result return_result) noexcept __HS_ATTRIBUTE_NORETURN;
hs::Result GetInfo(uint64_t *out_info_value, InfoType info_type,
                   hs::svc::Handle handle, uint64_t info_subtype) noexcept;
void FlushEntireDataCache(void) noexcept;
hs::Result FlushDataCache(uintptr_t address, size_t size) noexcept;
// [3.0.0+] 0x2C - MapPhysicalMemory
// [3.0.0+] 0x2D - UnmapPhysicalMemory
// [5.0.0+] 0x2E - GetFutureThreadInfo
// TODO(Kaenbyō): [1.0.0+] 0x2F - GetLastThreadInfo
hs::Result GetResourceLimitLimitValue(
    uint64_t *value, hs::svc::Handle resource_limit_handle,
    hs::svc::LimitableResource limitable_resource) noexcept;
hs::Result GetResourceLimitCurrentValue(
    uint64_t *value, hs::svc::Handle resource_limit_handle,
    hs::svc::LimitableResource limitable_resource) noexcept;
hs::Result SetThreadActivity(hs::svc::Handle thread_handle,
                             hs::svc::ThreadActivity activity) noexcept;
// TODO(Kaenbyō): [1.0.0+] 0x33 - GetThreadContext3
// [4.0.0+] 0x34 - WaitForAddress
// [4.0.0+] 0x35 - SignalToAddress
// [8.0.0+] 0x36 - SynchronizePreemptionState
// [1.0.0+] 0x3C - DumpInfo (stubbed?)
// [4.0.0+] 0x3D - DumpInfoNew (subbed?)
hs::Result CreateSession(hs::svc::Handle *server_handle,
                         hs::svc::Handle *client_handle, bool is_light,
                         uintptr_t unk0) noexcept;
hs::Result AcceptSession(hs::svc::Handle *out_session_handle,
                         hs::svc::Handle port_handle) noexcept;
hs::Result ReplyAndReceiveLight(hs::svc::Handle light_session_handle) noexcept;
hs::Result ReplyAndReceive(int32_t *index, hs::svc::Handle const *handles,
                           int32_t, hs::svc::Handle target_session_handle,
                           uint64_t timeout) noexcept;
hs::Result ReplyAndReceiveWithUserBuffer(
    int32_t *index, uintptr_t user_buffer_address, size_t user_buffer_size,
    hs::svc::Handle const *handles, int32_t,
    hs::svc::Handle target_session_handle, uint64_t timeout) noexcept;
hs::Result CreateEvent(hs::svc::Handle *writable_event_handle,
                       hs::svc::Handle *readable_event_handle) noexcept;
// [5.0.0+] 0x48 - MapPhysicalMemoryUnsafe
// [5.0.0+] 0x49 - UnmapPhysicalMemoryUnsafe
// [5.0.0+] 0x4A - SetUnsafeLimit
// [4.0.0+] 0x4B - CreateCodeMemory
// [4.0.0+] 0x4C - ControlCodeMemory
void SleepSystem(void) noexcept;
hs::Result ReadWriteRegister(uint32_t *value, uintptr_t register_address,
                             uint32_t read_write_mask,
                             uint32_t input_value) noexcept;
hs::Result SetProcessActivity(hs::svc::Handle thread_handle,
                              hs::svc::ProcessActivity activity) noexcept;
hs::Result CreateSharedMemory(hs::svc::Handle *out_shared_memory_handle,
                              size_t size, MemoryPermission my_permission,
                              MemoryPermission other_permission) noexcept;
hs::Result MapTransferMemory(hs::svc::Handle transfer_memory_handle,
                             uintptr_t address, size_t size,
                             MemoryPermission permission) noexcept;
hs::Result UnmapTransferMemory(hs::svc::Handle transfer_memory_handle,
                               uintptr_t address, size_t size) noexcept;
hs::Result CreateInterruptEvent(hs::svc::Handle *out_event_handle,
                                hs::svc::Interrupt interrupt,
                                hs::svc::InterruptType interrupt_type) noexcept;
// [1.0.0+] 0x54 - QueryPhysicalAddress (leftover, never used in prod)
hs::Result QueryIoMapping(uintptr_t *virtual_address,
                          uintptr_t physical_address, size_t size) noexcept;
hs::Result CreateDeviceAddressSpace(hs::svc::Handle *address_space_handle,
                                    uintptr_t address_space_start,
                                    uintptr_t address_space_end) noexcept;
hs::Result AttachDeviceAddressSpace(
    hs::svc::DeviceName device, hs::svc::Handle address_space_handle) noexcept;
hs::Result DetachDeviceAddressSpace(
    hs::svc::DeviceName device, hs::svc::Handle address_space_handle) noexcept;
hs::Result MapDeviceAddressSpaceByForce(
    hs::svc::Handle address_space_handle, hs::svc::Handle process_handle,
    uintptr_t device_map_address, size_t device_map_size,
    uintptr_t device_address, hs::svc::MemoryPermission permission) noexcept;
hs::Result MapDeviceAddressSpaceAligned(
    hs::svc::Handle address_space_handle, hs::svc::Handle process_handle,
    uintptr_t device_map_address, size_t device_map_size,
    uintptr_t device_address, hs::svc::MemoryPermission permission) noexcept;
hs::Result MapDeviceAddressSpace(uintptr_t *unknown,
                                 hs::svc::Handle address_space_handle,
                                 hs::svc::Handle process_handle,
                                 uintptr_t device_map_address,
                                 size_t device_map_size,
                                 uintptr_t device_address,
                                 hs::svc::MemoryPermission permission) noexcept;
hs::Result UnmapDeviceAddressSpace(hs::svc::Handle address_space_handle,
                                   hs::svc::Handle process_handle,
                                   uintptr_t device_map_address,
                                   size_t device_map_size,
                                   uintptr_t device_address) noexcept;
hs::Result InvalidateProcessDataCache(hs::svc::Handle, uintptr_t address,
                                      size_t size) noexcept;
hs::Result StoreProcessDataCache(hs::svc::Handle, uintptr_t address,
                                 size_t size) noexcept;
hs::Result FlushProcessDataCache(hs::svc::Handle, uintptr_t address,
                                 size_t size) noexcept;
hs::Result DebugActiveProcess(hs::svc::Handle *debug_handle,
                              uintptr_t process_id) noexcept;
hs::Result BreakDebugProcess(hs::svc::Handle debug_handle) noexcept;
hs::Result TerminateDebugProcess(hs::svc::Handle debug_handle) noexcept;
// TODO(Kaenbyō): [1.0.0+] 0x63 - GetDebugEvent
// TODO(Kaenbyō): [1.0.0+, changed in 3.0.0] 0x64 - ContinueDebugEvent
hs::Result GetProcessList(int32_t *out_process_id_count, uint64_t *process_ids,
                          int32_t process_ids_array_size) noexcept;
hs::Result GetThreadList(int32_t *out_thread_id_count, uint64_t *thread_ids,
                         int32_t thread_ids_array_size,
                         hs::svc::Handle process_handle) noexcept;
// TODO(Kaenbyō): [1.0.0+] 0x67 - GetDebugThreadContext
// TODO(Kaenbyō): [1.0.0+] 0x68 - SetDebugThreadContext
// TODO(Kaenbyō): [1.0.0+] 0x69 - QueryDebugProcessMemory
// TODO(Kaenbyō): [1.0.0+] 0x6A - ReadDebugProcessMemory
// TODO(Kaenbyō): [1.0.0+] 0x6B - WriteDebugProcessMemory
// TODO(Kaenbyō): [1.0.0+] 0x6C - SetHardwareBreakPoint
// TODO(Kaenbyō): [1.0.0+] 0x6D - GetDebugThreadParam
// [5.0.0+] 0x6F - GetSystemInfo
hs::Result CreatePort(hs::svc::Handle *out_client_port_handle,
                      hs::svc::Handle *out_server_port_handle,
                      int32_t max_sessions, bool is_light,
                      uintptr_t name) noexcept;
hs::Result ManageNamedPort(hs::svc::Handle *out_server_port_handle,
                           char const *name, int32_t max_sessions) noexcept;
hs::Result ConnectToPort(hs::svc::Handle *out_session_handle,
                         hs::svc::Handle client_port_handle) noexcept;
hs::Result SetProcessMemoryPermission(
    hs::svc::Handle process_handle, uintptr_t address, size_t size,
    hs::svc::MemoryPermission permission) noexcept;
hs::Result MapProcessMemory(uintptr_t dst_address,
                            hs::svc::Handle process_handle,
                            uintptr_t src_address, uintptr_t size) noexcept;
hs::Result UnmapProcessMemory(uintptr_t dst_address,
                              hs::svc::Handle process_handle,
                              uintptr_t src_address, uintptr_t size) noexcept;
hs::Result QueryProcessMemory(MemoryInfo *memory_info, uint32_t *page_info,
                              hs::svc::Handle process_handle,
                              uintptr_t address) noexcept;
hs::Result MapProcessCodeMemory(hs::svc::Handle process_handle,
                                uintptr_t dst_address, uintptr_t src_address,
                                uintptr_t size) noexcept;
hs::Result UnmapProcessCodeMemory(hs::svc::Handle process_handle,
                                  uintptr_t dst_address, uintptr_t src_address,
                                  uintptr_t size) noexcept;
hs::Result CreateProcess(hs::svc::Handle *out_process_handle,
                         void *process_info, uint32_t const *capabilities,
                         uint32_t capabilities_count) noexcept;
hs::Result StartProcess(hs::svc::Handle process_handle,
                        int32_t main_thread_priority, int32_t default_cpu_id,
                        size_t main_thread_stack_size) noexcept;
hs::Result TerminateProcess(hs::svc::Handle process_handle) noexcept;
hs::Result GetProcessInfo(uint64_t *out, hs::svc::Handle process_handle,
                          hs::svc::ProcessInfoType process_info_type) noexcept;
hs::Result CreateResourceLimit(hs::svc::Handle *resource_limit_handle) noexcept;
hs::Result SetResourceLimitLimitValue(
    hs::svc::Handle resource_limit_handle,
    hs::svc::LimitableResource limitable_resource, uint64_t value) noexcept;
void CallSecureMonitor(void) noexcept;
}  // namespace aarch64
}  // namespace svc

}  // namespace hs
