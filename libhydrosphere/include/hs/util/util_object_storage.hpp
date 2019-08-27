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

namespace hs::util {
/**
 * \short Obtains a POD type suitable to use as storage for an object of a size
 * of at most Len bytes, aligned as specified by Align.
 *
 * The obtained type is aliased as member type aligned_storage::type.
 *
 * \tparam Len The size of the storage object in bytes.
 * \tparam Align The alignment requested in bytes.
 *
 * \pre Len shall not be zero.
 *
 * \remark This is identical to libcxx's aligned_storage.
 */
template <size_t Len, size_t Align = sizeof(size_t)>
struct aligned_storage {
    /**
     * \short A POD type suitable to store Len bytes, aligned as specified by
     * Align.
     */
    struct type {
        alignas(Align) unsigned char data[Len];
    };
};

/**
 * \short Obtains a POD type suitable to use as storage for an object.
 * \tparam T The object type that needs to be hold.
 * \tparam Align The alignment requested in bytes.
 */
template <typename T, size_t Align = sizeof(size_t)>
struct ObjectStorage {
    /**
     * \short A POD type suitable to store a T object.
     */
    typename aligned_storage<sizeof(T), Align>::type storage;

    /**
     * \short Cast the underlying storage to a T reference.
     * \return A T reference to the underlying storage.
     */
    inline T &Get() noexcept { return reinterpret_cast<T &>(storage); }

    /**
     * \short Cast the underlying storage to a const T reference.
     * \return A const T reference to the underlying storage.
     */
    inline const T &Get() const noexcept {
        return reinterpret_cast<const T &>(storage);
    }

    /**
     * \short Cast the underlying storage to a T pointer.
     * \return A T pointer to the underlying storage.
     */
    inline T *GetPointer() noexcept { return reinterpret_cast<T *>(&storage); }

    /**
     * \short Cast the underlying storage to a const T pointer.
     * \return A const T pointer to the underlying storage.
     */
    inline const T *GetPointer() const noexcept {
        return reinterpret_cast<const T *>(&storage);
    }

    inline T &operator*() noexcept { return Get(); }

    inline T *operator->() noexcept { return GetPointer(); }
};
}  // namespace hs::util
