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
#ifndef __SENSOR_HUB_TOUCH_CORE_H__
#define __SENSOR_HUB_TOUCH_CORE_H__
#include "sensor_hub_cap_calculate.h"

#ifdef __cplusplus
extern "C" {
#endif

// touch debug
#define TOUCH_TRACE TRACE

#define SENS_CAP_REG(a)             (*(volatile uint32_t *)(a))

extern struct cap_state_t cap_state[4];

/* function typedef */
typedef void (*capsensor_proccess_func_type)(void *buf, int len);

/* ===================== Local Function declaration ===================== */


/* ===================== Extern Function declaration ===================== */
/* sensor hub send data to mcu */
void app_sensor_hub_core_send_touch_req_no_rsp(uint8_t *ptr, uint16_t len);

/* init capsensor */
void capsensor_reg_init(void);

/* capsensor irq handler */
void cap_sensor_irq_handler(void);

/* capsensor irq init */
void cap_sensor_irqinit(void);

/* capsensor dma init */
void capsensor_dma_init(uint16_t size);

/* capsensor process function */
int capsensor_process(void* data, int len);

/* app_sensor_hub_core_touch_init */
void app_sensor_hub_core_touch_init(void);

/* capsensor_process_register_callback */
int capsensor_process_register_callback(capsensor_proccess_func_type p);

/* dump cap sensor data */
void check_mem_data(void* data, int len);

/* Set the offset value of the differential channel */
void set_offset_value(uint16_t offset_0, uint16_t offset_1);

/* Set the threshold value of the differential channel */
void set_threshold_value(uint16_t th_high_ch0, uint16_t th_high_ch1, uint16_t th_low_ch0, uint16_t th_low_ch1);

#ifdef __cplusplus
}
#endif

#endif /* __SENSOR_HUB_TOUCH_CORE_H__ */
