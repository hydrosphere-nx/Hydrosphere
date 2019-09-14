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

#include <hs/mem/memory_default_delete.hpp>
#include <hs/mem/unique_ptr.hpp>
#include <hs/util/util_template_api.hpp>
#include <hs/util/util_std_new.hpp>
#include <hs/util.hpp>

namespace hs::mem {
template <class Alloc, class T> struct memory_allocator_delete {
    Alloc &allocator;
    // Default constructor
    constexpr explicit memory_allocator_delete(Alloc &allocator) noexcept :
        allocator(allocator) {}

    template <class U> memory_allocator_delete(
        const memory_allocator_delete<Alloc, U>& d) noexcept :
            allocator(d.allocator) {}

    void operator()(T* ptr) const {
        ptr->~T();
        allocator.Free(ptr);
    }
};

template<class T, class Alloc, class... Args>
    unique_ptr<T, memory_allocator_delete<Alloc, T>> allocate_unique(Alloc& a,
    Args&&... args) {

    // First we allocate T.
    T *data = reinterpret_cast<T*>(a.Allocate(sizeof(T)));

    // Allocation might fail so assert in that case.
    __HS_ASSERT(data != nullptr);

    // Then we call the constructor using args sent by the user.
    new (data) T(hs::util::forward<Args>(args)...);

    // Finally construct the unique_ptr.
    return unique_ptr<T, memory_allocator_delete<Alloc, T>>(data,
        memory_allocator_delete<Alloc, T>(a));
}

}  // namespace hs::mem
