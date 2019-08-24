/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/os/os_api.hpp>
#include <hs/os/os_mutex_api.hpp>

enum MutexState {
    MutexState_Uninitialized = 0,
    MutexState_Initialized = 1,
};

namespace hs::os {
void InitializeMutex(Mutex *mutex, bool is_recursive) noexcept {
    mutex->critical_section = CriticalSection();
    mutex->is_recursive = is_recursive;
    mutex->counter = 0;
    mutex->owner = hs::svc::Handle();
    mutex->state = MutexState_Initialized;
}

// TODO(Kaenbyō): debug assert on preconditions
void LockMutex(Mutex *mutex) noexcept {
    auto current_thread_handle = hs::os::GetCurrentThreadHandle();

    // We don't own the mutex? enter the critical section and set the owner
    // after that.
    if (mutex->owner != current_thread_handle) {
        mutex->critical_section.Enter();
        mutex->owner = current_thread_handle;
    }

    // If this is recursive, increment the counter.
    if (mutex->is_recursive) {
        mutex->counter++;
    }
}

// TODO(Kaenbyō): debug assert on preconditions
bool TryLockMutex(Mutex *mutex) noexcept {
    auto current_thread_handle = hs::os::GetCurrentThreadHandle();

    if (mutex->is_recursive) {
        // If we don't own the lock and cannot enter the critical section,
        // return false.
        if (mutex->owner != current_thread_handle &&
            !mutex->critical_section.TryEnter()) {
            return false;
        }

        // Otherwise, we own it so it's fine.
        mutex->counter++;
        mutex->owner = current_thread_handle;
        return true;

    } else if (mutex->critical_section.TryEnter()) {
        mutex->owner = current_thread_handle;
        return true;
    }

    return false;
}

// TODO(Kaenbyō): debug assert on preconditions
void UnlockMutex(Mutex *mutex) noexcept {
    // If this lock is recursive, try to decrement the counter and check if we
    // don't hit 0.
    if (mutex->is_recursive) {
        mutex->counter--;
        if (mutex->counter != 0) {
            return;
        }
    }

    mutex->owner = hs::svc::Handle();
    mutex->critical_section.Leave();
}

void FinalizeMutex(Mutex *mutex) noexcept {
    mutex->state = MutexState_Uninitialized;
}

}  // namespace hs::os
