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
#include <hs/os.hpp>
#include <hs/svc.hpp>
#include <hs/util.hpp>

#include <hs/os/os_tls.hpp>
#include <os/detail/os_threadlist.hpp>
#include <os/detail/os_virtualmemory_allocator.hpp>

namespace hs {
namespace os {
namespace detail {
void UserExceptionHandler(void);
}  // namespace detail
}  // namespace os

namespace init {
void Start(uint64_t thread_handle, uintptr_t argument_address,
           void (*notify_exception_handler_ready)(),
           void (*call_initializator)());
}
}  // namespace hs

__HS_ATTRIBUTE_VISIBILITY_HIDDEN void InitMainThread(
    hs::svc::Handle thread_handle) noexcept {
    // Construct the thread list
    auto thread_list = hs::os::detail::ThreadList::thread_list.GetPointer();
    new (thread_list) hs::os::detail::ThreadList(thread_handle);

    // Setup the current thread context
    auto tls_storage = hs::os::ThreadLocalStorage::GetThreadLocalStorage();
    tls_storage->SetThreadContext(&thread_list->GetMainThread());
}

extern"C" void hsMain(void);

void hs::init::Start(uint64_t thread_handle, uintptr_t argument_address,
                     void (*notify_exception_handler_ready)(),
                     void (*call_initializator)()) {
    // TODO(Kaenbyō): handle argv & argc
    __HS_IGNORE_ARGUMENT(argument_address);
    // TODO(Kaenbyō): activate exception handler when implemented.
    __HS_IGNORE_ARGUMENT(notify_exception_handler_ready);
    __HS_DEBUG_LOG("Starting Hydrosphere (build: " __DATE__ " " __TIME__ ")");

    // Init main thread
    InitMainThread(svc::Handle::FromRawValue(thread_handle));

    __HS_DEBUG_LOG("Initializing virtual memory allocators");

    // Init virtual memory allocators
    hs::os::detail::InitializeVirtualMemoryAllocators();

    // Ask rtld to call init fo all modules?
    call_initializator();

    // TODO(Kaenbyō): More init here.

    hsMain();

    hs::svc::ExitProcess();
    hs::svc::Break(0, 0, 0);
}
