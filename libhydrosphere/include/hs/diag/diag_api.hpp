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

#include <hs/hs_result.hpp>

namespace hs::diag {
/**
 * \defgroup diag_api Diagnostic API
 * \short API helping diagnostic issues.
 * \name Diagnostic API
 * \addtogroup diag_api
 * @{
 */

/**
 * \short Causes abnormal process termination.
 * 
 * This send abort information to an availaible logger (currently only hs::svc::OutputDebugString) before calling hs::svc::Break.
 * 
 * \remark In the future, this will propagate the abort to an observer for better debugging capabilities.
 */
void Abort(const char *failed_condition, const char *file_name,
           const char *function_name, int line_number) noexcept;

/**
 * \short Causes abnormal process termination with a custom message.
 * 
 * This send abort information to an availaible logger (currently only hs::svc::OutputDebugString) before calling hs::svc::Break.
 * 
 * \remark In the future, this will propagate the abort to an observer for better debugging capabilities.
 */
void Abort(const char *failed_condition, const char *file_name,
           const char *function_name, int line_number, const char *format,
           ...) noexcept;

/**
 * \short Causes abnormal process termination with a hs::Result and a custom message.
 * 
 * This send abort information to an availaible logger (currently only hs::svc::OutputDebugString) before calling hs::svc::Break.
 * 
 * \remark In the future, this will propagate the abort to an observer for better debugging capabilities.
 */
void Abort(const char *failed_condition, const char *file_name,
           const char *function_name, int line_number, const hs::Result *result,
           const char *format, ...) noexcept;

/**
 * @}
 */

}  // namespace hs::diag
