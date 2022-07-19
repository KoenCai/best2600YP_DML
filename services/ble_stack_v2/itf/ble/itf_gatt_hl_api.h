/**
 * @file itf_gatt_hl_api.h
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

#ifndef GATT_HL_API_H_
#define GATT_HL_API_H_

/**
 ****************************************************************************************
 * @addtogroup GATT
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "itf_rwip_config.h"
#if (BLE_GATT)
#include <stdint.h>

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Create and Initialize the GATT module.
 *
 * @param[in] init_type  Type of initialization (@see enum rwip_init_type)
 ****************************************************************************************
 */
void gatt_init(uint8_t init_type);

/**
 ****************************************************************************************
 * @brief Handle BLE link creation
 *
 * @param[in] conidx        Connection Index
 ****************************************************************************************
 */
void gatt_create(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Handle BLE link disconnection
 *
 * @param[in] conidx        Connection Index
 ****************************************************************************************
 */
void gatt_cleanup(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Setup bearer for EATT registration
 ****************************************************************************************
 */
uint16_t gatt_bearer_setup(void);

/**
 ****************************************************************************************
 * @brief Initiate establishment of EATT bearers
 *
 * @param[in] conidx        Connection Index
 ****************************************************************************************
 */
void gatt_bearer_eatt_estab(uint8_t conidx);

#if (BLE_GATT_CLI)
/**
 ****************************************************************************************
 * @brief Inform GATT client users that peer database has changed
 *
 * @param[in] conidx        Connection Index
 * @param[in] out_of_sync   True if an out of sync error has been received
 * @param[in] start_hdl     Service change start handle
 * @param[in] end_hdl       Service change end handle
 ****************************************************************************************
 */
void gatt_cli_event_svc_chg(uint8_t conidx, bool out_of_sync,  uint16_t start_hdl, uint16_t end_hdl);
#endif // (BLE_GATT_CLI)

/**
 ****************************************************************************************
 * @brief Provide connection information to GATT module for EATT Collision mitigation timer
 *
 * @param[in] conidx        Connection Index
 * @param[in] con_interval  Connection interval in 1.25ms step
 * @param[in] con_latency   Connection Latency
 ****************************************************************************************
 */
void gatt_con_info_set(uint8_t conidx, uint16_t con_interval, uint16_t con_latency);
#endif // (BLE_GATT)

/// @} GATT

#endif // GATT_HL_API_H_
