/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/os/os_condition_variable_api.hpp>
#include <hs/util/util_api.hpp>

enum ConditionVariableState {
    ConditionVariableState_Uninitialized = 0,
    ConditionVariableState_Initialized = 1,
};

namespace hs::os {

void InitializeConditionVariable(ConditionVariable *condvar) noexcept {
    condvar->condition_variable = ConditionVariableImpl();
    condvar->state = ConditionVariableState_Initialized;
}

void SignalConditionVariable(ConditionVariable *condvar) noexcept {
    condvar->condition_variable.Signal();
}

void BroadcastConditionVariable(ConditionVariable *condvar) noexcept {
    condvar->condition_variable.Broadcast();
}

ConditionVariableStatus WaitTimeoutConditionVariable(
    ConditionVariable *condvar, Mutex *mutex, int64_t timeout) noexcept {
    if (!condvar->condition_variable.WaitTimeout(&mutex->critical_section,
                                                 timeout)) {
        return ConditionVariableStatus::TimeOut;
    }

    return ConditionVariableStatus::NoTimeOut;
}

void WaitConditionVariable(ConditionVariable *condvar,
                           Mutex *mutex) noexcept {
    condvar->condition_variable.Wait(&mutex->critical_section);
}

void FinalizeConditionVariable(ConditionVariable *condvar) noexcept {
    condvar->state = ConditionVariableState_Uninitialized;
}
}  // namespace hs::os
