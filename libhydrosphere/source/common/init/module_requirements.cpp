/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/hs_config.hpp>
#include <hs/hs_macro.hpp>
#include <hs/svc.hpp>

#define MODULE_NAME "hydrosphere"
#define MODULE_NAME_LEN 11

struct ModuleName {
    int unknown;
    int name_lengh;
    char name[MODULE_NAME_LEN + 1];
};

__HS_ATTRIBUTE_SECTION(".rodata.module_name")
__HS_ATTRIBUTE_VISIBILITY_HIDDEN ModuleName module_name = {
    .unknown = 0, .name_lengh = MODULE_NAME_LEN, .name = MODULE_NAME};

// TODO(Kaenbyō): use them
extern "C" __HS_ATTRIBUTE_VISIBILITY_HIDDEN void _init(void) {}

extern "C" __HS_ATTRIBUTE_VISIBILITY_HIDDEN void _fini(void) {}
