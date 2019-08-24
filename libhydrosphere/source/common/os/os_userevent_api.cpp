/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/os/os_user_event_api.hpp>

enum UserEventState {
    UserEventState_Uninitialized = 0,
    UserEventState_Initialized = 1,
};

namespace hs::os {
void InitializeUserEvent(UserEvent *event, bool is_signaled_at_init,
                         bool is_auto_clear) noexcept {
    event->critical_section = CriticalSection();
    event->condition_variable = ConditionVariableImpl();
    event->is_signaled_at_init = is_signaled_at_init;
    event->is_auto_clear = is_auto_clear;
    event->state = UserEventState_Initialized;
}

void FinalizeUserEvent(UserEvent *event) noexcept {
    event->state = UserEventState_Uninitialized;
}

void WaitUserEvent(UserEvent *event) noexcept {
    event->critical_section.Enter();

    while (!event->is_signaled) {
        event->condition_variable.Wait(&event->critical_section);
    }

    if (event->is_signaled && event->is_auto_clear) {
        event->is_signaled = false;
    }

    event->critical_section.Leave();
}

bool IsUserEventSignaled(UserEvent *event) noexcept {
    event->critical_section.Enter();

    bool is_signaled = event->is_signaled;
    if (event->is_auto_clear) {
        event->is_signaled = false;
    }

    event->critical_section.Leave();

    return is_signaled;
}

void SignalUserEvent(UserEvent *event) noexcept {
    event->critical_section.Enter();

    if (!event->is_signaled) {
        event->is_signaled = true;
        event->condition_variable.Signal();
    }

    event->critical_section.Leave();
}

void ClearUserEvent(UserEvent *event) noexcept {
    event->critical_section.Enter();
    event->is_signaled = false;
    event->critical_section.Leave();
}

}  // namespace hs::os
