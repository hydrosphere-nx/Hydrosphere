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
#include <hs/os/os_critical_section.hpp>
#include <hs/util/util_object_storage.hpp>

namespace hs::os::detail {
class VirtualMemoryAllocator {
   private:
    hs::os::CriticalSection critical_section;
    uintptr_t address_space_start;
    uintptr_t address_space_end;
    size_t guard_page_size;
    uintptr_t next_availaible_address;
    static inline bool InsideAddressRange(uintptr_t start_address,
                                          uintptr_t end_address,
                                          uintptr_t target_address) noexcept {
        return (target_address >= start_address) &&
               (target_address < end_address);
    }

   public:
    VirtualMemoryAllocator(uintptr_t address_space_start,
                           uintptr_t address_space_end,
                           size_t guard_page_size) noexcept
        : critical_section(),
          address_space_start(address_space_start),
          address_space_end(address_space_end),
          guard_page_size(guard_page_size),
          next_availaible_address(address_space_start) {}
    __HS_DISALLOW_COPY(VirtualMemoryAllocator);
    void *Reserve(size_t size, size_t alignement) noexcept;
    void Free(void *ptr) noexcept;
};

extern __HS_ATTRIBUTE_VISIBILITY_HIDDEN
    hs::util::ObjectStorage<VirtualMemoryAllocator>
        g_AddressSpaceAllocator;
extern __HS_ATTRIBUTE_VISIBILITY_HIDDEN
    hs::util::ObjectStorage<VirtualMemoryAllocator>
        g_StackAllocator;

void InitializeVirtualMemoryAllocators(void) noexcept;

}  // namespace hs::os::detail
