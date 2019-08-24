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

#include <hs/os/os_condition_variable_impl.hpp>
#include <hs/os/os_critical_section.hpp>
#include <hs/util/util_template_api.hpp>

namespace hs::os {
/**
 * \defgroup barrier_api Barrier API
 * \short API implementing the barrier synchronization primitive.
 * \ingroup os_api
 * \name Barrier API
 * \addtogroup barrier_api
 * @{
 */

/**
 * \short This is the context of a barrier.
 * 
 * See \ref barrier_api "Barrier API" for usages.
 **/
struct Barrier {
    /**
     * \private
     * \short Internal object state.
     */
    uint8_t state;
    /**
     * \private
     */
    char reserved[3];

    /**
     * \private
     * \short Number of threads needed to release the barrier.
     */
    uint32_t arrive_count;

    /**
     * \private
     * \short Number of threads actually waiting.
     */
    uint32_t number_to_wait;

    /**
     * \private
     * \short The critical section of the barrier.
     */
    CriticalSection critical_section;

    /**
     * \private
     * \short The condition variable of the barrier.
     */
    ConditionVariableImpl condition_variable;
};

static_assert(hs::util::is_pod<Barrier>::value, "Barrier isn't pod");

/**
 * \short Initialize a Barrier for \c num_to_wait participating threads.
 *
 * \remark The set of participating threads is the first \c num_to_wait threads to arrive at the synchronization point.
 *
 * \param[in] barrier A pointer to a Barrier.
 * \param[in] number_to_wait The number of threads this barrier need to wait on.
 * 
 * \pre ``number_to_wait`` > 0
 * \pre ``barrier`` is uninitialized.
 * \post ``barrier`` is initialized.
 */
void InitializeBarrier(Barrier *barrier, uint32_t number_to_wait) noexcept;

/**
 * \short Blocks and arrive at the barrier's synchronization point.
 *
 * \param[in] barrier A pointer to a Barrier.
 * 
 * \pre ``barrier`` is initialized.
 * \post The thread arrived at the barrier's synchronization point.
 */
void AwaitBarrier(Barrier *barrier) noexcept;

/**
 * \short Finalize a Barrier.
 *
 * \param[in] barrier A pointer to a Barrier.
 * 
 * \pre ``barrier`` is initialized.
 * \post ``barrier`` is uninitialized.
 */
void FinalizeBarrier(Barrier *barrier) noexcept;

/**
 * @}
 */

}  // namespace hs::os
