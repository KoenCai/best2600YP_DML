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


#ifndef APP_AMSC_H_
#define APP_AMSC_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 *
 * @brief AMSC Application entry point.
 *
 * @{
 ****************************************************************************************
 */


#if BLE_AMS_CLIENT
/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app.h"                     // Application Definitions
#include "app_task.h"                // application task definitions
#include "itf_prf_types.h"
#include "itf_prf_utils.h"
#include "itf_prf.h"
#include "string.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * AMSC Application Functions
 *
 ****************************************************************************************
 */

void app_amsc_add_amsc(void);

/**
 ****************************************************************************************
 * @brief Inialize application and enable AMSC profile.
 *
 ****************************************************************************************
 */
void app_amsc_enable(uint8_t conidx);

#endif //BLE_AMS_CLIENT

/// @} APP

#endif // APP_AMSC_H_
