/**
 * @file itf_prf.h
 * @author BES AI team
 * @version 0.1
 * @date 2021-05-07
 * 
 * @copyright Copyright (c) 2015-2021 BES Technic.
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
 */

#ifndef _PRF_H_
#define _PRF_H_

/**
 ****************************************************************************************
 * @addtogroup PROFILE PROFILES
 * @ingroup ROOT
 * @brief Bluetooth Low Energy Host Profiles
 *
 * The PROFILE of the stack contains the profile layers (@ref PROX "PROXIMITY",
 * @ref HTP "HTP",@ref FIND "FIND ME" @ref BPS "Blood Pressure").
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup PRF
 * @ingroup Profile
 * @brief Definitions of Profile management - Native API
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "itf_gapm_msg.h"

/*
 * DEFINES
 ****************************************************************************************
 */
#define PRF_SVC_DESC_CLI_CFG_LEN           (2)

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * MACROS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief This function is used to instantiate a profile (service or client) into SW.
 *        This function allocates profile attribute database and required environment variables.
 *
 *        Profile must be instantiated after execution of @see GAPM_SET_DEV_CONFIG_CMD.
 *        Parameter field should be set according to profile settings which are described
 *        in corresponding profile interface specifications.
 *
 *        @note Allocated profiles are freed if a @see GAPM_RESET_CMD or
 *              @see GAPM_SET_DEV_CONFIG_CMD commands are executed.
 *
 * @param[in]     api_id       Profile API identifier (@see enum TASK_API_ID)
 * @param[in]     sec_lvl      Security level (@see enum gatt_svc_info_bf)
 * @param[in]     user_prio    GATT User priority
 * @param[in]     p_param      Configuration parameters of profile collector or service (32 bits aligned)
 * @param[in]     p_cb         Callback structure that handles event from profile.
 * @param[in|out] p_start_hdl  Pointer to Service start handle (0 - dynamically allocated), only applies for services.
 *
 * @return status of adding profile task
 ****************************************************************************************
 */
uint16_t prf_add_profile(uint8_t api_id, uint8_t sec_lvl, uint8_t user_prio, const void* p_params, const void* p_cb,
                         uint16_t* p_start_hdl);

uint8_t prf_get_task_from_id(uint8_t api_id);


/// @} PRF

#endif /* _PRF_H_ */
