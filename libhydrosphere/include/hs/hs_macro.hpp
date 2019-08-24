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

#define __HS_ATTRIBUTE_VISIBILITY_HIDDEN __attribute__((visibility("hidden")))
#define __HS_ATTRIBUTE_USED __attribute__((used))
#define __HS_ATTRIBUTE_NORETURN __attribute__((noreturn))
#define __HS_ATTRIBUTE_PACKED __attribute__((packed))
#define __HS_ATTRIBUTE_WEAK __attribute__((weak))
#define __HS_ATTRIBUTE_SECTION(section_name) \
    __attribute__((section(section_name)))
#define __HS_ATTRIBUTE_NAKED __attribute((naked))
#define __HS_ATTRIBUTE_ALIGNED(align) __attribute__((aligned(align)))
#define __HS_ASM __asm__

#define __HS_DISALLOW_COPY(TypeName)   \
    /** \private */                    \
    TypeName(const TypeName&) = delete
#define __HS_DISALLOW_ASSIGN(TypeName) \
    /** \private */                    \
    TypeName& operator=(const TypeName&) = delete
#define __HS_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
    /** \private */                                   \
    TypeName() = delete;                              \
    __HS_DISALLOW_COPY(TypeName);                     \
    __HS_DISALLOW_ASSIGN(TypeName);

#define __HS_IGNORE_ARGUMENT(x) (void)(x)
#define __HS_BIT(n) (1U << (n))
#define __HS_AS_STRING(x) #x
#define __HS_CONCATENATE_STRING(x, y) x##y
