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
#include <hs/os/os_api.hpp>
#include <hs/os/os_condition_variable_impl.hpp>
#include <hs/os/os_critical_section.hpp>
#include <hs/svc.hpp>

namespace hs::os {

void ConditionVariableImpl::Signal(void) noexcept {
    hs::svc::SignalProcessWideKey((uintptr_t) & this->image, 1);
}

void ConditionVariableImpl::Broadcast(void) noexcept {
    hs::svc::SignalProcessWideKey((uintptr_t) & this->image, -1);
}

void ConditionVariableImpl::Wait(CriticalSection *critical_section) noexcept {
    auto result = hs::svc::WaitProcessWideKeyAtomic(
        (uintptr_t)critical_section, (uintptr_t) & this->image,
        hs::os::GetCurrentThreadHandle(), -1);
    __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);
}

bool ConditionVariableImpl::WaitTimeout(CriticalSection *critical_section,
                                        int64_t timeout) noexcept {
    auto result = hs::svc::WaitProcessWideKeyAtomic(
        (uintptr_t)critical_section, (uintptr_t) & this->image,
        hs::os::GetCurrentThreadHandle(), timeout);

    if (result.Err()) {
        if ((result.GetValue() & 0x3FFFFF) == 0xEA01) {
            critical_section->Enter();
            return false;
        }

        __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);
    }
    return true;
}
}  // namespace hs::os
