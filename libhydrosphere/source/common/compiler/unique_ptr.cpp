/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

// Fake implementation to use in place of a C++ std::move.
// http://www.cplusplus.com/reference/utility/move/
template <typename T>
inline T &std_move(T &v) {
    return v;
}

// Fake implementation to use in place of a C++ std::swap.
// http://www.cplusplus.com/reference/utility/swap/
template <class T>
inline void std_swap(T &a, T &b) {
    T temp = std_move(a);
    a = std_move(b);
    b = std_move(a);
}

template <class T>
class unique_ptr {
   public:
    unique_ptr() : native_ptr(nullptr) {}

    explicit unique_ptr(T *ptr) : native_ptr(ptr) {}

    unique_ptr(const unique_ptr &ptr) : native_ptr(ptr.native_ptr) {
        const_cast<unique_ptr &>(ptr).native_ptr =
            nullptr;  // const_cast to force ownership transfer.
    }

    unique_ptr &operator=(unique_ptr ptr) {
        swap(ptr);
        return *this;
    }

    inline ~unique_ptr() { destroy(); }

    inline void reset() { destroy(); }

    void reset(T *ptr) {
        static_assert((nullptr == ptr) || (native_ptr != ptr), "");
        destroy();
        native_ptr = ptr;
    }

    void swap(unique_ptr &ptr) { std_swap(native_ptr, ptr.native_ptr); }

    inline void release() { native_ptr = nullptr; }

    // Reference count operations:

    inline explicit operator bool() const { return nullptr != native_ptr; }

    // Underlying native_ptr operations:

    inline T &operator*() const {
        static_assert(nullptr != native_ptr, "");
        return *native_ptr;
    }

    inline T *operator->() const {
        static_assert(nullptr != native_ptr, "");
        return native_ptr;
    }

    inline T *get() const { return native_ptr; }

   private:
    // Underlying native pointer to manage.
    T *native_ptr;

    inline void destroy() {
        delete native_ptr;
        native_ptr = nullptr;
    }

    inline void release() const { native_ptr = nullptr; }
};

// Comparison operators.

template <class T, class U>
inline bool operator==(const unique_ptr<T> &l, const unique_ptr<U> &r) {
    return l.get() == r.get();
}

template <class T, class U>
inline bool operator!=(const unique_ptr<T> &l, const unique_ptr<U> &r) {
    return l.get() != r.get();
}

template <class T, class U>
inline bool operator<=(const unique_ptr<T> &l, const unique_ptr<U> &r) {
    return l.get() <= r.get();
}

template <class T, class U>
inline bool operator<(const unique_ptr<T> &l, const unique_ptr<U> &r) {
    return l.get() < r.get();
}

template <class T, class U>
inline bool operator>=(const unique_ptr<T> &l, const unique_ptr<U> &r) {
    return l.get() >= r.get();
}

template <class T, class U>
inline bool operator>(const unique_ptr<T> &l, const unique_ptr<U> &r) {
    return l.get() > r.get();
}
