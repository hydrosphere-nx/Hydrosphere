/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/hs_config.hpp>
#include <hs/os/os_api.hpp>
#include <hs/os/os_tls.hpp>

namespace hs::os {
__HS_ATTRIBUTE_NAKED ThreadLocalStorage*
ThreadLocalStorage::GetThreadLocalStorage() noexcept {
#ifdef HYDROSPHERE_TARGET_AARCH64
    __HS_ASM("mrs x0, tpidrro_el0");
    __HS_ASM("ret");
#elif HYDROSPHERE_TARGET_AARCH32
    __HS_ASM("mrc p15, 0, r0, c13, c0, 3");
    __HS_ASM("bx lr");
#else
#error "TLS not implemented for this architecture"
#endif
}

svc::Handle GetCurrentThreadHandle() noexcept {
    return ThreadLocalStorage::GetThreadLocalStorage()
        ->GetThreadContext()
        ->thread_handle;
}
}  // namespace hs::os
