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
#include <hs/os/os_critical_section.hpp>
#include <hs/svc.hpp>
#include <hs/util/util_template_api.hpp>

namespace hs::os {
/**
 * \defgroup user_event_api User Event API
 * \short API implementing the user event synchronization primitive.
 * \remark This API is used to share events between threads in the same process.
 * \ingroup os_api
 * \name User Event API
 * \addtogroup user_event_api
 * @{
 */

/**
 * \short This is the context of a user event.
 *
 * See \ref user_event_api "User Event API" for usages.
 **/
struct UserEvent {
    /**
     * \private
     * \short Internal object state.
     */
    uint8_t state;

    /**
     * \private
     * \short True if the UserEvent is signaled at initialization.
     */
    bool is_signaled_at_init;

    /**
     * \private
     * \short True if the UserEvent is signaled.
     */
    bool is_signaled;

    /**
     * \private
     * \short True if the UserEvent must be automatically cleared after a wait
     * operation.
     */
    bool is_auto_clear;

    /**
     * \private
     * \short The lock around the UserEvent.
     */
    CriticalSection critical_section;

    /**
     * \private
     * \short A condition variable used to signal user event.
     */
    ConditionVariableImpl condition_variable;
};

static_assert(sizeof(UserEvent) == 0xC, "invalid event_t size");
static_assert(hs::util::is_pod<UserEvent>::value, "UserEvent isn't pod");

/**
 * \short Create a UserEvent.
 *
 * \param[in] event A pointer to a UserEvent.
 * \param[in] is_signaled_at_init True if the UserEvent must be automatically
 * signaled after the initialization. \param[in] is_auto_clear True if the
 * UserEvent must be automatically cleared after a wait operation.
 *
 * \pre ``event`` is uninitialized.
 * \post ``event`` is initialized.
 */
void InitializeUserEvent(UserEvent *event, bool is_signaled_at_init,
                         bool is_auto_clear) noexcept;
/**
 * \short Wait a signal on a UserEvent.
 *
 * \param[in] event A pointer to a UserEvent.
 *
 * \pre ``event`` is initialized.
 * \post The ``event`` has been signaled.
 */
void WaitUserEvent(UserEvent *event) noexcept;

/**
 * \short Get the signal state of a UserEvent.
 *
 * \param[in] event A pointer to a UserEvent.
 * \pre ``event`` is initialized.
 * \return true if the ``event`` has been signaled.
 */
bool IsUserEventSignaled(UserEvent *event) noexcept;

/**
 * \short Signal a UserEvent.
 *
 * \param[in] event A pointer to a UserEvent.
 *
 * \pre ``event`` is initialized.
 * \post The ``event`` has been signaled.
 */
void SignalUserEvent(UserEvent *event) noexcept;

/**
 * \short Clear the signal state of a UserEvent.
 *
 * \param[in] event A pointer to a UserEvent.
 *
 * \pre ``event`` is initialized.
 * \post The ``event`` has been cleared.
 */
void ClearUserEvent(UserEvent *event) noexcept;

/**
 * \short Finalize a UserEvent.
 *
 * \param[in] event A pointer to a UserEvent.
 *
 * \pre ``event`` is initialized.
 * \post ``event`` is uninitialized.
 */
void FinalizeUserEvent(UserEvent *event) noexcept;

/**
 * @}
 */

}  // namespace hs::os
