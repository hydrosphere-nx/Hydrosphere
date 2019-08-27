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

#include <stdint.h>
#include <hs/hs_macro.hpp>

namespace hs::util {

/**
 * \short default_intrusive_tag as defined in
 * [P0406-r1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0406r1.html#list)
 */
struct DefaultIntrusiveTag;

/**
 * \short intrusive_list_element as defined in
 * [P0406-r1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0406r1.html#list)
 */
template <class Tag = DefaultIntrusiveTag>
class IntrusiveListElement {
   private:
    IntrusiveListElement* prev;
    IntrusiveListElement* next;

   public:
    IntrusiveListElement() {
        prev = this;
        next = this;
    }
    __HS_DISALLOW_COPY(IntrusiveListElement);
    __HS_DISALLOW_ASSIGN(IntrusiveListElement);

    bool HasNext() const noexcept { return this->next != this; }

    IntrusiveListElement* GetPrev() noexcept { return this->prev; }

    const IntrusiveListElement* GetPrev() const noexcept { return this->prev; }

    IntrusiveListElement* GetNext() noexcept { return this->next; }

    const IntrusiveListElement* GetNext() const noexcept { return this->next; }

    void LinkPrev(IntrusiveListElement* link) noexcept { LinkPrev(link, link); }

    void LinkNext(IntrusiveListElement* link) noexcept { LinkNext(link, link); }

    void Unlink() noexcept { Unlink(this->next); }

    void LinkPrev(IntrusiveListElement* first_link,
                  IntrusiveListElement* last_link) noexcept {
        auto link = last_link->prev;

        first_link->prev = this->prev;
        link->next = this;
        this->prev->next = first_link;
        this->prev = link;
    }

    void LinkNext(IntrusiveListElement* first_link,
                  IntrusiveListElement* last_link) noexcept {
        auto link = last_link->prev;

        first_link->prev = this;
        link->next = this->next;
        this->next->prev = link;
        this->next = first_link;
    }

    void Unlink(IntrusiveListElement* last_link) noexcept {
        auto link = last_link->prev;

        this->prev->next = last_link;
        last_link->prev = this->prev;
        link->next = this;
        this->prev = link;
    }

    template <class T>
    static T* GetElementPointer(IntrusiveListElement* other) noexcept {
        return reinterpret_cast<T*>(reinterpret_cast<char*>(other) -
                                    GetLinkOffset<T>());
    }

    template <class T>
    static const T* GetElementPointer(
        const IntrusiveListElement* other) noexcept {
        return reinterpret_cast<const T*>(reinterpret_cast<const char*>(other) -
                                          GetLinkOffset<T>());
    }

    template <class T>
    static T& GetElement(IntrusiveListElement& other) noexcept {
        return *reinterpret_cast<T*>(reinterpret_cast<char*>(&other) -
                                     GetLinkOffset<T>());
    }

    template <class T>
    static uintptr_t GetLinkOffset() noexcept {
        return reinterpret_cast<uintptr_t>(&((reinterpret_cast<T*>(0))->prev));
    }
};

/**
 * \short Partial implementation of intrusive_list as defined in
 * [P0406-r1](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0406r1.html#list)
 * \remark This doesn't implement any methods related to the std namespace.
 */
template <class T, class Tag = DefaultIntrusiveTag>
class IntrusiveList {
   public:
    __HS_DISALLOW_COPY(IntrusiveList);

    /**
     * \short The type of the element the iterator can point to.
     */
    typedef T value_type;

    /**
     * \short The type of a reference to an element the iterator can point to.
     */
    typedef value_type& reference;

    /**
     * \short The type of a const reference to an element the iterator can point
     * to.
     */
    typedef const value_type& const_reference;

    class iterator;
    class const_iterator;

    /**
     * \short The type of the count of element the iterator hold.
     */
    typedef int size_type;

    /**
     * \short Type to express the result of subtracting one iterator from
     * another.
     */
    typedef int difference_type;
    // We don't have the standard library.
    // typedef std::reverse_iterator<iterator> reverse_iterator;
    // typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // Define iterators implementation
    class const_iterator {
       public:
        friend class iterator;
        friend class IntrusiveList;

        typedef IntrusiveList::value_type value_type;
        typedef IntrusiveList::difference_type difference_type;
        typedef const value_type& const_reference;
        typedef const value_type* const_pointer;

        explicit const_iterator(const_pointer value) noexcept : value(value) {}
        explicit const_iterator(const IntrusiveListElement<Tag>* value) noexcept
            : value(IntrusiveListElement<Tag>::template GetElementPointer<
                    value_type>(value)) {}
        explicit const_iterator(iterator& other) noexcept
            : value(other.value) {}

        const_reference operator*() noexcept { return *value; }

        const_pointer operator->() noexcept { return value; }

        const_iterator& operator++() noexcept {
            value = IntrusiveListElement<Tag>::template GetElementPointer<
                value_type>(value->GetNext());
            return *this;
        }

        const_iterator operator++(int) noexcept {
            const_iterator temp(*this);
            ++(*this);
            return temp;
        }

        const_iterator& operator--() noexcept {
            value = IntrusiveListElement<Tag>::template GetElementPointer<
                value_type>(value->GetPrev());
            return *this;
        }

        const_iterator operator--(int) noexcept {
            const_iterator temp(*this);
            --(*this);
            return temp;
        }

        bool operator==(const const_iterator& target) const noexcept {
            return value == target.value;
        }

        bool operator!=(const const_iterator& target) const noexcept {
            return value != target.value;
        }

       private:
        const_pointer value;
    };

    class iterator {
       public:
        friend class const_iterator;
        friend class IntrusiveList;

        typedef IntrusiveList::value_type value_type;
        typedef IntrusiveList::difference_type difference_type;
        typedef value_type& reference;
        typedef value_type* pointer;

        explicit iterator(pointer value) noexcept : value(value) {}
        explicit iterator(IntrusiveListElement<Tag>* value) noexcept
            : value(IntrusiveListElement<Tag>::template GetElementPointer<
                    value_type>(value)) {}
        explicit iterator(const_iterator& other) noexcept
            : value(const_cast<T*>(other.value)) {}

        reference operator*() noexcept { return *value; }

        pointer operator->() noexcept { return value; }

        iterator& operator++() noexcept {
            value = IntrusiveListElement<Tag>::template GetElementPointer<
                value_type>(value->GetNext());
            return *this;
        }

        iterator operator++(int) noexcept {
            iterator temp(*this);
            ++(*this);
            return temp;
        }

        iterator& operator--() noexcept {
            value = IntrusiveListElement<Tag>::template GetElementPointer<
                value_type>(value->GetPrev());
            return *this;
        }

        iterator operator--(int) noexcept {
            iterator temp(*this);
            --(*this);
            return temp;
        }

        bool operator==(const iterator& target) const noexcept {
            return value == target.value;
        }

        bool operator!=(const iterator& target) const noexcept {
            return value != target.value;
        }

       private:
        pointer value;
    };

    // Construct
    IntrusiveList() noexcept : root() {}

    // Do we need thoses?
    /*template <class InputIterator>
      IntrusiveList(InputIterator first, InputIterator last);
    IntrusiveList(IntrusiveList&& x) noexcept;
    ~IntrusiveList() noexcept;
    IntrusiveList& operator=(IntrusiveList&& x) noexcept;
    template <class InputIterator>
      void assign(InputIterator first, InputIterator last);*/

    // Iterators
    /**
     * \short Returns an iterator pointing to the first element in the
     * container.
     */
    iterator begin() noexcept { return iterator(root.GetNext()); }

    /**
     * \short Returns an const_iterator pointing to the first element in the
     * container.
     */
    const_iterator begin() const noexcept {
        return const_iterator(root.GetNext());
    }

    /**
     * \short Returns an iterator referring to the past-the-end element in the
     * container.
     */
    iterator end() noexcept { return iterator(&root); }

    /**
     * \short Returns an const_iterator referring to the past-the-end element in
     * the container.
     */
    const_iterator end() const noexcept { return const_iterator(&root); }

    // We don't have the standard library.
    /*reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;*/

    /**
     * \short Returns a const_iterator pointing to the first element in the
     * container.
     *
     * \warning If the container is empty, the returned iterator value shall not
     * be dereferenced.
     */
    const_iterator cbegin() const noexcept { return this->begin(); }

    /**
     * \short Returns a const_iterator pointing to the past-the-end element in
     * the container.
     *
     * If the container is empty, this function returns the same as
     * IntrusiveList::cbegin. \warning The value returned shall not be
     * dereferenced.
     */
    const_iterator cend() const noexcept { return this->end(); }
    // We don't have the standard library.
    // const_reverse_iterator crbegin() const noexcept;
    // const_reverse_iterator crend() const noexcept;

    /**
     * \short Test whether container is empty.
     */
    bool empty() const noexcept { return !root.HasNext(); }

    /**
     * \short Returns the number of elements in the container.
     */
    size_type size() const noexcept {
        size_type result = 0;
        const_iterator temp = begin();
        for (; temp != end(); ++temp) ++result;

        return result;
    }
    // size_type max_size() const noexcept;

    /**
     * \short Returns a reference to the first element in the container.
     *
     * Unlike member IntrusiveList::begin, which returns an iterator to this
     * same element, this function returns a direct reference. \warning Calling
     * this function on an empty container causes undefined behavior.
     */
    reference front() noexcept {
        return IntrusiveListElement<Tag>::template GetElement<value_type>(
            *root.GetNext());
    }

    /**
     * \short Returns a reference to the first element in the container.
     *
     * Unlike member IntrusiveList::cbegin, which returns an iterator to this
     * same element, this function returns a direct reference. \warning Calling
     * this function on an empty container causes undefined behavior.
     */
    const_reference front() const noexcept {
        return IntrusiveListElement<Tag>::template GetElement<value_type>(
            *root.GetNext());
    }

    /**
     * \short Returns a reference to the last element in the container.
     *
     * Unlike member IntrusiveList::cbegin, which returns an iterator to this
     * same element, this function returns a direct reference. \warning Calling
     * this function on an empty container causes undefined behavior.
     */
    reference back() noexcept {
        return IntrusiveListElement<Tag>::template GetElement<value_type>(
            *root.GetPrev());
    }

    /**
     * \short Returns a reference to the last element in the container.
     *
     * Unlike member IntrusiveList::cbegin, which returns an iterator to this
     * same element, this function returns a direct reference. \warning Calling
     * this function on an empty container causes undefined behavior.
     */
    const_reference back() const noexcept {
        return IntrusiveListElement<Tag>::template GetElement<value_type>(
            *root.GetPrev());
    }

    /**
     * \short Adds a new element at the start of the container after its current
     * first element.
     *
     * \remark This effectively increases the container size by one.
     */
    void push_front(T& x) noexcept { this->root.LinkNext(&x); }

    /**
     * \short Adds a new element at the end of the container after its current
     * last element.
     *
     * \remark This effectively increases the container size by one.
     */
    void push_back(T& x) noexcept { this->root.LinkPrev(&x); }

    /**
     * \short Removes the last element in the container, effectively reducing
     * the container size by one.
     */
    void pop_back() noexcept { this->root.GetPrev()->Unlink(); }

    /**
     * \short Removes the first element in the container, effectively reducing
     * the container size by one.
     */
    void pop_front() noexcept { this->root.GetNext()->Unlink(); }

    /**
     * \short The container is extended by inserting new elements before the
     * element at the specified position, effectively increasing the container
     * size by the number of elements inserted.
     */
    iterator insert(const_iterator position, T& x) noexcept {
        T* ref = const_cast<T*>(position.value);

        ref->LinkPrev(&x);

        return iterator(&x);
    }

    /*template <class InputIterator>
    iterator insert(const_iterator position, InputIterator first,
                    InputIterator last);*/

    /**
     * \short Removes from the container a single element (position) .
     * \remark This effectively reduces the container size by one.
     */
    iterator erase(const_iterator position) noexcept {
        return this->erase(position, cend());
    }

    /**
     * \short Removes from the container a range of elements. ([first,last))
     * \remark This effectively reduces the container size by the number of
     * elements removed.
     */
    iterator erase(const_iterator position, const_iterator last) noexcept {
        if (position == last) {
            return last;
        }

        iterator temp = iterator(position);
        temp->Unlink();
        return ++temp;
    }
    // void swap(IntrusiveList&) noexcept;

    /**
     * \short Removes all elements from the container, leaving the container
     * with a size of 0.
     */
    void clear() noexcept {
        while (!this->empty()) {
            this->pop_front();
        }
    }

    // List operations
    /* void splice(const_iterator position, IntrusiveList& x) noexcept;
    void splice(const_iterator position, IntrusiveList&& x) noexcept;
    void splice(const_iterator position, IntrusiveList& x,
                const_iterator i) noexcept;
    void splice(const_iterator position, IntrusiveList&& x,
                const_iterator i) noexcept;
    void splice(const_iterator position, IntrusiveList& x, const_iterator first,
                const_iterator last) noexcept;
    void splice(const_iterator position, IntrusiveList&& x,
                const_iterator first, const_iterator last) noexcept;*/

    // void remove(const T& value);
    // template <class Predicate> void remove_if(Predicate pred);

    // maybe uneeded as we don't have the standard library.
    /*void unique();
    template <class BinaryPredicate>
    void unique(BinaryPredicate binary_pred);*/

    // sort and reverse are irrelevent for intrusive lists.
    /*void sort();
    template <class Compare> void sort(Compare comp);

    void reverse() noexcept;*/
   private:
    IntrusiveListElement<Tag> root;
};
}  // namespace hs::util
