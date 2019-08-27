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

namespace hs::util {
/**
 * \short integral_constant wraps a static constant of specified type.
 * \remark This is a simple implementation of std::integral_constant.
 * \remark It is the base class for the C++ type traits.
 */
template <class T, T v>
struct integral_constant {
    static constexpr T value = v;
    typedef T value_type;
    // using injected-class-name
    typedef integral_constant type;
    constexpr operator value_type() const noexcept { return value; }
    // since c++14
    constexpr value_type operator()() const noexcept { return value; }
};

/**
 * \short If T is a plain old data type, that is, both trivial and
 * standard-layout, provides the member constant value equal true. For any other
 * type, value is false. \remark This is a simple implementation of std::is_pod
 * using __is_pod from clang builtins.
 */
template <class T>
struct is_pod : public integral_constant<bool, __is_pod(T)> {};

}  // namespace hs::util
