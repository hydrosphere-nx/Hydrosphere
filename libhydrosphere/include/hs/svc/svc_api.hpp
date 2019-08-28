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

#include <hs/hs_config.hpp>
#include <hs/svc/svc_types.hpp>

#ifdef HYDROSPHERE_TARGET_AARCH64
#include <hs/svc/svc_api-aarch64.hpp>
#elif HYDROSPHERE_TARGET_AARCH32
#include <hs/svc/svc_api-aarch32.hpp>
#endif

namespace hs {
namespace svc {

inline hs::Result SetHeapSize(uintptr_t *out_address,
                              size_t heap_size) noexcept {
    return hs::svc::SetHeapSize(out_address, heap_size);
}

inline hs::Result ArbitrateLock(hs::svc::Handle owner_thread_handle,
                                uintptr_t lock_address,
                                hs::svc::Handle requester_handle) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::ArbitrateLock(
        owner_thread_handle, lock_address, requester_handle);
}

inline hs::Result ArbitrateUnlock(uintptr_t lock_address) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::ArbitrateUnlock(lock_address);
}

inline hs::Result OutputDebugString(const char *str, size_t str_size) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::OutputDebugString(str,
                                                                    str_size);
}

inline hs::Result SignalProcessWideKey(uintptr_t address,
                                       int32_t count) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::SignalProcessWideKey(address,
                                                                       count);
}

inline hs::Result WaitProcessWideKeyAtomic(uintptr_t mutex_address,
                                           uintptr_t condvar_address,
                                           hs::svc::Handle thread_handle,
                                           uint64_t timeout) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::WaitProcessWideKeyAtomic(
        mutex_address, condvar_address, thread_handle, timeout);
}

inline void SleepThread(int64_t nanoseconds) noexcept {
    hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::SleepThread(nanoseconds);
}

inline hs::Result CloseHandle(hs::svc::Handle handle) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::CloseHandle(handle);
}

inline hs::Result StartThread(hs::svc::Handle thread_handle) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::StartThread(thread_handle);
}

inline hs::Result WaitSynchronization(int32_t *index,
                                      const hs::svc::Handle *handles,
                                      int32_t handle_count,
                                      int64_t timeout) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::WaitSynchronization(
        index, handles, handle_count, timeout);
}

inline hs::Result CreateThread(hs::svc::Handle *out_thread_handle,
                               uintptr_t thread_entry_point, uintptr_t argument,
                               uintptr_t stack_top, int32_t priority,
                               int32_t cpu_id) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::CreateThread(
        out_thread_handle, thread_entry_point, argument, stack_top, priority,
        cpu_id);
}

inline void ExitThread(void) noexcept __HS_ATTRIBUTE_NORETURN {
    hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::ExitThread();
}

inline void ExitProcess(void) noexcept {
    hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::ExitProcess();
}

inline hs::Result GetThreadPriority(int32_t *priority,
                                    hs::svc::Handle thread_handle) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::GetThreadPriority(
        priority, thread_handle);
}

inline hs::Result SetThreadPriority(hs::svc::Handle thread_handle,
                                    int32_t priority) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::SetThreadPriority(
        thread_handle, priority);
}

inline hs::Result QueryMemory(MemoryInfo *memory_info, uint32_t *page_info,
                              uintptr_t address) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::QueryMemory(
        memory_info, page_info, address);
}

inline hs::Result GetInfo(uint64_t *out_info_value, InfoType info_type,
                          hs::svc::Handle handle,
                          uint64_t info_subtype) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::GetInfo(
        out_info_value, info_type, handle, info_subtype);
}

inline hs::Result MapMemory(uintptr_t dst_address, uintptr_t src_address,
                            uintptr_t size) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::MapMemory(dst_address,
                                                            src_address, size);
}

inline hs::Result UnmapMemory(uintptr_t dst_address, uintptr_t src_address,
                              uintptr_t size) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::UnmapMemory(
        dst_address, src_address, size);
}

inline hs::Result Break(uint32_t break_reason, size_t input_value1,
                        size_t input_value2) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::Break(
        break_reason, input_value1, input_value2);
}

inline uint64_t GetSystemTick(void) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::GetSystemTick();
}

inline hs::Result CreateEvent(hs::svc::Handle *writable_event_handle,
                              hs::svc::Handle *readable_event_handle) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::CreateEvent(
        writable_event_handle, readable_event_handle);
}

inline hs::Result SignalEvent(hs::svc::Handle handle) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::SignalEvent(handle);
}

inline hs::Result ClearEvent(hs::svc::Handle handle) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::ClearEvent(handle);
}

inline hs::Result ResetSignal(hs::svc::Handle handle) noexcept {
    return hs::svc::HYDROSPHERE_TARGET_ARCH_NAME::ResetSignal(handle);
}

}  // namespace svc

}  // namespace hs
