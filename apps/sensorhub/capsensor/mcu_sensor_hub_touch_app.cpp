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
#ifndef CHIP_SUBSYS_SENS
#include "cmsis.h"
#ifdef RTOS
#include "cmsis_os.h"
#endif
#include "hal_trace.h"
#include "hal_timer.h"
#include "hal_sysfreq.h"
#include "app_sensor_hub.h"
#include "sensor_hub.h"
#include "analog.h"
#include "mcu_sensor_hub_app.h"
#include "mcu_sensor_hub_touch_app.h"
#include "apps.h"
#include "app_key.h"
#include "app_voice_assist_wd.h"
#include "app_ibrt_if.h"
// #include "app_tws_ibrt_ui_test.h"

#define OFF_EAR 0
#define ON_EAR  1
#define CORE_BRIDGE_TOUCH_MCU_REQ_DATA       0xA5
#define CORE_BRIDGE_TOUCH_MCU_REQ_RSP_DATA   0x5A

static capsensor_received_func_type capsensor_received_cb = NULL;

struct HAL_KEY_STATUS_T {
    enum HAL_KEY_CODE_T code_down;
    enum HAL_KEY_CODE_T code_ready;
    enum HAL_KEY_CODE_T code_click;
    enum HAL_KEY_EVENT_T event;
    uint32_t time_updown;
    uint32_t time_click;
    uint8_t cnt_repeat;
    uint8_t cnt_click;
};

typedef struct
{
    uint8_t reqData;
} APP_MCU_SENSOR_HUB_TOUCH_REQ_T;

typedef struct
{
    uint8_t rspData;
} APP_MCU_SENSOR_HUB_TOUCH_RSP_T;

extern void app_ibrt_raw_ui_test_key(APP_KEY_STATUS *status, void *param);

static void app_mcu_sensor_hub_transmit_touch_no_rsp_cmd_handler(uint8_t* ptr, uint16_t len)
{
    app_core_bridge_send_data_without_waiting_rsp(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP, ptr, len);
}

static void app_mcu_sensor_hub_touch_no_rsp_cmd_received_handler(uint8_t* ptr, uint16_t len)
{
    TRACE(0, "Get touch no rsp command from sensor hub core:");
    DUMP8("%02x ", ptr, len);

    if (capsensor_received_cb != NULL)
    {
        (*capsensor_received_cb)(ptr, len);
    }
}


static void app_mcu_sensor_hub_touch_no_rsp_cmd_tx_done_handler(uint16_t cmdCode,
    uint8_t* ptr, uint16_t len)
{
    TRACE(0, "cmdCode 0x%x tx done", cmdCode);
}


void app_mcu_sensor_hub_send_touch_req_no_rsp(void)
{
    APP_MCU_SENSOR_HUB_TOUCH_REQ_T req;
    req.reqData = CORE_BRIDGE_TOUCH_MCU_REQ_DATA;
    app_core_bridge_send_cmd(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP,
        (uint8_t *)&req, sizeof(req));
}

CORE_BRIDGE_TASK_COMMAND_TO_ADD(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP,
                                "touch no rsp req to sensor hub core",
                                app_mcu_sensor_hub_transmit_touch_no_rsp_cmd_handler,
                                app_mcu_sensor_hub_touch_no_rsp_cmd_received_handler,
                                0,
                                NULL,
                                NULL,
                                app_mcu_sensor_hub_touch_no_rsp_cmd_tx_done_handler);


void capsensor_ana_init(void)
{
    analog_write(0x17C, 0x0180);
}


/***************************************************************************
 * @brief register key event processing callback function
 * 
 * @param p function pointer
 * @return int ret 0: success; other: failed
 ***************************************************************************/
int capsensor_receive_register_callback(capsensor_received_func_type p)
{
    uint8_t ret = 0;

    if(p == NULL)
    {
        ret = -1;
        return ret;
    }

    capsensor_received_cb = p;

    TRACE(1,"[%s] register receive callback success\n", __func__);

    return ret;
}


/***************************************************************************
 * @brief processing the sensor hub key event
 * 
 * @param ptr pointer of data
 * @param len data len
 ***************************************************************************/
void capsensor_event_process(uint8_t* ptr, uint16_t len)
{

#if 1//def VOICE_ASSIST_WD_ENABLED
    APP_KEY_STATUS status;
    status.code = APP_KEY_CODE_PWR;
#endif

    memcpy(&status, ptr, len);
    
    switch (status.event)
    {
    // case APP_KEY_EVENT_NONE:
    //     TRACE(1, "NULL!");
    //     break;
    case OFF_EAR:
        TRACE(1, "capsensor state= off ear\n");
        status.event = OFF_EAR;
        break;

    case ON_EAR:
        TRACE(1, "capsensor state= on ear\n");
        status.event = ON_EAR;
        break;

    case APP_KEY_EVENT_CLICK:
    #if 1//def VOICE_ASSIST_WD_ENABLED
        status.event = APP_KEY_EVENT_CLICK;
    #else
        extern bool infrasound_fadeout_flag;
        if (infrasound_fadeout_flag == true)
        {
            infrasound_fadeout_flag = false;
        }
        app_voice_assist_wd_open();
    #endif
        break;

    case APP_KEY_EVENT_DOUBLECLICK:
        // app_key_simulate_key_event(APP_KEY_CODE_PWR, key_event);
        app_ibrt_if_a2dp_send_forward(BT_DEVICE_ID_1);
        break;

    case APP_KEY_EVENT_LONGPRESS:
        status.event = APP_KEY_EVENT_LONGPRESS;
        // app_key_simulate_key_event(APP_KEY_CODE_PWR, key_event);
        break;

    case APP_KEY_EVENT_LONGLONGPRESS:
        status.event = APP_KEY_EVENT_LONGLONGPRESS;
        // app_key_simulate_key_event(APP_KEY_CODE_PWR, key_event);
        break;
        
    default:
        break;
    }

    app_ibrt_raw_ui_test_key(&status, NULL);
}
#endif