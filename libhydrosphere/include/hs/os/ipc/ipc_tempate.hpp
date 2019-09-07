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

template <size_t BufferCount = 8,
          size_t CopyHandleCount = 8,
          size_t MoveHandleCount = 8>
class Message {
 private:
    hs::svc::Handle copy_handles[CopyHandleCount];
    hs::svc::Handle move_handles[MoveHandleCount];
};
}  // namespace hs::os::ipc
