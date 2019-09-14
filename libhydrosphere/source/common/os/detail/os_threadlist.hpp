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

#include <hs/hs_macro.hpp>
#include <hs/os/os_critical_section.hpp>
#include <hs/os/os_thread_api.hpp>
#include <hs/svc/svc_types.hpp>
#include <hs/util/util_intrusive_list.hpp>
#include <hs/util/util_object_storage.hpp>

namespace hs::os::detail {
class ThreadList {
 private:
    hs::os::CriticalSection critical_section;
    hs::util::IntrusiveList<hs::os::Thread> list;
    hs::os::Thread main_thread;

 public:
    explicit ThreadList(hs::svc::Handle thread_handle)
        : critical_section(), list(), main_thread() {
        main_thread.thread_handle = thread_handle;
        hs::os::SetThreadName(&main_thread, "MainThread");
        list.push_back(main_thread);
    }
    __HS_DISALLOW_COPY(ThreadList);
    __HS_DISALLOW_ASSIGN(ThreadList);

    void AddThread(Thread &thread) noexcept {
        critical_section.Enter();
        list.push_back(thread);
        critical_section.Leave();
    }

    void RemoveThread(Thread &thread) noexcept {
        // The main thread must not be removed!
        if (&thread == &main_thread) {
            return;
        }

        critical_section.Enter();
        thread.Unlink();
        critical_section.Leave();
    }

    hs::os::Thread &GetMainThread() noexcept { return main_thread; }

    hs::util::IntrusiveList<hs::os::Thread> &Aquire() {
        critical_section.Enter();
        return list;
    }

    void Release() { critical_section.Leave(); }

    static ThreadList &Get() { return *thread_list; }

    static hs::util::ObjectStorage<ThreadList> thread_list;
};
}  // namespace hs::os::detail
