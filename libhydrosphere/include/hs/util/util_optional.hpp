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

#include <hs/util/util_object_storage.hpp>

namespace hs::util {

/**
 * \short Obtains a POD type that manages an optional contained value.
 * \tparam T The object type that needs to be hold.
 */
template <typename T>
class Optional {
   private:
    /**
     * \short True if the Optional contains a value.
     */
    bool has_value;
    /**
     * \short Reserved for future usage.
     */
    char reserved[3];

    /**
     * \short POD storage of the value.
     */
    ObjectStorage<T, alignof(T)> value;

   public:
    constexpr explicit operator bool() const noexcept { return HasValue(); }

    /**
     * \short Check whether the Optional contains a value.
     */
    constexpr bool HasValue() const noexcept { return has_value; }

    /**
     * \short Store the given value in the Optional.
     *
     * \param[in] new_value the new value to move inside the Optional.
     */
    constexpr void SetValue(T new_value) noexcept {
        *value = new_value;
        has_value = true;
    }

    /**
     * \short Get a reference to the value contained in the Optional.
     *
     * \remark This value isn't considered valid if HasValue returns false.
     */
    constexpr T &GetValue() noexcept { return value.Get(); }

    /**
     * \short Get a pointer to the value contained in the Optional.
     *
     * \remark This value isn't considered valid if HasValue returns false.
     */
    constexpr T *GetValuePointer() noexcept { return value.GetPointer(); }

    inline T &operator*() noexcept { return GetValue(); }
    inline T *operator->() noexcept { return GetValuePointer(); }
};

}  // namespace hs::util
