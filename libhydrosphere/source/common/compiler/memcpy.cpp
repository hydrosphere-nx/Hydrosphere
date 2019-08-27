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
#include <hs/hs_macro.hpp>

// We define memcpy as we don't have any libraries that can provide it.
// If there is any, as this is weak, it's going to be discared.
extern "C" __HS_ATTRIBUTE_WEAK void *memcpy(void *dst, const void *src,
                                            size_t len) {
    const char *from = (const char *)src;
    char *to = reinterpret_cast<char *>(dst);

    while (len-- > 0) *to++ = *from++;
    return dst;
}

extern "C" __HS_ATTRIBUTE_WEAK void *memset(void *s, int c, size_t n) {
    unsigned char *p = reinterpret_cast<unsigned char *>(s);
    while (n--) *p++ = (unsigned char)c;
    return s;
}
