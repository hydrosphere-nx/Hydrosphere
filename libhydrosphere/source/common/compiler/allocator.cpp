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

#include "hs/hs_macro.hpp"
#include "hs/svc/svc_api.hpp"

extern void *memcpy(void *dst, const void *src, size_t len);
extern void *memset(void *s, int c, size_t n);

extern "C" __HS_ATTRIBUTE_WEAK void *sbrk(size_t increment) {
    using namespace hs::svc;

    Handle handle;  // How to initialize this?
    size_t base_address = 0, out_address = 0;

    GetInfo(&base_address, InfoType::HeapRegionBaseAddr, handle, 0);

    size_t size_needed = increment - base_address;
    size_t new_size;

    // Must be a multiple of 0x200000.
    size_t remainder = size_needed % 0x200000;
    if (remainder != 0)
        new_size = size_needed + 0x200000 - remainder;
    else
        new_size = size_needed;

    SetHeapSize(&out_address, new_size);

    return (void *)(&out_address - &size_needed);
}

struct FreeBlock {
    size_t size;
    FreeBlock *next;
};

static FreeBlock free_block_list_head = {0, nullptr};
static const size_t align_to = 16;

extern "C" __HS_ATTRIBUTE_WEAK void *malloc(size_t size) {
    size = (size + sizeof(size_t) + (align_to - 1)) & ~(align_to - 1);
    FreeBlock *block = free_block_list_head.next;
    FreeBlock **head = &(free_block_list_head.next);

    while (block != nullptr) {
        if (block->size >= size) {
            *head = block->next;
            return ((char *)block) + sizeof(size_t);
        }
        head = &(block->next);
        block = block->next;
    }

    block = (FreeBlock *)sbrk(size);
    block->size = size;

    return ((char *)block) + sizeof(size_t);
}

extern "C" __HS_ATTRIBUTE_WEAK void free(void *ptr) {
    auto *block = (FreeBlock *)(((char *)ptr) - sizeof(size_t));
    block->next = free_block_list_head.next;
    free_block_list_head.next = block;
}

extern "C" __HS_ATTRIBUTE_WEAK void *calloc(size_t num, size_t nsize) {
    size_t size;
    void *block;

    if (!num || !nsize) return nullptr;

    size = num * nsize;
    if (nsize != size / num)  // Overflow check.
        return nullptr;

    block = malloc(size);
    if (!block) return nullptr;

    memset(block, 0, size);
    return block;
}

extern "C" __HS_ATTRIBUTE_WEAK void *realloc(void *ptr, size_t size) {
    FreeBlock *block;
    void *res;

    if (!ptr || !size) return malloc(size);

    block = (FreeBlock *)ptr - 1;
    if (block->size >= size) return block;

    res = malloc(size);
    if (res) {
        memcpy(res, ptr, block->size);
        free(block);
    }
    return res;
}
