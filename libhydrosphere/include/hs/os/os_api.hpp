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

#include <hs/svc/svc_types.hpp>

namespace hs::os {

/**
 * \name Handle API
 * \addtogroup os_api
 * @{
 */

/**
 * \short Get the current thread handle.
 * \return The current thread handle.
 */
svc::Handle GetCurrentThreadHandle() noexcept;


/**
 * \short Get the pseudo thread handle.
 * \return The pseudo thread handle.
 */
inline svc::Handle GetThreadPseudoHandle() noexcept {
    return svc::Handle::FromRawValue(0xFFFF8000);
}

/**
 * \short Get the pseudo process handle.
 * \return The pseudo process handle.
 */
inline svc::Handle GetProcessPseudoHandle() noexcept {
    return svc::Handle::FromRawValue(0xFFFF8001);
}

/**
 * @}
 */

}  // namespace hs::os
