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

enum class BufferType: uint8_t {
    Invalid = 0,
    A,
    B,
    W,
    X,
    C,
};

struct Buffer {
    uint64_t address;
    // NOTE: The max size of a buffer is on 36 bits.
    uint64_t size;
    BufferType buffer_type;
    uint8_t flags;
};

static_assert(sizeof(Buffer) == 0x18, "invalid size for Buffer");

static_assert(hs::util::is_pod<Buffer>::value, "Buffer isn't pod");


}  // namespace hs::os::ipc
