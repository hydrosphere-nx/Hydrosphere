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
    uint16_t flags;

    /**
     * \short Pack a A/B/W buffer to the IPC region.
     */
    constexpr uint32_t pack_buffer(uint32_t *buffer) noexcept {
        // Lower 32-bits of the size.
        buffer[0] = static_cast<uint32_t>(size);

        // Lower 32-bits of address.
        buffer[1] = static_cast<uint32_t>(address);

        // Packed
        // 1-0: Flags.
        // 4-2: Bit 38-36 of address.
        // 27-24: Bit 35-32 of size.
        // 31-28: Bit 35-32 of address.
        buffer[2] = (flags & 3)
                    | (((address >> 32) & 15) << 28)
                    | ((address >> 36) << 2);

        return 3;
    }

    /**
     * \short Pack a X buffer to the IPC region.
     */
    constexpr uint32_t pack_out_pointer(uint32_t *buffer) noexcept {
        // Packed
        // - 5-0: Bits 5-0 of counter.
        // - 8-6: Bit 38-36 of address.
        // - 11-9; Bits 11-9 of counter.
        // - 15-12: Bit 35-32 of address.
        // - 31-16: Size
        buffer[0] = flags | (size << 16)
                          | (((address >> 32) & 15) << 12)
                          | (((address >> 36) & 15) << 6);

        // Lower 32-bits of address.
        buffer[1] = static_cast<uint32_t>(address);

        return 2;
    }

    /**
     * \short Pack a C buffer to the IPC region.
     */
    constexpr uint32_t pack_in_pointer(uint32_t *buffer) noexcept {
        // Lower 32-bits of address.
        buffer[0] = static_cast<uint32_t>(address);

        // Packed
        // - 15-0: Rest of address.
        // - 31-16: Size
        buffer[1] = (address >> 32) | (size << 16);

        return 2;
    }

    /**
     * \short Pack a C buffer size to the IPC region.
     */
    constexpr uint32_t pack_in_pointer_u16_size(uint16_t *buffer) noexcept {
        buffer[0] = (size > 0xFFFF) ? 0 : size;
        return 1;
    }
};

static_assert(sizeof(Buffer) == 0x18, "invalid size for Buffer");

static_assert(hs::util::is_pod<Buffer>::value, "Buffer isn't pod");


}  // namespace hs::os::ipc
