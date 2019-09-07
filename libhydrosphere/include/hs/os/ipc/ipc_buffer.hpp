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
#include <hs/svc/svc_types.hpp>

namespace hs::os::ipc {

enum class BufferType {
    A,
    B,
    W,
    X,
    C,
};

class Buffer {
 public:
    Buffer(BufferType type, uintptr_t address, size_t size, uint8_t flags):
        type(type), address(address), size(size), flags(flags) {}

 private:
    uintptr_t address;
    size_t size;
    BufferType type;
    uint8_t flags;
};

}  // namespace hs::os::ipc
