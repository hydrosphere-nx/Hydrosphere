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

#include <hs/os/os_condition_variable_impl.hpp>
#include <hs/os/os_critical_section.hpp>
#include <hs/svc.hpp>
#include <hs/util/util_intrusive_list.hpp>
#include <hs/util/util_object_storage.hpp>
#include <hs/util/util_template_api.hpp>

namespace hs::os {
/**
 * \defgroup thread_api Thread API
 * \short API managing Threads.
 * \ingroup os_api
 * \name Thread API
 * \addtogroup thread_api
 * @{
 */

/**
 * \short Thread entrypoint function type.
 * \arg ``argument``: argument given to hs::os::CreateThread.
 */
typedef void (*ThreadEntrypointFunction)(void *argument);

/**
 * \short The max size of a thread name.
 */
const size_t THREAD_NAME_SIZE = 0x20;

/**
 * \short Represent the state of a Thread.
 */
enum class ThreadState {
    /**
     * \short The Thread is uninitialized and can be initialized by using hs::os::CreateThread.
     */
    Uninitialized,
    /**
     * \short The Thread is initialized
     */
    Initialized,
    /**
     * \short The Thread was destroyed before being run.
     */
    Destroyed,
    /**
     * \short The Thread was started and haven't exited.
     */
    Started,
    /**
     * \short The Thread has exit.
     */
    Exited
};

/**
 * \short This is the context of a thread.
 * 
 * See \ref thread_api "Thread API" for usages.
 **/
struct Thread : public hs::util::IntrusiveListElement<> {
    /**
     * \private
     * \short Internal object state.
     */
    ThreadState state;

    /**
     * \private
     * \short The Thread entrypoint.
     */
    ThreadEntrypointFunction thread_entrypoint;

    /**
     * \private
     * \short The Thread argument.
     */
    void *thread_argument;

    /**
     * \private
     * \short The stack pointer given by the user.
     */
    void *original_thread_stack;

    /**
     * \private
     * \short The stack size given by the user.
     */
    size_t thread_stack_size;

    /**
     * \private
     * \short The Thread priority given by the user.
     */
    int priority;

    /**
     * \private
     * \short The memory mirror of the ``original_thread_stack`` mapped in the Stack region.
     */
    void *mapped_thread_stack;

    /**
     * \private
     * \short True if the ``original_thread_stack`` is mapped in the Stack region.
     */
    bool is_alias_thread_stack_mapped;

    /**
     * \private
     * \short A condition variable used to signal thread change of states.
     */
    hs::util::ObjectStorage<ConditionVariableImpl> condition_variable;

    /**
     * \private
     * \short The lock around the Thread.
     */
    hs::util::ObjectStorage<CriticalSection> critical_section;

    /**
     * \private
     * \short The handle of Thread when started.
     */
    svc::Handle thread_handle;

    /**
     * \private
     * \short The Thread name given by the user.
     */
    char thread_name[THREAD_NAME_SIZE];
};

/**
 * \short Create a Thread.
 *
 * \param[in] thread A pointer to a Thread.
 * \param[in] thread_entrypoint The entrypoint of the Thread.
 * \param[in] argument The argument to pass to the entrypoint when starting the Thread.
 * \param[in] stack A pointer to a memory region that will be used as a stack by the Thread.
 * \param[in] stack_size The size of the stack (must be page aligned).
 * \param[in] priority The priority of the Thread (0x2C is the usual priority of the main thread. 0x3B on core 0 to 2 and 0x3F on core 3 is a special priority that enables preemptive multithreading).
 * \param[in] cpuid The ID of the CPU core to use (-2 means the default core of the current process).
 *
 * \pre ``thread`` state is ThreadState::Uninitialized.
 * \pre ``thread_entrypoint`` is not a null pointer.
 * \pre ``stack`` is not a null pointer.
 * \pre ``stack_size`` is page aligned and not equal to 0.
 * \pre ``priority`` is within the range 0-0x3F.
 * \pre ``cpuid`` is within the range 0-3 or is -2.
 *
 * \post ``thread`` state is ThreadState::Initialized.
 */
hs::Result CreateThread(Thread *thread,
                        ThreadEntrypointFunction thread_entrypoint,
                        void *argument, void *stack, size_t stack_size,
                        int priority, int cpuid = -2) noexcept;

/**
 * \short Destroy a Thread.
 *
 * This function has the following behaviours:
 *
 * - If the ``thread`` state is ThreadState::Initialized, the thread is started and signaled as ThreadState::Destroyed (The thread will imediately exit).
 * - It waits for the Thread to exit and ensures that the Thread stack mirror is unmapped if needed.
 *
 * \param[in] thread A pointer to a Thread.
 * \pre ``thread`` state is **not** ThreadState::Uninitialized.
 * \post ``thread`` state is ThreadState::Uninitialized.
 */
void DestroyThread(Thread *thread) noexcept;

/**
 * \short Start a Thread.
 *
 * \param[in] thread A pointer to a Thread.
 * \pre ``thread`` state is ThreadState::Initialized.
 * \post ``thread`` state is ThreadState::Started.
 */
void StartThread(Thread *thread) noexcept;

/**
 * \short Wait for a Thread to exit.
 *
 * \param[in] thread A pointer to a Thread.
 * \pre ``thread`` state is **not** ThreadState::Uninitialized.
 * \post ``thread`` state is ThreadState::Exited.
 */
void WaitThread(Thread *thread) noexcept;

/**
 * \short Yield to other threads.
 *
 * This gives a hint to the scheduler that the current thread is willing to yield its current use of a CPU core.
 */
void YieldThread(void) noexcept;

/**
 * \short Sleep the current Thread for a given amount of nanoseconds.
 */
void SleepThread(int64_t nanoseconds) noexcept;

/**
 * \short Set a Thread name.
 *
 * \pre ``thread`` state is **not** ThreadState::Uninitialized.
 * \pre ``name`` length is inferior to hs::os::THREAD_NAME_SIZE.
 * \post ``thread`` name is ``name``.
 */
void SetThreadName(Thread *thread, const char *name) noexcept;

/**
 * \short Get the current Thread instance.
 */
Thread *GetCurrentThread(void) noexcept;

/**
 * \short Change a Thread priority.
 * 
 * This return the previous thread priority.
 * \pre ``thread`` state is **not** ThreadState::Uninitialized.
 * \post ``thread`` priority is now ``priority``.
 */
int ChangeThreadPriority(Thread *thread, int priority) noexcept;

/**
 * \short Get the original Thread priority.
 *
 * \pre ``thread`` state is **not** ThreadState::Uninitialized.
 */
int GetOriginalThreadPriority(Thread *thread) noexcept;


/**
 * \short Get the current Thread priority.
 *
 * \pre ``thread`` state is **not** ThreadState::Uninitialized.
 */
int GetCurrentThreadPriority(Thread *thread) noexcept;

/**
 * @}
 */

}  // namespace hs::os
