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
#include <hs/os/os_condition_variable_impl.hpp>
#include <hs/os/os_mutex_api.hpp>
#include <hs/svc.hpp>
#include <hs/util/util_template_api.hpp>

namespace hs::os {
/**
 * \defgroup condition_variable_api Condition Variable API
 * \short API implementing the condition variable synchronization primitive.
 * \ingroup os_api
 * \name Condition Variable API
 * \addtogroup condition_variable_api
 * @{
 */

/**
 * \short This is the context of a condition variable.
 *
 * See \ref condition_variable_api "Condition Variable API" for usages.
 **/
struct ConditionVariable {
    /**
     * \private
     * \short Internal object state.
     */
    uint8_t state;

    /**
     * \private
     */
    char reserved[3];

    /**
     * \private
     * \short the actual condition variable implementation.
     */
    ConditionVariableImpl condition_variable;
};

/**
 * \short Type that indicates whether a hs::os::WaitTimeoutConditionVariable
 * returned because of a timeout or not.
 */
enum class ConditionVariableStatus {
    /**
     * \short The function returned without a timeout (i.e., it was signaled).
     */
    NoTimeOut = 0,
    /**
     * \short The function returned because it reached its time limit (timeout).
     */
    TimeOut = 1
};

static_assert(sizeof(ConditionVariable) == 0x8,
              "invalid ConditionVariable size");

static_assert(hs::util::is_pod<ConditionVariable>::value,
              "ConditionVariable isn't pod");

/**
 * \short Initialize a ConditionVariable.
 *
 * \param[in] condvar A pointer to a ConditionVariable.
 *
 * \pre ``condvar`` is uninitialized.
 * \post ``condvar`` is initialized.
 */
void InitializeConditionVariable(ConditionVariable *condvar) noexcept;

/**
 * \short Signal one.
 *
 * \param[in] condvar A pointer to a ConditionVariable.
 *
 * Unblocks one of the threads currently waiting for this condition.
 * \pre ``condvar`` must be initialized and a thread is waiting.
 * \post The waiting thread receives a signal and is unblocked.
 */
void SignalConditionVariable(ConditionVariable *condvar) noexcept;

/**
 * \short Signal all (Broadcast).
 *
 * \param[in] condvar A pointer to a ConditionVariable.
 *
 * Unblocks all threads currently waiting for this condition.
 * \pre ``condvar`` must be initialized and one or multiple threads are waiting.
 * \post All waiting threads receive signaled and are unblocked.
 */
void BroadcastConditionVariable(ConditionVariable *condvar) noexcept;

/**
 * \short Wait for timeout or until signaled.
 *
 * \param[in] condvar A pointer to a ConditionVariable.
 * \param[in] mutex A pointer to a locked Mutex.
 * \param[in] timeout The number of nanoseconds before timing out.
 *
 * The execution of the current thread is blocked during ``timeout``, or until
 * signaled (if the latter happens first). \pre ``condvar`` must be initialized
 * and ``mutex`` must be locked. \post The thread was signaled and was
 * unblocked, or, the ``timeout`` expired.
 */
ConditionVariableStatus WaitTimeoutConditionVariable(ConditionVariable *condvar,
                                                     Mutex *mutex,
                                                     int64_t timeout) noexcept;
/**
 * \short Wait until signaled.
 *
 * \param[in] condvar A pointer to a ConditionVariable.
 * \param[in] mutex A pointer to a locked Mutex.
 *
 * The execution of the current thread is blocked until signaled.
 * \pre ``condvar`` must be initialized and ``mutex`` must be locked.
 * \post The thread was signaled and was unblocked.
 */
void WaitConditionVariable(ConditionVariable *condvar, Mutex *mutex) noexcept;

/**
 * \short Finalize a ConditionVariable.
 *
 * \param[in] condvar A pointer to a ConditionVariable.
 *
 * \pre ``condvar`` is initialized.
 * \post ``condvar`` is uninitialized.
 */
void FinalizeConditionVariable(ConditionVariable *condvar) noexcept;

/**
 * @}
 */

}  // namespace hs::os
