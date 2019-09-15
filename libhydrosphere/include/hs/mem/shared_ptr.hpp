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

#include <hs/diag/diag_macro.hpp>
#include <hs/util/util_api.hpp>

namespace hs::mem {
template <class DeleterRefCount>
class shared_ptr_count {
 public:
    shared_ptr_count(long *ref_count, DeleterRefCount &d) :
        ref_count_(ref_count), deleter_ref_count_(d) {
        if (ref_count_ != nullptr) {
            *ref_count_ = 0;
        }
    }

    shared_ptr_count(const shared_ptr_count<DeleterRefCount> &count) = default;

    void swap(shared_ptr_count<DeleterRefCount> &count) noexcept {
        auto temp = ref_count_;
        ref_count_ = count.ref_count_;
        count.ref_count_ = temp;

        auto temp_deleter = deleter_ref_count_;
        deleter_ref_count_ = count.deleter_ref_count_;
        count.deleter_ref_count_ = deleter_ref_count_;
    }

    long use_count() const noexcept {
        return (ref_count_ != nullptr) ? *ref_count_ : 0;
    }

    template <class U>
    void acquire(U *ptr) {
        if (ptr == nullptr) return;

        // If the ref_count isn't allocated, we are always sure that we don't
        // have some custom allocator so we use the standard new.
        if (ref_count_ == nullptr)
            ref_count_ = new long(1);  // may throw std::bad_alloc (?)
        else
            ++(*ref_count_);
    }

    template <class D, class U>
    void release(D &deleter, U *ptr) noexcept {
        if (ref_count_ == nullptr) return;

        --(*ref_count_);
        if (*ref_count_ <= 0) {
            deleter(ptr);
            deleter_ref_count_(ref_count_);
        }
        ref_count_ = nullptr;
    }

 private:
    // Internal reference counter.
    long *ref_count_;
    DeleterRefCount &deleter_ref_count_;
};

/**
 * \short A smart pointer that retains shared ownership of an object through a
 * pointer.
 */
template <class T,
          class Deleter = hs::mem::default_delete<T>,
          class DeleterRefCount = hs::mem::default_delete<long>>
class shared_ptr {
 public:
    typedef T* pointer;
    typedef T element_type;
    typedef Deleter deleter_type;

    /**
     * \short Constructs a new shared_ptr from a nullptr.
     */
    shared_ptr() noexcept : native_ptr_(nullptr), deleter_(), count_(nullptr) {}

    /**
     * \short Constructs a new shared_ptr given a raw pointer.
     * \param[in] ptr The raw pointer to manage.
     */
    explicit shared_ptr(pointer ptr) : deleter_(), count_(nullptr) {
        acquire(ptr);
    }

    /**
     * \short Constructs a new shared_ptr given a raw pointer and a destructor.
     * \param[in] ptr The raw pointer to manage.
     * \param[in] d The destructor to use when disposing of the pointer.
     */
    explicit shared_ptr(pointer ptr, Deleter d)
        : deleter_(d), count_(nullptr) {
            acquire(ptr);
    }

    /**
     * \short Constructs a new shared_ptr given a raw pointer, a destructor and an allocator.
     * \param[in] ptr The raw pointer to manage.
     * \param[in] d The destructor to use when disposing of the pointer.
     * \param[in] d2 The destructor to use when disposing of the refcount.
     * \param[in] alloc The allocator to use when allocating the refcount.
     */
    template <class Alloc>
    shared_ptr(pointer ptr, Deleter d, DeleterRefCount d2, Alloc &alloc)
        : deleter_(d),
        count_(reinterpret_cast<long *>(alloc.Allocate(sizeof(long))), d2) {
            acquire(ptr);
        }

    /**
     * \short Constructs a new shared_ptr to share ownership.
     * \param[in] ptr A shared_ptr to take the reference count from.
     * \param[in] p The raw pointer to acquire ownership of.
     * \warning This should only be used for pointer casts as it doesn't manage
     *          two separate <T> and <U> pointers!
     */
    template <class U>
    shared_ptr(const shared_ptr<U> &ptr, pointer p)
        : deleter_(ptr.deleter_), count_(ptr.count_) {
        acquire(p);
    }

    /**
     * \short Constructs a new shared_ptr that converts from another pointer
     *        type.
     * \param[in] ptr The shared_ptr to use.
     */
    template <class U>
    explicit shared_ptr(const shared_ptr<U> &ptr) noexcept
        : deleter_(ptr.deleter_), count_(ptr.count_) {
        acquire(static_cast<typename shared_ptr<T>::element_type *>(
            ptr.native_ptr_));
    }

    /**
     * \short Constructs a new sàhared_ptr by the copy-and-swap idiom.
     * \param[in] ptr The shared_ptr to use.
     */
    shared_ptr(const shared_ptr &ptr) noexcept
        : deleter_(ptr.deleter_), count_(ptr.count_) {
        acquire(ptr.native_ptr_);
    }

    /**
     * \short Destructs the owned object if no more shared_ptrs link to it.
     */
    inline ~shared_ptr() noexcept { release(); }

    /**
     * \short Assigns the shared_ptr.
     * \param[in] ptr The shared_ptr that should be assigned.
     */
    shared_ptr &operator=(shared_ptr ptr) noexcept {
        swap(ptr);
        return *this;
    }

    /**
     * \short Releases ownership of the managed object.
     */
    inline void reset() noexcept { release(); }

    /**
     * \short Replaces the managed object.
     * \param[in] p The pointer to replace the contents with.
     */
    void reset(pointer p) {
        release();
        acquire(p);  // May throw std::bad_alloc (?)
    }

    /**
     * \short Swaps the managed objects.
     * \param[in] ptr The pointer to exchange the contents with.
     */
    void swap(shared_ptr &ptr) noexcept {
        auto temp = native_ptr_;
        native_ptr_ = ptr.native_ptr_;
        ptr.native_ptr_ = temp;

        count_.swap(ptr.native_ptr_);
    }

    // Reference count operations:

    /**
     * \short Checks if the stored pointer is not null.
     */
    explicit operator bool() const noexcept { return 0 < count_.use_count(); }

    /**
     * \short Checks whether the managed object is managed only by the current
     *        shared_ptr instance.
     */
    bool unique() const noexcept { return 1 == count_.use_count(); }

    /**
     * \short Returns the number of shared_ptr objects referring to the same
     *        managed object.
     */
    long use_count() const noexcept { return count_.use_count(); }

    // Underlying pointer operations:

    /**
     * \short Dereferences the stored pointer.
     */
    T &operator*() const noexcept {
        return *native_ptr_;
    }

    /**
     * \short Provides access to the stored pointer.
     */
    T *operator->() const noexcept {
        return native_ptr_;
    }

    /**
     * \short Returns the stored pointer.
     */
    pointer get() const noexcept { return native_ptr_; }

 private:
    pointer native_ptr_;
    deleter_type deleter_;

    shared_ptr_count<DeleterRefCount> count_;


    // This allows pointer_cast functions to share the
    // reference count between different shared_ptr types.
    template <class U, class D, class D2>
    friend class shared_ptr;

    void acquire(pointer p) {
        count_.acquire(p);  // May throw std::bad_alloc (?)
        native_ptr_ = p;
    }

    void release() noexcept {
        count_.release(deleter_, native_ptr_);
        native_ptr_ = nullptr;
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
}  // namespace hs::mem
