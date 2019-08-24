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
#include <hs/os/os_critical_section.hpp>
#include <hs/svc.hpp>

#define HAS_LISTENERS 0x40000000

namespace hs::os {
void CriticalSection::Enter() noexcept {
    auto self_thread_handle = hs::os::GetCurrentThreadHandle();

    while (true) {
        uint32_t expected_value = 0;

        // If there is no contention, we won, return.
        if (atomic_compare_exchange_strong(&this->image, &expected_value,
                                           self_thread_handle.GetValue())) {
            return;
        }

        // If we own the lock or it was previously not owned, we won, return.
        if ((expected_value & ~HAS_LISTENERS) ==
            self_thread_handle.GetValue()) {
            return;
        }

        if (expected_value & HAS_LISTENERS) {
            hs::svc::ArbitrateLock(
                hs::svc::Handle::FromRawValue(expected_value & ~HAS_LISTENERS),
                (uintptr_t) & this->image, self_thread_handle);
        } else {
            // the value changed, we lost the race :(
            if (!atomic_compare_exchange_strong(
                    &this->image, &expected_value,
                    expected_value | HAS_LISTENERS)) {
                continue;
            } else {
                hs::svc::ArbitrateLock(
                            hs::svc::Handle::FromRawValue(expected_value),
                            reinterpret_cast<uintptr_t>(&this->image),
                            self_thread_handle);
            }
        }
    }
}

bool CriticalSection::TryEnter() noexcept {
    auto self_thread_handle = hs::os::GetCurrentThreadHandle();

    uint32_t expected_value = 0;
    // If there is no contention, we won, return.
    if (atomic_compare_exchange_strong(&this->image, &expected_value,
                                       self_thread_handle.GetValue())) {
        return true;
    }

    // If we already own the lock, we won, return.
    if ((expected_value & ~HAS_LISTENERS) == self_thread_handle.GetValue()) {
        return true;
    }

    return false;
}

void CriticalSection::Leave() noexcept {
    auto self_thread_handle = hs::os::GetCurrentThreadHandle();
    uint32_t expected_value = self_thread_handle.GetValue();

    if (!atomic_compare_exchange_strong(&this->image, &expected_value, 0)) {
        if (expected_value & HAS_LISTENERS) {
            hs::svc::ArbitrateUnlock((uintptr_t) & this->image);
        }
    }
}

bool CriticalSection::IsLockedByCurrentThread() noexcept {
    return (this->image & ~HAS_LISTENERS) ==
           hs::os::GetCurrentThreadHandle().GetValue();
}
}  // namespace hs::os
