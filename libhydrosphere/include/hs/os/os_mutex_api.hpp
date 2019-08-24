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

#include <stdint.h>
#include <hs/os/os_critical_section.hpp>
#include <hs/svc.hpp>
#include <hs/util/util_template_api.hpp>
#include <hs/util/util_object_storage.hpp>

namespace hs::os {
/**
 * \defgroup mutex_api Mutex API
 * \short API ensuring mutual exclusivity.
 * \ingroup os_api
 * \name Mutex API
 * \addtogroup mutex_api
 * @{
 */

/**
 * \short This is the context of a mutex.
 * 
 * See \ref mutex_api "Mutex API" for usages.
 **/
struct Mutex {
    /**
     * \private
     * \short Internal object state.
     */
    uint8_t state;

    /**
     * \private
     * \short Define if this Mutex is recursive or not.
     */
    bool is_recursive;

    /**
     * \private
     * \short Reserved for future usages.
     */
    char reserved[2];

    /**
     * \private
     * \short The thread Handle of the Mutex owner.
     */
    hs::svc::Handle owner;

    /**
     * \private
     * \short Recursive mutex counter
     */
    uint32_t counter;

    /**
     * \private
     * \short The critical section implementation.
     */
    CriticalSection critical_section;
};

static_assert(sizeof(Mutex) == 0x10, "invalid Mutex size");
static_assert(hs::util::is_pod<Mutex>::value, "Mutex isn't pod");

/**
 * \short Initialize a Mutex.
 *
 * \param[in] mutex A pointer to a Mutex.
 * \param[in] is_recursive Define if this Mutex is recursive or not.
 *
 * \pre ``mutex`` is uninitialized.
 * \post ``mutex`` is initialized.
 */
void InitializeMutex(Mutex *mutex, bool is_recursive) noexcept;

/**
 * \short Lock Mutex.
 * The calling thread locks the mutex, blocking if necessary.
 *
 * \remark If the mutex wasn't initialized as recursive and if it is currently locked by the same thread calling this function, it produces a deadlock.
 *
 * \param[in] mutex A pointer to a Mutex.
 *
 * \pre ``mutex`` is initialized.
 * \post The lock was acquired.
 */
void LockMutex(Mutex *mutex) noexcept;

/**
 * \short Lock Mutex if not locked by another thread.
 *
 * \param[in] mutex A pointer to a Mutex.
 *
 * \pre ``mutex`` is initialized.
 * \post The lock was acquired or is owned by another thread.
 *
 * \return true if the function succeeds in locking the mutex for the thread. false otherwise.
 */
bool TryLockMutex(Mutex *mutex) noexcept;

/**
 * \short Unlock Mutex.
 *
 * \param[in] mutex A pointer to a Mutex.
 *
 * \pre ``mutex`` is initialized.
 * \post The lock was released.
 */
void UnlockMutex(Mutex *mutex) noexcept;

/**
 * \short Finalize a Mutex.
 *
 * \param[in] mutex A pointer to a Mutex.
 *
 * \pre ``mutex`` is initialized.
 * \post ``mutex`` is uninitialized.
 */
void FinalizeMutex(Mutex *mutex) noexcept;

/**
 * @}
 */

}  // namespace hs::os
