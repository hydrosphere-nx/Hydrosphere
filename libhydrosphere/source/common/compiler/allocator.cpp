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

constexpr size_t memory_align(size_t size, size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

/**
 * \short Impelements a memory allocator that maps a given block
 *        of memory and manages it.
 */
class MemoryAllocator {
 public:
    /**
     * \short Constructs a new MemoryAllocator.
     * \param[in] start_address The start address of the memory region.
     * \param[in] region_size The size of the memory region.
     */
    MemoryAllocator(void *start_address, size_t region_size) noexcept
        : start_address_(start_address), region_size_(region_size) {
        head_ = {0, nullptr};
        used_memory_ = 0;
    }

    /**
     * \short Constructs a new MemoryAllocator.
     * \param[in] start_address The start address of the memory region.
     * \param[in] end_address The end address of the memory region.
     */
    MemoryAllocator(void *start_address, void *end_address) noexcept
        : MemoryAllocator(
              start_address,
              static_cast<size_t>(static_cast<char *>(end_address) -
                                  static_cast<char *>(start_address))) {}

    /**
     * \short Allocates memory on the mapped region.
     * \param[in] size The size of memory to allocate.
     * \param[in] alignment The alignment to use. Defaults to 16.
     * \return Pointer to the allocated memory block.
     */
    void *Allocate(size_t size, size_t alignment = 16) noexcept {
        size = memory_align(size, alignment);

        if (size + used_memory_ > region_size_) return nullptr;

        MemoryBlock *block = head_.next;
        MemoryBlock **head = &head_.next;

        // Find a free memory block.
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

    /**
     * \short Frees allocated memory.
     * \param[in] pointer A pointer to the memory block to free.
     */
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
