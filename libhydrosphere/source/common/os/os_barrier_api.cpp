/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#include <hs/os/os_barrier_api.hpp>

enum BarrierState {
    BarrierState_Uninitialized = 0,
    BarrierState_Initialized = 1,
};

namespace hs::os {
void InitializeBarrier(Barrier *barrier, uint32_t number_to_wait) noexcept {
    barrier->arrive_count = 0;
    barrier->number_to_wait = number_to_wait;
    barrier->critical_section = CriticalSection();
    barrier->condition_variable = ConditionVariableImpl();
    barrier->state = BarrierState_Initialized;
}

void AwaitBarrier(Barrier *barrier) noexcept {
    barrier->critical_section.Enter();

    barrier->arrive_count++;

    if (barrier->arrive_count == barrier->number_to_wait) {
        barrier->condition_variable.Broadcast();
    } else {
        barrier->condition_variable.Wait(&barrier->critical_section);
    }

    barrier->critical_section.Leave();
}

void FinalizeBarrier(Barrier *barrier) noexcept {
    barrier->state = BarrierState_Uninitialized;
}

}  // namespace hs::os
