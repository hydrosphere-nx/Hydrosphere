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
#include <hs/os/ipc/ipc_buffer.hpp>
#include <hs/util/util_array.hpp>
#include <hs/util/util_optional.hpp>
#include <hs/svc/svc_types.hpp>

namespace hs::os::ipc {

// Dummy zero-sized struct
struct EmptyRaw {
   int dummy[0];
};

template <uint8_t BufferCount = 8,
          uint8_t CopyHandleCount = 8,
          uint8_t MoveHandleCount = 8,
          typename T = EmptyRaw>
class Message {
 public:
    Message() {}
 private:
    hs::util::Array<Buffer, BufferCount> buffers;
    hs::util::Array<hs::svc::Handle, CopyHandleCount> copy_handles;
    hs::util::Array<hs::svc::Handle, MoveHandleCount> move_handles;
    uint8_t buffers_index;
    uint8_t copy_handles_index;
    uint8_t move_handles_index;
    bool is_request;
    hs::util::Optional<uint32_t> token;
    hs::util::Optional<T> raw;
};

static_assert(sizeof(EmptyRaw) == 0, "EmptyRaw isn't a zero-sized type!");
}  // namespace hs::os::ipc
