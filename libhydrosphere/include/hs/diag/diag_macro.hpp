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

#include <hs/diag/diag_api.hpp>
#include <hs/hs_config.hpp>
#include <hs/hs_macro.hpp>

/**
 * \name Diagnostic API Macros
 * \addtogroup diag_api
 * @{
 */

/**
 * \def __HS_ABORT_CONDITIONAL(condition, ...)
 * \short Calls hs::diag::Abort with a given formatable message if the condition
 * evaluates to false. \remark The formatable message is striped out in release
 * builds.
 */

/**
 * \def __HS_DEBUG_ASSERT(condition)
 * \short Calls hs::diag::Abort if the condition evaluates to false in debug
 * builds.
 */

/**
 * \def __HS_ABORT()
 * \short Unconditionally calls hs::diag::Abort.
 */

/**
 * \def __HS_ASSERT(condition)
 * \short Calls hs::diag::Abort if the condition evaluates to false.
 */

/**
 * \def __HS_ABORT_UNLESS_NOT_NULL(ptr)
 * \short Calls hs::diag::Abort if the given ptr is null.
 */

/**
 * \def __HS_ABORT_CONDITIONAL_RESULT(condition, result)
 * \short Calls hs::diag::Abort with a given result if the condition evaluates
 * to false.
 */

#ifdef HYDROSPHERE_DEBUG_DIAG
#define __HS_ABORT_CONDITIONAL(condition, ...)                                 \
    (static_cast<void>(                                                        \
        (condition || (hs::diag::Abort(__HS_AS_STRING(condition), __FILE__,    \
                                       __FUNCTION__, __LINE__, ##__VA_ARGS__), \
                       true))))
#define __HS_DEBUG_ASSERT(condition) \
    __HS_ABORT_CONDITIONAL(condition, "Debug assertion failed")
#else
#define __HS_ABORT_CONDITIONAL(condition, ...) \
    (static_cast<void>((condition || (hs::diag::Abort("", "", "", 0), true))))
#define __HS_DEBUG_ASSERT(condition, ...)
#endif

#define __HS_ABORT() __HS_ABORT_CONDITIONAL(false, "Unknown Hydrosphere abort")
#define __HS_ASSERT(condition) \
    __HS_ABORT_CONDITIONAL(condition, "Assertion failed")
#define __HS_ABORT_UNLESS_NOT_NULL(ptr) __HS_ASSERT(ptr != nullptr)

#define __HS_ABORT_CONDITIONAL_RESULT(condition, result) \
    __HS_ABORT_CONDITIONAL(condition, &result, "Assertion failed")

/**
 * @}
 */
