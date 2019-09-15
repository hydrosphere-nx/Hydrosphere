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

/**
 * \short Copies len bytes from src directly to dst.
 * \param[out] dst Pointer to the destination array where the content is to be
 *                 copied.
 * \param[in] src Pointer to the source of data to be copied. \param[in]
 * len Number of bytes to copy.
 */
extern "C" __HS_ATTRIBUTE_WEAK void *memcpy(void *dst, const void *src,
                                            size_t len) {
    const char *from = (const char *)src;
    char *to = reinterpret_cast<char *>(dst);

    while (len-- > 0) *to++ = *from++;
    return dst;
}

/**
 * \short Sets the first n bytes of s to c (interpreted as an unsigned char).
 * \param[out] s Pointer to the block of memory to fill.
 * \param[in] c Value to be set. Will be casted to unsigned char.
 * \param[in] n Number of bytes to be set to the value.
 */
extern "C" __HS_ATTRIBUTE_WEAK void *memset(void *s, int c, size_t n) {
    unsigned char *p = reinterpret_cast<unsigned char *>(s);
    while (n--) *p++ = (unsigned char)c;
    return s;
}
