/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

// TODO(Kaenbyō): use librtld definitions
__attribute__((section(".bss")))
__attribute__((visibility("hidden")))
char __nx_module_runtime[0xD0];  // 0xD0 is the max size needed by rtld for
                                 // the runtime module

extern "C" void __hydrosphere_module_marker(void);

extern "C" __attribute__((used)) void __hydrosphere_module_marker_shim() {
    __hydrosphere_module_marker();
}
