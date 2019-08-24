/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/diag/diag_api.hpp>
#include <hs/svc.hpp>
#include <hs/util.hpp>

__HS_ATTRIBUTE_VISIBILITY_HIDDEN void AbortSimpleImpl(
    const char *failed_condition, const char *file_name,
    const char *function_name, int line_number) noexcept {
    __HS_DEBUG_LOG("! Condition: %s", failed_condition);
    __HS_DEBUG_LOG("! File: %s", file_name);
    __HS_DEBUG_LOG("! Function: %s", function_name);
    __HS_DEBUG_LOG("! Line: %d", line_number);
    __HS_DEBUG_LOG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    hs::svc::Break(0, 0, 0);
}

namespace hs::diag {

void Abort(const char *failed_condition, const char *file_name,
           const char *function_name, int line_number) noexcept {
    __HS_DEBUG_LOG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    __HS_DEBUG_LOG("! Hydrosphere Abort");

    AbortSimpleImpl(failed_condition, file_name, function_name, line_number);
}
void Abort(const char *failed_condition, const char *file_name,
           const char *function_name, int line_number, const char *format,
           ...) noexcept {
    __HS_DEBUG_LOG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    __HS_DEBUG_LOG("! Hydrosphere Abort with Custom Message");
    __HS_DEBUG_LOG("! Custom Message: ");
    va_list args;

    va_start(args, format);
    __HS_DEBUG_LOG_VARGS(format, args);
    va_end(args);

    AbortSimpleImpl(failed_condition, file_name, function_name, line_number);
}
void Abort(const char *failed_condition, const char *file_name,
           const char *function_name, int line_number, const hs::Result *result,
           const char *format, ...) noexcept {
    __HS_DEBUG_LOG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    __HS_DEBUG_LOG("! Hydrosphere Abort with Result");
    __HS_DEBUG_LOG("! Result: 08%x", result->GetValue());
    __HS_DEBUG_LOG("! Custom Message: ");
    va_list args;

    va_start(args, format);
    __HS_DEBUG_LOG_VARGS(format, args);
    va_end(args);

    AbortSimpleImpl(failed_condition, file_name, function_name, line_number);
}
}  // namespace hs::diag
