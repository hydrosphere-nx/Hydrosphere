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

#include <stdint.h>

namespace hs {
/**
 * \short Opaque type representing a result value.
 */
class Result {
    /**
     * \short The raw result code.
     */
    uint32_t value;

   public:
    /**
     * \short Construct a Result from a raw value.
     * \param[in] value The raw result value that defines the new Result.
     */
    explicit Result(uint32_t value) noexcept : value(value) {}

    /**
     * \short Check if the raw result code value match the Result::Success value
     */
    inline bool Ok() const noexcept { return value == Success; }

    /**
     * \short Check if the raw result code value doesn't match the
     * Result::Success value
     */
    inline bool Err() const noexcept { return !Ok(); }

    /**
     * \short Get a the raw result code value.
     */
    inline uint32_t GetValue() const noexcept { return value; }

    /**
     * \short A value representing a successful operation.
     */
    static const uint32_t Success = 0;
};
}  // namespace hs
