/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <stddef.h>

constexpr size_t MemoryAlign(size_t size, size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

class MemoryAllocator {
   public:
    MemoryAllocator(void *start_address, size_t region_size) noexcept
        : start_address_(start_address), region_size_(region_size) {
        head_ = {0, nullptr};
        used_memory_ = 0;
    }

    MemoryAllocator(void *start_address, void *end_address) noexcept
        : MemoryAllocator(
              start_address,
              static_cast<size_t>(static_cast<char *>(end_address) -
                                  static_cast<char *>(start_address))) {}

    void *Allocate(size_t size, size_t alignment = 16) noexcept {
        size = MemoryAlign(size, alignment);

        if (size + used_memory_ > region_size_) return nullptr;

        MemoryBlock *block = head_.next;
        MemoryBlock **head = &head_.next;

        while (block) {
            if (block->size >= size) {
                *head = block->next;
                return reinterpret_cast<char *>(block) + sizeof(size_t);
            }

            head = &block->next;
            block = block->next;
        }

        block = reinterpret_cast<MemoryBlock *>(
            static_cast<char *>(start_address_) + used_memory_);
        used_memory_ += size;
        block->size = size;

        return reinterpret_cast<char *>(block) + sizeof(MemoryBlock);
    }

    void Free(void *pointer) noexcept {
        auto block = reinterpret_cast<MemoryBlock *>(
            static_cast<char *>(pointer) - sizeof(MemoryBlock));
        block->next = head_.next;
        head_.next = block;
    }

   private:
    struct MemoryBlock {
        size_t size;
        MemoryBlock *next;
    };

    MemoryBlock head_;

    void *start_address_;

    size_t region_size_;

    size_t used_memory_;
};
