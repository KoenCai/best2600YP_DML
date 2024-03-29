/***************************************************************************
 *
 * Copyright 2015-2019 BES.
 * All rights reserved. All unpublished rights reserved.
 *
 * No part of this work may be used or reproduced in any form or by any
 * means, or stored in a database or retrieval system, without prior written
 * permission of BES.
 *
 * Use of this work is governed by a license granted by BES.
 * This work contains confidential and proprietary information of
 * BES. which is protected by copyright, trade secret,
 * trademark and other intellectual property rights.
 *
 ****************************************************************************/


#ifndef _ANCC_H_
#define _ANCC_H_

/**
 ****************************************************************************************
 * @addtogroup ANCC.
 * @ingroup ANC
 * @brief ANCS - Client Role.
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#if (BLE_ANC_CLIENT)
#include "anc_common.h"
#include "itf_ke_task.h"
#include "itf_prf_types.h"
#include "itf_prf_utils.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximum number of ANCC task instances
#define ANCC_IDX_MAX        (BLE_CONNECTION_MAX)

/// Possible states of the ancc task
enum ancc_states
{
    /// Idle state
    ANCC_FREE,
    /// Connected state
    ANCC_IDLE,
    /// Discovery
    ANCC_DISCOVERING,

    /// Number of defined states.
    ANCC_STATE_MAX
};

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// ANCS Characteristics
enum ancc_anc_chars
{
    /// Notification Source
    ANCC_CHAR_NTF_SRC,
    /// Control Point
    ANCC_CHAR_CTRL_PT,
    /// Data Source
    ANCC_CHAR_DATA_SRC,

    ANCC_CHAR_MAX,
};

/// ANCS Characteristic Descriptors
enum ancc_anc_descs
{
    /// Notification Source - Client Characteristic Configuration
    ANCC_DESC_NTF_SRC_CL_CFG,
    /// Data Source - Client Characteristic Configuration
    ANCC_DESC_DATA_SRC_CL_CFG,

    ANCC_DESC_MAX,

    ANCC_DESC_MASK = 0x10,
};

/// Pointer to the connection clean-up function
#define ANCC_CLEANUP_FNCT        (ancc_cleanup)
/*
 * STRUCTURES
 ****************************************************************************************
 */

/**
 * Structure containing the characteristics handles, value handles and descriptors for
 * the Alert Notification Service
 */
struct ancc_anc_content
{
    /// Service info
    struct prf_svc svc;

    /// Characteristic info:
    ///  - Notification Source
    ///  - Control Point
    ///  - Data Source
    prf_char_t chars[ANCC_CHAR_MAX];

    /// Descriptor handles:
    ///  - Notification Source Client Char Cfg
    ///  - Data Source Client Char Cfg
    prf_desc_t descs[ANCC_DESC_MAX];
};

struct ancc_cnx_env
{
    /// Current Operation
    void *operation;

    /// Provide an indication about the last operation
    uint16_t last_req;

    /// Last characteristic code discovered
    uint8_t last_char_code;

    /// Counter used to check service uniqueness
    uint8_t nb_svc;

    /// ANCS Characteristics
    struct ancc_anc_content anc;
};

/// ANCS Client environment variable
typedef PRF_ENV_TAG(ancc)
{
    /// profile environment
    prf_hdr_t prf_env;

    /// start handle of service
    uint16_t shdl;

    /// to save last write info
    defer_cfm_info_t wrInfo[ANCC_IDX_MAX];

    /// Environment variable pointer for each connections
    struct ancc_cnx_env* env[ANCC_IDX_MAX];

    /// State of different task instances
    ke_state_t state[ANCC_IDX_MAX];

    /// GATT User local index
    uint8_t user_lid;
} PRF_ENV_T(ancc);

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
void ancc_enable_rsp_send(PRF_ENV_T(ancc) *ancc_env, uint8_t conidx, uint8_t status);

#endif //(BLE_ANC_CLIENT)

/// @} ANCC

#endif //(_ANCC_H_)
