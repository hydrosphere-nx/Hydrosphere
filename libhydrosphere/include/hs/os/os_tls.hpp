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

#include <hs/os/os_thread_api.hpp>

namespace hs::os {

/**
 * Thread Local Storage Internal API
 * \private
 */
class ThreadLocalStorage {
 public:
    /**
     * The IPC command buffer.
     */
    char command_buffer[0x100];
    __HS_DISALLOW_IMPLICIT_CONSTRUCTORS(ThreadLocalStorage);

    /**
     * Get a pointer to the thread local storage of the current thread.
     */
    static ThreadLocalStorage *GetThreadLocalStorage() noexcept;


    /**
     * Get the thread context of the current thread.
     */
    inline Thread *GetThreadContext() noexcept { return this->context; }

    /**
     * Set the thread context of the current thread.
     */
    inline void SetThreadContext(Thread *thread_context) noexcept {
        this->context = thread_context;
    }

 private:
    /**
     * The thread context attached to this TLS storage.
     */
    Thread *context;
};

}  // namespace hs::os
