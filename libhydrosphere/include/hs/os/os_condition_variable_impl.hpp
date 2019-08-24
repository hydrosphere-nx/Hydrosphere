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

#include <hs/hs_macro.hpp>
#include <hs/os/os_critical_section.hpp>
#include <hs/util/util_template_api.hpp>

namespace hs::os {
/**
 * \short Condition Variable implementation.
 * 
 * \remark A condition variable is an object able to block the calling thread until notified to resume.
 */
class ConditionVariableImpl {
 private:
    uint_fast32_t image;

 public:
    /**
     * \short Signal one.
     * 
     * Unblocks one of the threads currently waiting for this condition.
     */
    void Signal(void) noexcept;

    /**
     * \short Signal all (Broadcast).
     * 
     * Unblocks all threads currently waiting for this condition.
     */
    void Broadcast(void) noexcept;

    /**
     * \short Wait until signaled.
     * 
     * The execution of the current thread (which shall have entered the critial section) is blocked until signaled.
     */
    void Wait(CriticalSection *critical_section) noexcept;

    /**
     * \short Wait for timeout or until signaled.
     * 
     * The execution of the current thread (which shall have entered the critial section) is blocked during \c timeout, or until signaled (if the latter happens first).
     */
    bool WaitTimeout(CriticalSection *critical_section,
                     int64_t timeout) noexcept;
};

static_assert(hs::util::is_pod<ConditionVariableImpl>::value,
"ConditionVariableImpl isn't pod");

}  // namespace hs::os
