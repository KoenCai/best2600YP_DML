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
#include "string.h"
#include "cmsis.h"
#ifdef RTOS
#include "cmsis_os.h"
#endif
#include "hal_cmu.h"
#include "hal_dma.h"
#include "hal_iomux.h"
#include "hal_mcu2sens.h"

#include "analog.h"
#include "hal_trace.h"
#include "hal_timer.h"
#include "hal_sysfreq.h"
#include "hwtimer_list.h"

// #include "audioflinger.h"
#include "sens_msg.h"
#include "sensor_hub_core.h"
#include "sensor_hub_core_app.h"
#include "sensor_hub_touch_core.h"
#include "app_sensor_hub.h"
#include "app_thread.h"

#include <math.h>
#include <string.h>

#include "hal_iomux.h"
#include "hal_gpio.h"
#include "app_key.h"
#include "sensor_hub_cap_calculate.h"
#include "hal_capsensor_best1501.h"

// Global variables
static void capsensor_thread(const void *arg);
osThreadDef(capsensor_thread, osPriorityBelowNormal, 1, (1024 * 2), "capsensor_thread");
static osThreadId capsensor_thread_id = NULL;

#if 0
struct cap_state_t cap_state[4] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0}
};
#endif

static struct HAL_DMA_DESC_T capsensor_dma_desc;
static uint8_t g_dma_channel = HAL_DMA_CHAN_NONE;

static struct HAL_DMA_CH_CFG_T dma_cfg;
static uint16_t data[16];

static capsensor_proccess_func_type capsensor_process_cb = NULL;

/* no response command start */
/***************************************************************************
 * @brief
 *
 * @param ptr : message pointer.
 * @param len : message lengths.
 ***************************************************************************/
static void app_sensor_hub_core_transmit_touch_no_rsp_cmd_handler(uint8_t* ptr, uint16_t len)
{
    app_core_bridge_send_data_without_waiting_rsp(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP, ptr, len);
}


/***************************************************************************
 * @brief
 *
 * @param ptr : message pointer.
 * @param len : message lengths.
 ***************************************************************************/
static void app_sensor_hub_core_touch_no_rsp_cmd_received_handler(uint8_t* ptr, uint16_t len)
{
    TOUCH_TRACE(0, "Get touch no rsp command from mcu:");
    DUMP8("%02x ", ptr, len);
}

/***************************************************************************
 * @brief
 *
 * @param cmdCode
 * @param ptr : message pointer.
 * @param len : message lengths.
 ***************************************************************************/
static void app_sensor_hub_core_touch_no_rsp_cmd_tx_done_handler(uint16_t cmdCode,
    uint8_t* ptr, uint16_t len)
{
    TOUCH_TRACE(0, "cmdCode 0x%x tx done", cmdCode);
}

/***************************************************************************
 * @brief
 *
 * @param ptr : message pointer.
 * @param len : message lengths.
 ***************************************************************************/
void app_sensor_hub_core_send_touch_req_no_rsp(uint8_t *ptr, uint16_t len)
{
    // APP_SENSOR_HUB_CORE_TOUCH_REQ_T req;
    // req.reqData = CORE_BRIDGE_TOUCH_REQ_DATA;
    // app_core_bridge_send_cmd(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP,
        // (uint8_t *)&req, sizeof(req));
    app_core_bridge_send_cmd(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP,
        ptr, len);
}

CORE_BRIDGE_TASK_COMMAND_TO_ADD(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP,
                                "touch no rsp req to mcu",
                                app_sensor_hub_core_transmit_touch_no_rsp_cmd_handler,
                                app_sensor_hub_core_touch_no_rsp_cmd_received_handler,
                                0,
                                NULL,
                                NULL,
                                app_sensor_hub_core_touch_no_rsp_cmd_tx_done_handler);
/* no response command end */

/***************************************************************************
 * @brief An instant message request from the sensor hub to the MCU.
 *
 * @param ptr : message pointer.
 * @param len : message lengths.
 ***************************************************************************/
static void app_sensor_hub_core_transmit_touch_instant_req_handler(uint8_t* ptr, uint16_t len)
{
    app_core_bridge_send_instant_cmd_data(MCU_SENSOR_HUB_INSTANT_CMD_TOUCH_REQ,
        ptr, len);
}


/***************************************************************************
 * @brief An instant message request from the MCU to the sensor hub.
 *
 * @param ptr : message pointer.
 * @param len : message lengths.
 ***************************************************************************/
static void app_sensor_hub_core_touch_instant_req_handler(uint8_t* ptr, uint16_t len)
{
    // for test purpose, we add log print here.
    // but as instant cmd handler will be directly called in intersys irq context,
    // for realistic use, should never do log print
    TOUCH_TRACE(0, "Get demo instant req command from mcu:");
    DUMP8("%02x ", ptr, len);
}

CORE_BRIDGE_INSTANT_COMMAND_TO_ADD(MCU_SENSOR_HUB_INSTANT_CMD_TOUCH_REQ,
                                app_sensor_hub_core_transmit_touch_instant_req_handler,
                                app_sensor_hub_core_touch_instant_req_handler);



/***************************************************************************
 * @brief cap sensor register configuration function
 *
 ***************************************************************************/
void capsensor_reg_init(void)
// void capsensor_reg_init(const struct CAPSENSOR_CFG_T *cfg)
{
    hal_senscmu_capsensor_set();
    hal_capsensor_setup();
}


/***************************************************************************
 * @brief dump cap data function
 *
 * @param data pointer of data
 * @param len data len
 ***************************************************************************/
void check_mem_data(void* data, int len)
{
    short* share_mem = (short*)data;

    int32_t i =0;
    int32_t remain = len;

    // TRACE(0,"-----------------------------AD data start-------------------------------");
    while(remain > 0)
    {
        for(i=0; i<32; i++)//output two line
        {
            if (remain >16)
            {
                DUMP16("%04X ",share_mem,16);
                share_mem +=16;
                remain -= 16;
            }
            else
            {
                DUMP16("%04X ",share_mem,remain);
                remain =0;

                // TRACE(0,"-----------------------------AD data end-------------------------------");
              return;
            }
        }

        hal_sys_timer_delay(MS_TO_TICKS(100));
    }
}


/***************************************************************************
 * @brief cap sensor interrupt handler
 *
 ***************************************************************************/
void cap_sensor_irq_handler(void)
{
    enum HAL_DMA_RET_T ret = HAL_DMA_ERR;

    hal_sysfreq_req(HAL_SYSFREQ_USER_INIT, HAL_CMU_FREQ_26M);
    hal_capsensor_irq_disable();


    ret = hal_gpdma_sg_start(&capsensor_dma_desc, &dma_cfg);
    if (ret != HAL_DMA_OK) {
        ASSERT(0, "%s hal_gpdma_sg_start failed.", __func__);
    }

    osSignalSet(capsensor_thread_id, 0x0001);
}


/***************************************************************************
 * @brief cap sensor interrupt init function
 *
 ***************************************************************************/
void cap_sensor_irqinit(void)
{
    NVIC_SetVector(CAP_SENSOR_IRQn, (uint32_t)cap_sensor_irq_handler);
    NVIC_SetPriority(CAP_SENSOR_IRQn, IRQ_PRIORITY_NORMAL);
    NVIC_ClearPendingIRQ(CAP_SENSOR_IRQn);
    NVIC_EnableIRQ(CAP_SENSOR_IRQn);
    hal_capsensor_irq_enable();
}

/***************************************************************************
 * @brief cap sensor dma handler
 *
 * @param remains
 * @param error
 * @param lli
 ***************************************************************************/
static void capsensor_dma_handler(uint32_t remains, uint32_t error, struct HAL_DMA_DESC_T *lli)
{
    if(g_dma_channel != HAL_DMA_CHAN_NONE)
    {
        // hal_gpdma_free_chan(g_dma_channel);
    }

    hal_sysfreq_req(HAL_SYSFREQ_USER_INIT, HAL_CMU_FREQ_32K);
}

/***************************************************************************
 * @brief cap sensor dma init
 *
 * @param dma_dst_data
 * @param size
 ***************************************************************************/
void capsensor_dma_init(uint16_t size)
{
    enum HAL_DMA_RET_T ret;

    memset(&dma_cfg, 0, sizeof(dma_cfg));

    dma_cfg.dst_bsize = HAL_DMA_BSIZE_16;
    dma_cfg.dst_periph = 0;
    dma_cfg.dst_width = HAL_DMA_WIDTH_HALFWORD;
    dma_cfg.handler = (HAL_DMA_IRQ_HANDLER_T)capsensor_dma_handler;
    dma_cfg.src = 0; // useless
    dma_cfg.src_bsize = HAL_DMA_BSIZE_16;

    dma_cfg.src_periph = HAL_AUDMA_CAP_SENS;
    dma_cfg.src_tsize = size; //1600; //1600*2/26=123us
    dma_cfg.src_width = HAL_DMA_WIDTH_HALFWORD;
    dma_cfg.try_burst = 1;
    dma_cfg.type = HAL_DMA_FLOW_P2M_DMA;
    dma_cfg.dst        = (uintptr_t)(data);

    dma_cfg.ch = hal_dma_get_chan(HAL_AUDMA_CAP_SENS, HAL_DMA_HIGH_PRIO);
    if(dma_cfg.ch== HAL_DMA_CHAN_NONE) {
        ASSERT(0, "%s hal_dma_get_chan failed.", __func__);
        return;
    }
    g_dma_channel = dma_cfg.ch;

    ret = hal_gpdma_init_desc(&capsensor_dma_desc, &dma_cfg, 0 ,1);
    if (ret != HAL_DMA_OK) {
        ASSERT(0, "%s hal_gpdma_init_desc failed.", __func__);
        return;
    }
}


static uint8_t ear_state                   = 0;
static int16_t samp_diff[USED_NUM]         = {0};
static int16_t threshold_high[USED_NUM]    = {THRESHOLD_HIGH, THRESHOLD_HIGH};
static int16_t threshold_low[USED_NUM]     = {THRESHOLD_LOW, THRESHOLD_LOW};
static uint8_t channels_used[USED_NUM * 2] = {0, 1, 2, 3};

static int16_t offset[USED_NUM]            = {0};
static bool calculate_flag                 = true;

#if 0
struct CAPSENSOR_CHANNELS_CFG_T channels_cfg[CHNUM] = {
    {
    .work_mode   = WEAR_MODE,//TOUCH_MODE,  //
    .channels    = 0,
    .chan_mode   = SINGLE_END,
    .sample_freq = 200
    },
    {
    .work_mode   = TOUCH_MODE,  //WEAR_MODE,//
    .channels    = 1,
    .chan_mode   = SINGLE_END,
    .sample_freq = 200
    },
    {
    .work_mode   = TOUCH_MODE, //WEAR_MODE,   //
    .channels    = 2,
    .chan_mode   = SINGLE_END,
    .sample_freq = 200
    },
    {
    .work_mode   = TOUCH_MODE,//WEAR_MODE, //
    .channels    = 3,
    .chan_mode   = SINGLE_END,
    .sample_freq = 200
    }
};
#endif

static bool calculate_offset_value(uint16_t *data, bool flag, uint8_t count)
{
    bool ret_val = false;
    bool cal_flag = false;          // calculate flag.
    static uint8_t cal_count = 0;   // calculate count.
    uint8_t cal_count_max = count;  // calculate count max.
    int16_t offset_val[USED_NUM] = {0};
    static int16_t offset_val_sum[USED_NUM] = {0};
    cal_flag = flag;

    for (uint8_t i = 0; i < USED_NUM; i++) {
        offset_val[i] = data[channels_used[i * 2 + 1]] - data[channels_used[i * 2]];

        // calculate offset value.
        if (cal_flag == true) {
            cal_count++;
            offset_val_sum[i] += offset_val[i];

            if (cal_count > cal_count_max) {
                offset[i - 1] = offset_val_sum[i - 1] / (cal_count  / 2);
                offset[i]     = offset_val_sum[i] / (cal_count / 2);
                cal_count = 0;
                cal_flag  = false;
                ret_val   = true;
                TRACE(0, "offset[i - 1]=%d,  offset[i]=%d",  offset[i - 1],  offset[i]);
            }
        }
    }

    return ret_val;
}


void set_offset_value(uint16_t offset_0, uint16_t offset_1)
{
    offset[0] = offset_0;
    offset[1] = offset_1;
}


void set_threshold_value(uint16_t th_high_ch0, uint16_t th_high_ch1, uint16_t th_low_ch0, uint16_t th_low_ch1)
{
    threshold_high[0] = th_high_ch0;
    threshold_high[1] = th_high_ch1;
    threshold_low[0]  = th_low_ch0;
    threshold_low[1]  = th_low_ch1;
}

/***************************************************************************
 * @brief cap sensor data processing function
 *
 * @param data pointer of data
 * @param len data len
 * @return int none
 ***************************************************************************/
int capsensor_process(void* data, int len)
{
    short *datai = data;
    uint8_t chN[CHNUM * REPNUM];
    uint16_t adcin[CHNUM * REPNUM];
    uint16_t chan_data_sum[CHNUM];
    uint8_t dataL = len;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t ch_id_min = 8;
    uint8_t ch_id_max = 0;
    uint8_t ear_on_count = 0;
    bool ret_val;
#if 0
    uint16_t samp1;
    double dc;
    uint8_t key     = 0;
    uint8_t keydown = 0;
    uint8_t counti  = 0;
#endif

    APP_KEY_STATUS status;

    for(i = 0; i < dataL; i++) {
        adcin[i] = datai[i] & 0xfff;            // 12bits[0:11] adc data.
        chN[i]   = (datai[i] & 0x7000) >> 12;    // 3bits[12:14] channel number.
    }

    ch_id_min = chN[0];
    ch_id_max = chN[0];
    for(i = 0; i < dataL; i++) {
        if(chN[i] < ch_id_min) {
            ch_id_min = chN[i];
        }

        if(chN[i] > ch_id_max) {
            ch_id_max = chN[i];
        }
    }

    /* calculate data sum of each channels. */
    for (i = 0; i < CHNUM; i++) {
        chan_data_sum[i] = 0;   // clear channel i data sum.

        // Number of samples per channel.
        for (j = 0; j < REPNUM; j++) {
            chan_data_sum[i] = chan_data_sum[i] + adcin[i * REPNUM + j];
        }
    }

    if (calculate_flag) {
        ret_val = calculate_offset_value(chan_data_sum, calculate_flag, 5);
        if (!ret_val) {
            return 0;
        }

        calculate_flag = false;
    }

    for (uint8_t i = 0; i < USED_NUM; i++) {
        samp_diff[i] = chan_data_sum[channels_used[i * 2 + 1]] - chan_data_sum[channels_used[i * 2]];
        status.code = APP_KEY_CODE_FN10;

        if (fabs(samp_diff[i] - offset[i]) >= threshold_high[i]) {
            if (!ear_state) {
                ear_on_count++;
                if (ear_on_count >= 2) {
                    ear_on_count = 0;
                    status.event = ear_state = ON_EAR;
                    app_core_bridge_send_cmd(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP, (uint8_t *)&status, sizeof(APP_KEY_STATUS));
                    TRACE(1, "channel=%d samp_diff=%d ear_state=%d on", channels_used[i], samp_diff[i], ear_state);
                }
            }
        }else if (fabs(samp_diff[i] - offset[i]) < threshold_low[i]) {
            if (ear_state) {
                status.event = ear_state = OFF_EAR;
                app_core_bridge_send_cmd(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP, (uint8_t *)&status, sizeof(APP_KEY_STATUS));
                TRACE(1, "channel=%d samp_diff=%d ear_state=%d off", channels_used[i], samp_diff[i], ear_state);
            }
        }

        // TRACE(1, "channel=%d samp_diff=%d", channels_used[i], samp_diff[i] - offset[i]);
#if 0
        samp1 = chan_data_sum[channels_used[i]];

        if (channels_cfg[channels_used[i]].work_mode == TOUCH_MODE) // touch mode.
        {
            dc = dc_value(samp1);
            if((keydown == APP_KEY_EVENT_CLICK) || (keydown == APP_KEY_EVENT_UP))
            {
                counti++;
                if(counti >= (FS / 100 - 1))
                {
                    keydown = 0;
                    counti  = 0;
                }
                TRACE(1, "counti =%d", counti);
            }
            else
            {
                key = key_read(samp1, dc, &keydown, channels_used[i]);
            }

            if(key > APP_KEY_EVENT_NONE)
            {
                status.code = APP_KEY_CODE_PWR;
                switch (key)
                {
                    case APP_KEY_EVENT_CLICK:
                        status.event = key;
                        TRACE(1, "detected channel=%d presskey=%d is signle click ", channels_used[i], status.event);
                        break;

                    case APP_KEY_EVENT_DOUBLECLICK:
                        status.event = key;
                        TRACE(1, "detected channel=%d presskey=%d is double click ", channels_used[i], status.event);
                        break;

                    case APP_KEY_EVENT_LONGPRESS:
                        status.event = key;
                        TRACE(1, "detected channel=%d presskey=%d is long click ", channels_used[i], status.event);
                        break;

                    case APP_KEY_EVENT_LONGLONGPRESS:
                        status.event = key;
                        TRACE(1, "detected channel=%d presskey=%d is longlong click ", channels_used[i], status.event);
                    break;

                    default:
                        TRACE(0, "invalid key =%d", key);
                        key = APP_KEY_EVENT_NONE;
                        break;
                }

                if (key != APP_KEY_EVENT_NONE;) {
                    app_core_bridge_send_cmd(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP, (uint8_t *)&status, sizeof(APP_KEY_STATUS));
                }
            }
            keydown = APP_KEY_EVENT_NONE;
            key = APP_KEY_EVENT_NONE;
        }
        else if (channels_cfg[channels_used[i]].work_mode == WEAR_MODE) // wear mode.
        {
            cap_state[channels_used[i]].pre_state = cap_state[channels_used[i]].cur_state;
            edge_detect1(samp1, &cap_state[channels_used[i]].cur_state, channels_used[i]);


            if (cap_state[channels_used[i]].pre_state != cap_state[channels_used[i]].cur_state)
            {
                status.code = APP_KEY_CODE_FN10;
                if(cap_state[channels_used[i]].cur_state == ON_EAR)
                {
                    status.event = cap_state[channels_used[i]].cur_state;
                    TRACE(1, "detected channel=%d  state= ON EAR\n", channels_used[i]);
                }
                if(cap_state[channels_used[i]].cur_state == OFF_EAR)
                {
                    status.event = cap_state[channels_used[i]].cur_state;
                    TRACE(1, "detected channel=%d state= OFF EAR\n", channels_used[i]);
                }

                app_core_bridge_send_cmd(MCU_SENSOR_HUB_TASK_CMD_TOUCH_REQ_NO_RSP, (uint8_t *)&status, sizeof(APP_KEY_STATUS));
            }
        }
#endif
    }
    /* end of new code */

    return 0;
}


/***************************************************************************
 * @brief register cap data processing callback function
 *
 * @param p function pointer
 * @return int ret 0: success; other: failed
 ***************************************************************************/
int capsensor_process_register_callback(capsensor_proccess_func_type p)
{
    uint8_t ret = 0;

    if(p == NULL)
    {
        ret = -1;
        return ret;
    }

    capsensor_process_cb = p;

    TRACE(1,"[%s] register process callback success\n", __func__);

    return ret;
}


/***************************************************************************
 * @brief cap sensor process thread
 *
 * @param arg
 ***************************************************************************/
static void capsensor_thread(const void *arg)
{
    /* init cap sensor regesiter. */
    capsensor_reg_init();

    /* init capsensor dma */
    capsensor_dma_init(8);

    /* init cap sensor irq */
    cap_sensor_irqinit();

    /* trace sensor mcu freq */
    TOUCH_TRACE(0, "sensor MCU freq=%d", hal_sys_timer_calc_cpu_freq(5,0));

    while(1) {
        osEvent evt;

        evt = osSignalWait(0, osWaitForever);
        if (evt.status == osEventSignal)
        {
            if (evt.value.signals == 0x01)
            {
                if (capsensor_process_cb != NULL)
                {
                    capsensor_process_cb(data, 8);
                }
                else
                {
                    capsensor_process(data, 8);
                    // check_mem_data((uint8_t *)data, 8);
#if 0
                    if (!dma_cycle_cnt)
                    {
                        odd_count++;
                    }
                    else
                    {
                        even_count++;
                    }

                    if (odd_count >= 5)
                    {
                        odd_count = 0;
                        capsensor_process(data[dma_cycle_cnt], 16);
                        // check_mem_data((uint8_t *)data[dma_cycle_cnt], 16);
                    }

                    if (even_count >= 5)
                    {
                        even_count = 0;
                        capsensor_process(data[dma_cycle_cnt], 16);
                        // check_mem_data((uint8_t *)data[dma_cycle_cnt], 16);
                    }
#endif
                }
            }
        }
    }
}


/***************************************************************************
 * @brief init snesorhub touch key functions.
 *
 ***************************************************************************/
void app_sensor_hub_core_touch_init(void)
{
    TOUCH_TRACE(1, "%s", __func__);

    capsensor_thread_id = osThreadCreate(osThread(capsensor_thread), NULL);
}
