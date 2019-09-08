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

#include <stddef.h>
#include <hs/hs_config.hpp>
#include <hs/util/util_template_api.hpp>
#include <hs/svc/svc_types.hpp>

namespace hs::os::ipc {

enum BufferType {
    BufferType_Invalid = 0,
    BufferType_A,
    BufferType_B,
    BufferType_W,
    BufferType_X,
    BufferType_C,
};

struct Buffer {
    uintptr_t address;
    // The max size of a buffer is on 36 bits and we know that the retail switch only have 4GB of RAM.
    // Therefore, it's unlikely that the user would want to tranfer a buffer of more than 4GB via IPC.
    uint32_t size;
    uint8_t buffer_type;
    uint8_t flags;
};

#ifdef HYDROSPHERE_TARGET_AARCH64
static_assert(sizeof(Buffer) == 0x10, "invalid size for Buffer");
#elif HYDROSPHERE_TARGET_AARCH32
static_assert(sizeof(Buffer) == 0x0C, "invalid size for Buffer");
#endif

static_assert(hs::util::is_pod<Buffer>::value, "Buffer isn't pod");


}  // namespace hs::os::ipc
