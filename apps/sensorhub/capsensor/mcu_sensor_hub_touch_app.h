/***************************************************************************
 * Copyright 2015-2021 BES.
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
 ***************************************************************************/
#ifndef __MCU_SENSOR_HUB_TOUCH_APP_H__
#define __MCU_SENSOR_HUB_TOUCH_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* function typedef */
typedef void (*capsensor_received_func_type)(uint8_t *buf, uint16_t len);

/* ===================== Function declaration ===================== */
/* capsensor receive data callback function */
int capsensor_receive_register_callback(capsensor_received_func_type p);

/* mcu send data to sensor hub */
void app_mcu_sensor_hub_send_touch_req_no_rsp(void);

/* capsensor received data process function */
void capsensor_event_process(uint8_t* ptr, uint16_t len);

/* capsensor analog init */
void capsensor_ana_init(void);

#ifdef __cplusplus
}
#endif

#endif  /* end ifdef __MCU_SENSOR_HUB_TOUCH_APP_H__ */