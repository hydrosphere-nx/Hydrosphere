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

#include <stdatomic.h>
#include <stdint.h>

#include <hs/hs_macro.hpp>
#include <hs/util/util_template_api.hpp>

namespace hs::os {
/**
 * \short Critical Section implementation.
 * 
 * \remark CriticalSection ensures mutual exclusion of access.
 */
class CriticalSection {
 private:
    volatile _Atomic(uint_fast32_t) image;

 public:
    /**
     * \short Enter the critical section.
     */
    void Enter() noexcept;

    /**
     * \short Try to enter the critical section.
     */
    bool TryEnter() noexcept;

    /**
     * \short Leave the critical section.
     */
    void Leave() noexcept;

   /**
    * \short Returns true if the current thread owns the lock of the critical section.
    */
    bool IsLockedByCurrentThread() noexcept;
};

static_assert(hs::util::is_pod<CriticalSection>::value,
"CriticalSection isn't pod");

}  // namespace hs::os
