/**
 ****************************************************************************************
 *
 * @file ke_env.h
 *
 * @brief This file contains the definition of the kernel.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef _ITF_KE_ENV_H_
#define _ITF_KE_ENV_H_

/**
 ****************************************************************************************
 * @addtogroup ENV Environment
 * @ingroup KERNEL
 * @brief Kernel Environment
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "itf_rwip_config.h"          // stack configuration
#include "itf_ke_event.h"             // kernel event
#include "itf_ble_define.h"

// forward declaration
struct mblock_free;

/// Kernel environment definition
struct ke_env_tag
{
    /// Queue of sent messages but not yet delivered to receiver
    struct co_list queue_sent;
    /// Queue of messages delivered but not consumed by receiver
    struct co_list queue_saved;
    /// Queue of timers
    struct co_list queue_timer;
    /// Root pointer = pointer to first element of heap linked lists
    struct mblock_free * heap[KE_MEM_BLOCK_MAX];
    /// Size of heaps
    uint16_t heap_size[KE_MEM_BLOCK_MAX];

    #if (KE_PROFILING)
    /// Size of heap used
    uint16_t heap_used[KE_MEM_BLOCK_MAX];
    /// Maximum heap memory used
    uint32_t max_heap_used;
    #endif //KE_PROFILING
};

/// Kernel environment
extern struct ke_env_tag ke_env;

/// @} ENV

#endif // _ITF_KE_ENV_H_
