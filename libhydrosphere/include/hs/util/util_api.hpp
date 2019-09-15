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

#include <stdarg.h>
#include <stddef.h>

/**
 * \short Internal symbol pointing to the actual vsnprintf(3) implementation.
 * \private
 */
extern "C" int __hydrosphere_vsnprintf(char *str, size_t size,
                                       const char *format, va_list args);

/**
 * \short Internal symbol pointing to the actual snprintf(3) implementation.
 * \private
 */
extern "C" int __hydrosphere_snprintf(char *str, size_t size,
                                      const char *format, ...);

namespace hs::util {

/**
 * \short Implementation of vsnprintf(3)
 */
inline int VSNPrintf(char *str, size_t size, const char *format,
                     va_list args) noexcept {
    return __hydrosphere_vsnprintf(str, size, format, args);
}

/**
 * \short Implementation of snprintf(3)
 */
inline int SNPrintf(char *str, size_t size, const char *format, ...) noexcept {
    va_list args;

    va_start(args, format);
    int res = VSNPrintf(str, size, format, args);
    va_end(args);

    return res;
}

}  // namespace hs::util

/**
 * \def __HS_DEBUG_LOG(...)
 * \short printf helper to hs::svc::OutputDebugString
 */
#define __HS_DEBUG_LOG(...)                              \
    {                                                    \
        char log_buf[0x200];                             \
        hs::util::SNPrintf(log_buf, 0x200, __VA_ARGS__); \
        hs::svc::OutputDebugString(log_buf, 0x200);      \
    }

/**
 * \def __HS_DEBUG_LOG_VARGS(...)
 * \short vprintf helper to hs::svc::OutputDebugString
 */
#define __HS_DEBUG_LOG_VARGS(format, va_args)                 \
    {                                                         \
        char log_buf[0x200];                                  \
        hs::util::VSNPrintf(log_buf, 0x200, format, va_args); \
        hs::svc::OutputDebugString(log_buf, 0x200);           \
    }
