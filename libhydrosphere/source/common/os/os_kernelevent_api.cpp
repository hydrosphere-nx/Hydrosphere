/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/diag.hpp>
#include <hs/os/os_kernel_event_api.hpp>

enum KernelEventState {
    KernelEventState_Uninitialized = 0,
    KernelEventState_Initialized = 1,
};

namespace hs::os {

hs::Result CreateKernelEvent(KernelEvent *event, bool is_auto_clear) noexcept {
    svc::Handle readable_handle;
    svc::Handle writable_handle;
    auto result = hs::svc::CreateEvent(&writable_handle, &readable_handle);
    if (result.Ok()) {
        event->state = KernelEventState_Initialized;

        event->readable_handle.SetValue(readable_handle);
        event->writable_handle.SetValue(writable_handle);

        event->is_auto_clear = is_auto_clear;
    }

    return result;
}

void LoadKernelEvent(KernelEvent *event,
                     util::Optional<svc::Handle> readable_handle,
                     util::Optional<svc::Handle> writable_handle) noexcept {
    event->state = KernelEventState_Initialized;
    event->readable_handle = readable_handle;
    event->writable_handle = writable_handle;
}

void WaitKernelEvent(KernelEvent *event) noexcept {
    int32_t index;

    // Wait for the event to be signaled
    while (hs::svc::WaitSynchronization(
               &index, event->readable_handle.GetValuePointer(), 1, -1)
               .Err()) {
    }

    if (event->is_auto_clear) {
        hs::Result result = hs::Result(0);
        do {
            result = hs::svc::ResetSignal(*event->readable_handle);
        } while ((result.GetValue() & 0x3FFFFF) == 0xFA01);

        __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);
    }
}

bool IsKernelEventSignaled(KernelEvent *event) noexcept {
    hs::Result result = hs::Result(0);

    if (event->is_auto_clear) {
        result = hs::svc::ResetSignal(*event->readable_handle);

        return result.Ok();
    } else {
        while (true) {
            int32_t index;

            result = hs::svc::WaitSynchronization(
                &index, event->readable_handle.GetValuePointer(), 1, 0);

            if (result.Ok()) {
                return true;
            }

            if ((result.GetValue() & 0x3FFFFF) == 0xEA01) {
                return false;
            }
        }
    }
    return false;
}

hs::Result SignalKernelEvent(KernelEvent *event) noexcept {
    return hs::svc::SignalEvent(*event->writable_handle);
}

inline hs::svc::Handle GetKernelEventHandle(KernelEvent *event) noexcept {
    if (!event->readable_handle) {
        return *event->writable_handle;
    }

    return *event->readable_handle;
}

hs::Result ClearKernelEvent(KernelEvent *event) noexcept {
    return hs::svc::ClearEvent(GetKernelEventHandle(event));
}

svc::Handle PopReadableHandle(KernelEvent *event) noexcept {
    auto handle = *event->readable_handle;
    event->readable_handle = util::Optional<svc::Handle>();

    return handle;
}

svc::Handle PopWritableHandle(KernelEvent *event) noexcept {
    auto handle = *event->writable_handle;
    event->writable_handle = util::Optional<svc::Handle>();

    return handle;
}

void DestroyKernelEvent(KernelEvent *event) noexcept {
    if (event->readable_handle) {
        hs::svc::CloseHandle(*event->readable_handle);
    }

    if (event->writable_handle) {
        hs::svc::CloseHandle(*event->writable_handle);
    }

    event->readable_handle = util::Optional<svc::Handle>();
    event->writable_handle = util::Optional<svc::Handle>();
    event->is_auto_clear = false;
    event->state = KernelEventState_Uninitialized;
}

}  // namespace hs::os
