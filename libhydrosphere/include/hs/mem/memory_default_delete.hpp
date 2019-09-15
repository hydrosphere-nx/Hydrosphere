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

namespace hs::mem {
template <class T> struct default_delete {
    // Default constructor
    constexpr default_delete() noexcept = default;

    template <class U> default_delete(const default_delete<U>& d) noexcept;

    void operator()(T* ptr) const {
        delete ptr;
    }
};
}  // namespace hs::mem
