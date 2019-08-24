/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/diag.hpp>
#include <hs/hs_macro.hpp>
#include <hs/os/os_api.hpp>
#include <hs/svc.hpp>
#include <hs/util.hpp>
#include <os/detail/os_virtualmemory_allocator.hpp>

#define PAGE_SIZE 0x1000

namespace hs::os::detail {
__HS_ATTRIBUTE_VISIBILITY_HIDDEN hs::util::ObjectStorage<VirtualMemoryAllocator>
    g_AddressSpaceAllocator;
__HS_ATTRIBUTE_VISIBILITY_HIDDEN hs::util::ObjectStorage<VirtualMemoryAllocator>
    g_StackAllocator;

void *VirtualMemoryAllocator::Reserve(size_t size, size_t alignement) noexcept {
    this->critical_section.Enter();

    // First page align the size
    size = (size + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);

    // Make sure we are going to be in range
    if (!this->InsideAddressRange(this->address_space_start,
                                  this->address_space_end,
                                  this->next_availaible_address)) {
        this->next_availaible_address = this->address_space_start;
    }

    uintptr_t address = this->next_availaible_address;
    hs::svc::MemoryInfo memory_info;
    uint32_t page_info;
    bool already_retried = false;

    while (true) {
        // Add guard pages
        address += this->guard_page_size;

        // Align address to user requirement
        address = (address + (alignement - 1)) & ~(alignement - 1);

        if (!this->InsideAddressRange(this->address_space_start,
                                      this->address_space_end, address)) {
            // We scanned all the address space but haven't found a free spot so
            // we need to return here
            if (already_retried) {
                address = 0;
                break;
            } else {
                // We might have some free spots before the last allocated
                // address, try to scan this part
                address = this->address_space_start;
                already_retried = true;
                continue;
            }
        }

        hs::svc::QueryMemory(&memory_info, &page_info, address);

        // not availaible/enough memory, try next region
        if (memory_info.type != 0 || size > memory_info.size) {
            address = memory_info.address + memory_info.size;
            continue;
        }

        this->next_availaible_address = address + size;
        break;
    }

    this->critical_section.Leave();
    return reinterpret_cast<void *>(address);
}

void VirtualMemoryAllocator::Free(void *ptr) noexcept {
    __HS_IGNORE_ARGUMENT(ptr);
}

void InitializeVirtualMemoryAllocators(void) noexcept {
    uint64_t address_space_start = 0;
    uint64_t address_space_size = 0;

    uint64_t stack_address_space_start = 0;
    uint64_t stack_address_space_size = 0;

    if (hs::svc::GetInfo(&address_space_start, svc::InfoType::AddressSpaceBase,
                         hs::os::GetProcessPseudoHandle(), 0)
            .Err()) {
        // TODO(Kaenbyō): 1.0.0 support hardcoded values
        __HS_ABORT();
    } else {
        auto result = hs::svc::GetInfo(&address_space_size,
                                       svc::InfoType::AddressSpaceSize,
                                       hs::os::GetProcessPseudoHandle(), 0);
        __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);

        result = hs::svc::GetInfo(&stack_address_space_start,
                                  svc::InfoType::StackRegionBase,
                                  hs::os::GetProcessPseudoHandle(), 0);
        __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);

        result = hs::svc::GetInfo(&stack_address_space_size,
                                  svc::InfoType::StackRegionSize,
                                  hs::os::GetProcessPseudoHandle(), 0);
        __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);
    }

    *g_AddressSpaceAllocator = VirtualMemoryAllocator(
        address_space_start, address_space_start + address_space_size, 0x1000);
    *g_StackAllocator = VirtualMemoryAllocator(
        stack_address_space_start,
        stack_address_space_start + stack_address_space_size, 0x1000);
}
}  // namespace hs::os::detail
