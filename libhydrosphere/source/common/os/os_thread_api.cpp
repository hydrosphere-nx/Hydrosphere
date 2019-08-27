/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/hs_macro.hpp>
#include <hs/os/os_thread_api.hpp>
#include <hs/os/os_tls.hpp>
#include <hs/svc.hpp>
#include <os/detail/os_threadlist.hpp>
#include <os/detail/os_virtualmemory_allocator.hpp>
#include <util/util_string_api.hpp>

#include <hs/diag.hpp>
#include <hs/util.hpp>

namespace hs::os {

typedef void (*ThreadEntrypoint)(Thread *);

static void _thread_entry_wrapper(Thread *context) noexcept {
    auto tls_storage = os::ThreadLocalStorage::GetThreadLocalStorage();
    tls_storage->SetThreadContext(context);

    // Make sure the thread context is correctly sync before continuating.
    __HS_ASM("dsb sy");

    auto &critical_section = context->critical_section;
    auto &condition_variable = context->condition_variable;

    critical_section->Enter();

    // Wait until we are really in a starting state (we don't want to break
    // anything here)
    while (context->state == ThreadState::Initialized) {
        condition_variable->Wait(critical_section.GetPointer());
    }

    // Check we are really in a started state!
    if (context->state == ThreadState::Started) {
        critical_section->Leave();
        context->thread_entrypoint(context->thread_argument);
    } else {
        // We are exiting prematuraly!
        critical_section->Leave();
    }

    // TODO(Kaenbyō): TLS destruction

    critical_section->Enter();

    context->state = ThreadState::Exited;
    condition_variable->Broadcast();

    // TODO(Kaenbyō): notify others threads that we are exiting

    critical_section->Leave();

    hs::svc::ExitThread();
}

__HS_ATTRIBUTE_VISIBILITY_HIDDEN hs::Result CreateThreadUnsafe(
    Thread *thread, ThreadEntrypoint entry_point, int cpuid) noexcept {
    size_t retry_count = 0;

    svc::Handle thread_handle;

    while (true) {
        auto result = hs::svc::CreateThread(
            &thread_handle, reinterpret_cast<uintptr_t>(entry_point),
            reinterpret_cast<uintptr_t>(thread),
            reinterpret_cast<uintptr_t>(thread->mapped_thread_stack) +
                thread->thread_stack_size,
            thread->priority, cpuid);
        if (result.Ok()) {
            thread->thread_handle = thread_handle;
            return result;
        } else if ((result.GetValue() & 0x3FFE00) != 0xCE00) {
            __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);
        }

        if (++retry_count > 8) {
            break;
        }
    }

    // out of resources
    return hs::Result(0x1203);
}

__HS_ATTRIBUTE_VISIBILITY_HIDDEN static hs::Result CreateAliasStackUnsafe(
    Thread *thread) noexcept {
    void *stack_mirror_address = hs::os::detail::g_StackAllocator->Reserve(
        thread->thread_stack_size, 0x1000);

    if (stack_mirror_address == nullptr) {
        // out of resources
        return hs::Result(0x1203);
    }

    auto result = hs::svc::MapMemory(
        reinterpret_cast<uintptr_t>(stack_mirror_address),
        reinterpret_cast<uintptr_t>(thread->original_thread_stack),
        thread->thread_stack_size);
    if (result.Ok()) {
        thread->mapped_thread_stack = stack_mirror_address;
        thread->is_alias_thread_stack_mapped = true;
    } else {
        hs::os::detail::g_StackAllocator->Free(stack_mirror_address);
    }

    return result;
}

__HS_ATTRIBUTE_VISIBILITY_HIDDEN void WaitForExitThread(
    Thread *thread) noexcept {
    while (true) {
        int32_t index;
        auto result =
            hs::svc::WaitSynchronization(&index, &thread->thread_handle, 1, -1);

        if (result.Ok()) {
            break;
        }

        if ((result.GetValue() & 0x3FFFFF) != 0xEC01) {
            __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);
        }
    }
}

hs::Result CreateThread(Thread *thread,
                        ThreadEntrypointFunction thread_entrypoint,
                        void *argument, void *stack, size_t stack_size,
                        int priority, int cpuid) noexcept {
    __HS_ABORT_UNLESS_NOT_NULL(thread);
    // TODO(Kaenbyō): preconditions assert
    new (thread) Thread();
    *thread->critical_section = CriticalSection();
    *thread->condition_variable = ConditionVariableImpl();
    thread->thread_entrypoint = thread_entrypoint;
    thread->thread_argument = argument;
    thread->original_thread_stack = stack;
    thread->is_alias_thread_stack_mapped = false;
    thread->thread_stack_size = stack_size;
    thread->priority = priority;
    thread->state = ThreadState::Initialized;

    // TODO(Kaenbyō): TLS slots

    auto result = CreateAliasStackUnsafe(thread);
    __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);

    result = CreateThreadUnsafe(thread, _thread_entry_wrapper, cpuid);
    if (result.Err()) {
        hs::svc::UnmapMemory(
            reinterpret_cast<uintptr_t>(thread->mapped_thread_stack),
            reinterpret_cast<uintptr_t>(thread->original_thread_stack),
            thread->thread_stack_size);
        hs::os::detail::g_StackAllocator->Free(thread->mapped_thread_stack);
    }

    // TODO(Kaenbyō): incremental thread name (with the number of the thread)
    SetThreadName(thread, "UnnamedThread");

    hs::os::detail::ThreadList::Get().AddThread(*thread);

    return result;
}

__HS_ATTRIBUTE_VISIBILITY_HIDDEN void StartThreadUnsafe(
    Thread *thread) noexcept {
    auto result = hs::svc::StartThread(thread->thread_handle);
    __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);
}

__HS_ATTRIBUTE_VISIBILITY_HIDDEN void DestroyThreadUnsafe(
    Thread *thread) noexcept {
    auto result = hs::svc::CloseHandle(thread->thread_handle);
    __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);
}

void DestroyThread(Thread *thread) noexcept {
    auto &critical_section = thread->critical_section;

    critical_section->Enter();
    if (thread->state == ThreadState::Initialized) {
        // Start thread
        thread->state = ThreadState::Destroyed;
        StartThreadUnsafe(thread);
        thread->condition_variable->Signal();
    }
    critical_section->Leave();

    WaitForExitThread(thread);

    critical_section->Enter();
    if (thread->is_alias_thread_stack_mapped) {
        hs::svc::UnmapMemory(
            reinterpret_cast<uintptr_t>(thread->mapped_thread_stack),
            reinterpret_cast<uintptr_t>(thread->original_thread_stack),
            thread->thread_stack_size);
        hs::os::detail::g_StackAllocator->Free(thread->mapped_thread_stack);
        thread->is_alias_thread_stack_mapped = false;
        thread->mapped_thread_stack = nullptr;
    }

    DestroyThreadUnsafe(thread);

    hs::os::detail::ThreadList::Get().RemoveThread(*thread);
    thread->state = ThreadState::Uninitialized;
    thread->thread_name[0] = '\0';
    critical_section->Leave();
}

void StartThread(Thread *thread) noexcept {
    thread->critical_section->Enter();

    StartThreadUnsafe(thread);

    thread->state = ThreadState::Started;
    thread->condition_variable->Signal();

    thread->critical_section->Leave();
}

void WaitThread(Thread *thread) noexcept {
    WaitForExitThread(thread);
    auto &critical_section = thread->critical_section;

    critical_section->Enter();
    if (thread->is_alias_thread_stack_mapped) {
        hs::svc::UnmapMemory(
            reinterpret_cast<uintptr_t>(thread->mapped_thread_stack),
            reinterpret_cast<uintptr_t>(thread->original_thread_stack),
            thread->thread_stack_size);
        hs::os::detail::g_StackAllocator->Free(thread->mapped_thread_stack);
        thread->is_alias_thread_stack_mapped = false;
        thread->mapped_thread_stack = nullptr;
    }
    critical_section->Leave();
}

void YieldThread(void) noexcept { SleepThread(0); }

void SleepThread(int64_t nanoseconds) noexcept {
    hs::svc::SleepThread(nanoseconds);
}

void SetThreadName(Thread *thread, const char *name) noexcept {
    auto name_length = hs::util::Strlen(name);
    if (name_length >= os::THREAD_NAME_SIZE) {
        return;
    }

    // copy and null terminate
    memcpy(thread->thread_name, name, name_length);
    thread->thread_name[name_length] = '\0';
}

int ChangeThreadPriority(Thread *thread, int priority) noexcept {
    auto &critical_section = thread->critical_section;

    critical_section->Enter();
    int thread_priority = thread->priority;
    hs::svc::SetThreadPriority(thread->thread_handle, priority);
    thread->priority = priority;
    critical_section->Leave();

    return thread_priority;
}

Thread *GetCurrentThread(void) noexcept {
    return ThreadLocalStorage::GetThreadLocalStorage()->GetThreadContext();
}

int GetOriginalThreadPriority(Thread *thread) noexcept {
    return thread->priority;
}

int GetCurrentThreadPriority(Thread *thread) noexcept {
    int32_t priority;

    auto result = hs::svc::GetThreadPriority(&priority, thread->thread_handle);
    __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);
    return priority;
}

}  // namespace hs::os
