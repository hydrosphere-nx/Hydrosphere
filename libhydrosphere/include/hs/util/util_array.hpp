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

#include <hs/diag/diag_macro.hpp>

namespace hs::util {

template <typename T, size_t N = 1>
class Array {
 public:
    typedef T value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    // util to avoid redefining the current template type.
    typedef Array<T, N> this_type;

    // intentionally public for aggregate initialization.
    value_type values[N ? N : 1];

    void fill(const value_type& value) noexcept {
        for (size_t i = 0; i < this->size(); i++) {
            values[i] = value;
        }
    }

    void swap(this_type& x) noexcept {
        for (size_t i = 0; i < this->size(); i++) {
            value_type tmp = this->values[i];

            this->values[i] = x.values[i];

            x.values[i] = tmp;
        }
    }

    constexpr bool empty() const {
        return N == 0;
    }

    constexpr size_type size() const {
        return (size_type)N;
    }

    constexpr size_type max_size() const {
        return (size_type)N;
    }

    constexpr reference operator[](size_type n) {
        __HS_DEBUG_ASSERT(n < N);
        return this->values[n];
    }

    constexpr const_reference operator[](size_type n) const {
        __HS_DEBUG_ASSERT(n < N);
        return this->values[n];
    }

    constexpr value_type* data() noexcept {
        return this->values;
    }

    constexpr const value_type* data() const noexcept {
        return this->values;
    }

    constexpr reference at(size_type n) noexcept {
        __HS_DEBUG_ASSERT(n < N);
        return this->values[n];
    }

    constexpr const_reference at(size_type n) const noexcept {
        __HS_DEBUG_ASSERT(n < N);
        return this->values[n];
    }

    constexpr reference front() noexcept {
        __HS_DEBUG_ASSERT(!this->empty());
        return this->values[0];
    }

    constexpr const_reference front() const noexcept {
        __HS_DEBUG_ASSERT(!this->empty());
        return this->values[0];
    }

    constexpr reference back() noexcept {
        __HS_DEBUG_ASSERT(!this->empty());
        return this->values[N - 1];
    }

    constexpr const_reference back() const noexcept {
        __HS_DEBUG_ASSERT(!this->empty());
        return this->values[N - 1];
    }

    constexpr iterator begin() noexcept {
        __HS_DEBUG_ASSERT(!this->empty());
        return &this->values[0];
    }

    constexpr const_iterator begin() const noexcept {
        __HS_DEBUG_ASSERT(!this->empty());
        return &this->values[0];
    }

    constexpr const_iterator cbegin() const noexcept {
        __HS_DEBUG_ASSERT(!this->empty());
        return &this->values[0];
    }

    constexpr iterator end() noexcept {
        __HS_DEBUG_ASSERT(!this->empty());
        return &this->values[N - 1];
    }

    constexpr const_iterator end() const noexcept {
        __HS_DEBUG_ASSERT(!this->empty());
        return &this->values[N - 1];
    }

    constexpr const_iterator cend() const noexcept {
        __HS_DEBUG_ASSERT(!this->empty());
        return &this->values[N - 1];
    }
};
}  // namespace hs::util
