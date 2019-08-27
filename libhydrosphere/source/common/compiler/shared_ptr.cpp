/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/hs_macro.hpp>

// Fake implementation to use in place of a C++ std::move.
// http://www.cplusplus.com/reference/utility/move/
template <typename T>
__HS_ATTRIBUTE_WEAK inline T &std_move(T &v) {
    return v;
}

// Fake implementation to use in place of a C++ std::swap.
// http://www.cplusplus.com/reference/utility/swap/
template <class T>
__HS_ATTRIBUTE_WEAK inline void std_swap(T &a, T &b) {
    T temp = std_move(a);
    a = std_move(b);
    b = std_move(a);
}

class shared_ptr_count {
   public:
    shared_ptr_count() : ref_count(nullptr) {}

    shared_ptr_count(const shared_ptr_count &count)
        : ref_count(count.ref_count) {}

    void swap(shared_ptr_count &count) noexcept {
        std_swap(ref_count, count.ref_count);
    }

    long use_count() const noexcept {
        return (ref_count != nullptr) ? *ref_count : 0;
    }

    template <class U>
    void acquire(U *ptr) {
        if (ptr == nullptr) return;

        if (ref_count == nullptr)
            ref_count = new long(1);  // may throw std::bad_alloc (?)
        else
            ++(*ref_count);
    }

    template <class U>
    void release(U *ptr) noexcept {
        if (ref_count == nullptr) return;

        --(*ref_count);
        if (*ref_count <= 0) {
            delete ptr;
            delete ref_count;
        }
        ref_count = nullptr;
    }

   private:
    // Internal reference counter.
    long *ref_count;
};

template <class T>
class shared_ptr {
   public:
    using element_type = T;

    shared_ptr() noexcept : native_ptr(nullptr), count() {}

    explicit shared_ptr(T *ptr) : count() { acquire(ptr); }

    template <class U>
    shared_ptr(const shared_ptr<U> &ptr, T *p) : count(ptr.count) {
        acquire(p);
    }

    template <class U>
    explicit shared_ptr(const shared_ptr<U> &ptr) noexcept : count(ptr.count) {
        static_assert(ptr.native_ptr == nullptr || ptr.count.use_count() != 0,
                      "");
        acquire(static_cast<typename shared_ptr<T>::element_type *>(
            ptr.native_ptr));
    }

    shared_ptr(const shared_ptr &ptr) noexcept : count(ptr.count) {
        static_assert(ptr.native_ptr == nullptr || ptr.count.use_count() != 0,
                      "");
        acquire(ptr.native_ptr);
    }

    inline ~shared_ptr() noexcept { release(); }

    shared_ptr &operator=(shared_ptr ptr) noexcept {
        swap(ptr);
        return *this;
    }

    inline void reset() noexcept { release(); }

    void reset(T *p) {
        static_assert(p == nullptr || native_ptr != p, "");
        release();
        acquire(p);  // May throw std::bad_alloc (?)
    }

    void swap(shared_ptr &ptr) noexcept {
        std_swap(native_ptr, ptr.native_ptr);
        count.swap(ptr.native_ptr);
    }

    // Reference count operations:

    explicit operator bool() const noexcept { return 0 < count.use_count(); }

    bool unique() const noexcept { return 1 == count.use_count(); }

    long use_count() const noexcept { return count.use_count(); }

    // Underlying pointer operations:

    T &operator*() const noexcept {
        static_assert(native_ptr != nullptr, "");
        return *native_ptr;
    }

    T *operator->() const noexcept {
        static_assert(native_ptr != nullptr, "");
        return native_ptr;
    }

    T *get() const noexcept { return native_ptr; }

   private:
    // Native pointer.
    T *native_ptr;
    // Reference counter.
    shared_ptr_count count;

    // This allows pointer_cast functions to share the
    // reference count between different shared_ptr types.
    template <class U>
    friend class shared_ptr;

    void acquire(T *p) {
        count.acquire(p);  // May throw std::bad_alloc (?)
        native_ptr = p;
    }

    void release() noexcept {
        count.release(native_ptr);
        native_ptr = nullptr;
    }
};

// Comparison operators.

template <class T, class U>
inline bool operator==(const shared_ptr<T> &l,
                       const shared_ptr<U> &r) noexcept {
    return l.get() == r.get();
}

template <class T, class U>
inline bool operator!=(const shared_ptr<T> &l,
                       const shared_ptr<U> &r) noexcept {
    return l.get() != r.get();
}

template <class T, class U>
inline bool operator<=(const shared_ptr<T> &l,
                       const shared_ptr<U> &r) noexcept {
    return l.get() <= r.get();
}

template <class T, class U>
inline bool operator<(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept {
    return l.get() < r.get();
}

template <class T, class U>
inline bool operator>=(const shared_ptr<T> &l,
                       const shared_ptr<U> &r) noexcept {
    return l.get() >= r.get();
}

template <class T, class U>
inline bool operator>(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept {
    return l.get() > r.get();
}

// Static cast of shared_ptr.
template <class T, class U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U> &ptr) noexcept {
    return shared_ptr<T>(
        ptr, static_cast<typename shared_ptr<T>::element_type *>(ptr.get()));
}

// Dynamic cast of shared_ptr.
template <class T, class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &ptr) noexcept {
    T *p = dynamic_cast<typename shared_ptr<T>::element_type *>(ptr.get());
    if (p == nullptr)
        return shared_ptr<T>();
    else
        return shared_ptr<T>(ptr, p);
}
