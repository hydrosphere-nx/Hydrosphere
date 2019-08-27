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
#include <hs/util/util_template_api.hpp>

namespace hs {
namespace svc {

/**
 * \short Opaque type representing an Horizon Handle.
 */
class Handle {
   private:
    /**
     * \private
     * \short The raw handle value.
     */
    uint32_t value;

   public:
    /**
     * \short Returns the raw handle value stored inside Handle.
     */
    inline uint32_t GetValue() noexcept { return this->value; }

    /**
     * \short Set the raw handle value stored inside Handle.
     * \param[in] value The new raw handle value.
     */
    inline void SetValue(uint32_t value) noexcept { this->value = value; }

    /**
     * \short Checks if this Handle is considered invalid (value of 0)
     */
    inline bool IsInvalid() noexcept { return this->value == 0; }

    /**
     * \short Create an Handle from a raw handle value
     * \param[in] value The raw handle value.
     */
    inline static Handle FromRawValue(uint32_t value) {
        auto res = Handle();
        res.SetValue(value);
        return res;
    }
} __HS_ATTRIBUTE_PACKED;

static_assert(hs::util::is_pod<Handle>::value, "Handle isn't pod");

inline bool operator==(Handle a, Handle b) noexcept {
    return a.GetValue() == b.GetValue();
}

inline bool operator!=(Handle a, Handle b) noexcept {
    return a.GetValue() == b.GetValue();
}

enum class MemoryPermission {
    None = 0,
    Read = __HS_BIT(0),
    Write = __HS_BIT(1),
    Execute = __HS_BIT(2),
};

inline MemoryPermission operator|(MemoryPermission a,
                                  MemoryPermission b) noexcept {
    return static_cast<MemoryPermission>(static_cast<int>(a) |
                                         static_cast<int>(b));
}

struct MemoryInfo {
    uint64_t address;
    uint64_t size;
    uint32_t type;
    uint32_t attribute;
    uint32_t permission;
    uint32_t device_ref_count;
    uint32_t ipc_ref_count;
    uint32_t padding;
};

enum class LimitableResource {
    Memory = 0,
    Threads = 1,
    Events = 2,
    TransferMemories = 3,
    Sessions = 4,
};

enum class ThreadActivity { UnPaused = 0, Paused = 1 };

enum class ProcessActivity { UnPaused = 0, Paused = 1 };

// TODO(Kaenbyō): use infos from the TRM as it's a 1:1 mapping.
enum class Interrupt {};

enum class InterruptType {
    LevelTrigger = 0,
    EdgeTrigger = 1,
};

// TODO(Kaenbyō): populate this
enum class InfoType {
    AddressSpaceBase = 12,
    AddressSpaceSize = 13,
    StackRegionBase = 14,
    StackRegionSize = 15
};

enum class DeviceName {
    AFI = 0,
    AVPC,
    DC,
    DCB,
    HC,
    BDA,
    ISP2,
    MSENC_NVENC,
    NV,
    NV2,
    PPCS,
    SATA,
    VI,
    VIC,
    XUSB_HOST,
    XUSB_DEV,
    TSEC,
    PPCS1,
    DC1,
    SDMMC1A,
    SDMMC2A,
    SDMMC3A,
    SDMMC4A,
    ISP2B,
    GPU,
    GPUB,
    PPCS2,
    NVDEC,
    APE,
    SE,
    NVJPG,
    HC1,
    SE1,
    AXIAP,
    ETR,
    TSECB,
    TSEC1,
    TSECB1,
    NVDEC1,
};

enum class ProcessInfoType {
    ProcessState = 0,
};

}  // namespace svc
}  // namespace hs
