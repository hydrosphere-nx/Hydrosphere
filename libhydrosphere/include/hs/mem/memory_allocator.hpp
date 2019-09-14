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

#include <hs/hs_config.hpp>
#include <hs/diag/diag_macro.hpp>
#include <hs/util/util_intrusive_list.hpp>
#include <hs/util/util_std_new.hpp>

namespace hs::mem {
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
        : free_list_(), start_address_(start_address),
          region_size_(region_size) {
        used_memory_ = 0;

        MemoryBlock *first_block  = reinterpret_cast<MemoryBlock*>(
            start_address);
        // Enforce the constructor is called to avoid using uninialized data.
        new (first_block) MemoryBlock();

        first_block->size = region_size - sizeof(MemoryBlock);
        free_list_.push_front(*first_block);
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
    void *Allocate(size_t size, size_t alignment = __HS_CPU_PTR_ALIGN)
        noexcept {
        if (size == 0 || alignment == 0) {
            return nullptr;
        }

        // Ensure CPU ptr alignment
        size = memory_align(memory_align(size, alignment), __HS_CPU_PTR_ALIGN);

        void *ptr = nullptr;

        for (auto it = free_list_.begin(); it != free_list_.end(); ++it) {
            // FIXME: Really really dirty, change this!
            hs::util::IntrusiveList<MemoryBlock>::const_iterator cit =
                hs::util::IntrusiveList<MemoryBlock>::const_iterator(it);

            MemoryBlock &free_block = *it;

            __HS_DEBUG_ASSERT(free_block.size != 0);

            if (free_block.size < size) {
                continue;
            }

            size_t remaining_bytes = free_block.size - size;

            // Found a block that fit corectly.
            if (remaining_bytes >= sizeof(MemoryBlock) ||
                remaining_bytes >= 0) {
                char *block_data_start = reinterpret_cast<char*>(&free_block)
                    + sizeof(MemoryBlock);

                if (remaining_bytes > sizeof(MemoryBlock)) {
                    // We have enough space for the block and data, we split
                    // our block.
                    free_block.size -= remaining_bytes;

                    MemoryBlock *new_block = reinterpret_cast<MemoryBlock*>(
                        block_data_start + free_block.size);
                    // Enforce the constructor is called to avoid using
                    // uninialized data.
                    new (new_block) MemoryBlock();
                    new_block->size = remaining_bytes - sizeof(MemoryBlock);

                    free_list_.insert(cit, *new_block);
                }

                ptr = block_data_start;
                free_list_.remove(*cit);
                break;
            }
        }

        return ptr;
    }

    /**
     * \short Frees allocated memory.
     * \param[in] pointer A pointer to the memory block to free.
     */
    void Free(void *pointer) noexcept {
        if (pointer == nullptr) {
            return;
        }

        MemoryBlock *block = GetMemoryBlockFromUserAddress(pointer);

        // Invalid pointer
        if (block == nullptr) {
            return;
        }

        // If the free list is empty, add the block to it!
        if (free_list_.empty()) {
            free_list_.push_front(*block);
        } else {
            MemoryBlock *front_free_block = &free_list_.front();

            // If the block is before the first element of the free list
            if (front_free_block < block) {
                // If our block is just before the first element, we replace
                // the first element
                if (block->IsJustAfterMemoryBlock(front_free_block)) {
                    block->size += front_free_block->size;
                    free_list_.remove(*front_free_block);
                }

                free_list_.push_front(*block);

                return;
            }

            // Search for the position to insert ourself.
            auto target_position = free_list_.cbegin();

            for (; target_position != free_list_.cend(); ++target_position) {
                const MemoryBlock *tmp = &*target_position;
                if (tmp > block) {
                    break;
                }
            }

            // Insert the our block in the free list.
            free_list_.insert(target_position, *block);

            // If our block is just before the our target position, we replace
            // the first element.
            // TODO(Kaenbyō): Also try to merge chunk before the block that is
            // being freed.
            if (block->IsJustAfterMemoryBlock(&*target_position)) {
                block->size += front_free_block->size;
                free_list_.erase(target_position);
            }
        }
    }

 private:
    struct MemoryBlock : public hs::util::IntrusiveListElement<> {
        size_t size;
        char reserved[4];

        bool IsJustAfterMemoryBlock(const MemoryBlock *other) const {
            const char *other_raw = reinterpret_cast<const char*>(other);
            const char *this_raw = reinterpret_cast<const char*>(this);

            return other_raw + other->size == this_raw;
        }
    };

    #if HYDROSPHERE_TARGET_AARCH64
    static_assert(sizeof(MemoryBlock) == 0x20, "MemoryBlock size isn't right");
    #elif HYDROSPHERE_TARGET_AARCH32
    static_assert(sizeof(MemoryBlock) == 0x10, "MemoryBlock size isn't right");
    #endif

    hs::util::IntrusiveList<MemoryBlock> free_list_;

    void *start_address_;

    size_t region_size_;

    size_t used_memory_;

    constexpr size_t memory_align(size_t size, size_t alignment) {
        return (size + alignment - 1) & ~(alignment - 1);
    }

    MemoryBlock *GetMemoryBlockFromUserAddress(void *pointer) noexcept {
        char *block_position = static_cast<char *>(pointer) -
            sizeof(MemoryBlock);
        char *start_address = static_cast<char *>(start_address_);

        // Check the pointer sanity before accepting it
        if (start_address_ < block_position ||
            block_position > start_address + region_size_) {
            return nullptr;
        }

        return reinterpret_cast<MemoryBlock *>(block_position);
    }
};
}  // namespace hs::mem
