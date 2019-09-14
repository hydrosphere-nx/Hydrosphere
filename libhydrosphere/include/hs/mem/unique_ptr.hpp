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

#include <hs/mem/memory_api.hpp>

namespace hs::mem {
/**
 * \short A smart pointer that owns and manages another object through a pointer
 *        and disposes of that object when the unique_ptr goes out of scope.
 */
template <class T,
          class Deleter = hs::mem::default_delete<T>>
class unique_ptr {
 public:
    typedef T* pointer;
    typedef T element_type;
    typedef Deleter deleter_type;

    /**
     * \short Constructs a new unique_ptr from a nullptr.
     */
    unique_ptr() noexcept : native_ptr_(nullptr), deleter_() {}

    /**
     * \short Constructs a new unique_ptr given a raw pointer.
     * \param[in] ptr The pointer to be wrapped.
     */
    explicit unique_ptr(T *ptr) noexcept : native_ptr_(ptr), deleter_() {}

    /**
     * \short Constructs a new unique_ptr given a raw pointer.
     * \param[in] ptr The pointer to be wrapped.
     * \param[in] d The destructor to use when disposing of the pointer.
     */
    explicit unique_ptr(T *ptr, Deleter d) noexcept : native_ptr_(ptr),
        deleter_(d) {}

    /**
     * \short Constructs a new unique_ptr from an existing unique_ptr.
     * \param[in] ptr The unique_ptr to use.
     */
    unique_ptr(const unique_ptr &ptr) noexcept : native_ptr_(ptr.native_ptr_),
        deleter_(ptr.deleter_) {
        const_cast<unique_ptr &>(ptr).native_ptr_ =
            nullptr;  // const_cast to force ownership transfer.
    }

    /**
     * \short Destructs the managed object if such is present.
     */
    inline ~unique_ptr() noexcept { destroy(); }

    /**
     * \short Assigns the unique_ptr.
     * \param[in] ptr The unique_ptr that should be assigned.
     */
    unique_ptr &operator=(unique_ptr ptr) noexcept {
        swap(ptr);
        return *this;
    }

    /**
     * \short Destroys the managed object.
     */
    inline void reset() noexcept { destroy(); }

    /**
     * \short Replaces the managed object.
     * \param[in] ptr The pointer to replace the contents with.
     */
    void reset(pointer ptr) noexcept {
        static_assert((nullptr == ptr) || (native_ptr_ != ptr), "");
        destroy();
        native_ptr_ = ptr;
    }

    /**
     * Swaps the managed objects.
     * \param[in] ptr The pointer to exchange the contents with.
     */
    void swap(unique_ptr &ptr) noexcept {
        auto temp = native_ptr_;
        native_ptr_ = ptr.native_ptr_;
        ptr.native_ptr_ = temp;
    }

    /**
     * \short Returns a pointer to the managed object and releases the
     *        ownership.
     */
    inline pointer release() noexcept {
        pointer temp = native_ptr_;
        native_ptr_ = nullptr;
        return temp;
    }

    /**
     * \short Checks if there is an associated managed object.
     */
    inline explicit operator bool() const noexcept {
        return nullptr != native_ptr_;
    }

    // Underlying native_ptr_ operations:

    /**
     * \short Dereferences pointer to the managed object.
     */
    inline T &operator*() const noexcept {
        static_assert(nullptr != native_ptr_, "");
        return *native_ptr_;
    }

    /**
     * \short Provides access to a pointer to managed object.
     */
    inline T *operator->() const noexcept {
        static_assert(nullptr != native_ptr_, "");
        return native_ptr_;
    }

    /**
     * \short Returns a pointer to the managed object.
     */
    inline pointer get() const noexcept { return native_ptr_; }

 private:
    pointer native_ptr_;
    deleter_type deleter_;

    inline void destroy() noexcept {
        deleter_(native_ptr_);

        native_ptr_ = nullptr;
    }

    inline void release() const noexcept { native_ptr_ = nullptr; }
};

// Comparison operators.

template <class T, class U>
inline bool operator==(const unique_ptr<T> &l,
                       const unique_ptr<U> &r) noexcept {
    return l.get() == r.get();
}

template <class T, class U>
inline bool operator!=(const unique_ptr<T> &l,
                       const unique_ptr<U> &r) noexcept {
    return l.get() != r.get();
}

template <class T, class U>
inline bool operator<=(const unique_ptr<T> &l,
                       const unique_ptr<U> &r) noexcept {
    return l.get() <= r.get();
}

template <class T, class U>
inline bool operator<(const unique_ptr<T> &l, const unique_ptr<U> &r) noexcept {
    return l.get() < r.get();
}

template <class T, class U>
inline bool operator>=(const unique_ptr<T> &l,
                       const unique_ptr<U> &r) noexcept {
    return l.get() >= r.get();
}

template <class T, class U>
inline bool operator>(const unique_ptr<T> &l, const unique_ptr<U> &r) noexcept {
    return l.get() > r.get();
}
}  // namespace hs::mem
