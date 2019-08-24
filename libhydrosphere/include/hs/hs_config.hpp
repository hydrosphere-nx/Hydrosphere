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
#ifdef __aarch64__
#define HYDROSPHERE_TARGET_ARCH_NAME aarch64
#define HYDROSPHERE_TARGET_AARCH64 1
#elif __arm__
#define HYDROSPHERE_TARGET_ARCH_NAME aarch32
#define HYDROSPHERE_TARGET_AARCH32 1
#else
#error "Cannot determine the target architecture!"
#endif

#define HYDROSPHERE_DEBUG_DIAG 1
