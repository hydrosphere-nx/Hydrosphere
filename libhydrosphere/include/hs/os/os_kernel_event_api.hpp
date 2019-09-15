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
#include <hs/util/util_optional.hpp>
#include <hs/util/util_template_api.hpp>

namespace hs::os {
/**
 * \defgroup kernel_event_api Kernel Event API
 * \short API implementing the kernel event synchronization primitive.
 * \remark This API is used to share events between processes.
 * \ingroup os_api
 * \name Kernel Event API
 * \addtogroup kernel_event_api
 * @{
 */

/**
 * \short This is the context of a kernel event.
 *
 * See \ref kernel_event_api "Kernel Event API" for usages.
 **/
struct KernelEvent {
    /**
     * \private
     * \short Internal object state.
     */
    uint8_t state;

    /**
     * \private
     * \short True if the KernelEvent must be automatically cleared after a wait
     * operation.
     */
    bool is_auto_clear;

    /**
     * \private
     * \short Reserved for future usage.
     */
    char reserved[2];

    /**
     * \private
     * \short The readable handle.
     */
    util::Optional<svc::Handle> readable_handle;

    /**
     * \private
     * \short The writable handle.
     */
    util::Optional<svc::Handle> writable_handle;
};

static_assert(sizeof(KernelEvent) == 0x14, "invalid KernelEvent size");
static_assert(hs::util::is_pod<KernelEvent>::value, "KernelEvent isn't pod");
/**
 * \short Create a KernelEvent.
 *
 * \param[in] event A pointer to a KernelEvent.
 * \param[in] is_auto_clear True if the KernelEvent must be automatically
 * cleared after a wait operation.
 *
 * \pre ``event`` is uninitialized.
 * \post ``event`` is initialized.
 */
hs::Result CreateKernelEvent(KernelEvent *event, bool is_auto_clear) noexcept;

/**
 * \short Load a KernelEvent from raw handles.
 *
 * \param[in] event A pointer to a KernelEvent.
 * \param[in] readable_handle An Optional readable handle.
 * \param[in] writable_handle An Optional writable handle.
 *
 * \pre ``event`` is uninitialized.
 * \pre ``readable_handle`` or ``writable_handle`` contain a value.
 * \post ``event`` is initialized.
 */
void LoadKernelEvent(KernelEvent *event,
                     util::Optional<svc::Handle> readable_handle,
                     util::Optional<svc::Handle> writable_handle) noexcept;

/**
 * \short Wait a signal on a KernelEvent.
 *
 * \param[in] event A pointer to a KernelEvent.
 *
 * \pre ``event`` is initialized.
 * \pre ``event`` contains a readable handle.
 * \post The ``event`` has been signaled.
 */
void WaitKernelEvent(KernelEvent *event) noexcept;

/**
 * \short Get the signal state of a KernelEvent.
 *
 * \param[in] event A pointer to a KernelEvent.
 * \pre ``event`` is initialized.
 * \pre ``event`` contains a readable handle.
 * \return true if the ``event`` has been signaled.
 */
bool IsKernelEventSignaled(KernelEvent *event) noexcept;

/**
 * \short Signal a KernelEvent.
 *
 * \param[in] event A pointer to a KernelEvent.
 *
 * \pre ``event`` is initialized.
 * \pre ``event`` contains a writable handle.
 * \post The ``event`` has been signaled.
 */
hs::Result SignalKernelEvent(KernelEvent *event) noexcept;

/**
 * \short Clear the signal state of a KernelEvent.
 *
 * \param[in] event A pointer to a KernelEvent.
 *
 * \pre ``event`` is initialized.
 * \post The ``event`` has been cleared.
 */
hs::Result ClearKernelEvent(KernelEvent *event) noexcept;

/**
 * \short Pop the readable handle of a KernelEvent.
 *
 * \param[in] event A pointer to a KernelEvent.
 *
 * \pre ``event`` is initialized and contains a readable handle.
 * \post ``event`` doesn't contain a readable handle anymore.
 * \return The readable handle of the ``event``.
 */
svc::Handle PopReadableHandle(KernelEvent *event) noexcept;

/**
 * \short Pop the writable handle of a KernelEvent.
 *
 * \param[in] event A pointer to a KernelEvent.
 *
 * \pre ``event`` is initialized and contains a writable handle.
 * \post ``event`` doesn't contain a writable handle anymore.
 * \return The writable handle of the ``event``.
 */
svc::Handle PopWritableHandle(KernelEvent *event) noexcept;

/**
 * \short Destroy a KernelEvent.
 *
 * \param[in] event A pointer to a KernelEvent.
 *
 * \pre ``event`` is initialized.
 * \post ``event`` is uninitialized.
 */
void DestroyKernelEvent(KernelEvent *event) noexcept;

/**
 * @}
 */

}  // namespace hs::os
