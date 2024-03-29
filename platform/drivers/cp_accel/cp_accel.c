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
#ifdef CHIP_HAS_CP

#include "cp_accel.h"
#include "cmsis.h"
#include "cmsis_os.h"
#include "cmsis_nvic.h"
#include "hal_cmu.h"
#include "hal_location.h"
#include "hal_mcu2cp.h"
#include "hal_sleep.h"
#include "hal_timer.h"
#include "hal_trace.h"
#include "hal_sysfreq.h"
#include "mpu.h"
#include "stdarg.h"
#include "system_cp.h"
#include "hal_memsc.h"
#include "string.h"
#include "app_utils.h"

#ifdef CP_ACCEL_DEBUG
#define CP_ACCEL_TRACE(s, ...)              TRACE(s, ##__VA_ARGS__)
#else
#define CP_ACCEL_TRACE(s, ...)
#endif

#define CP_NO_FLASH_ACCESS

#define CP_CRASH_START_TIMEOUT              MS_TO_TICKS(100)
#define CP_TRACE_FLUSH_TIMEOUT              MS_TO_TICKS(200)
#define CP_CRASH_DUMP_TIMEOUT               MS_TO_TICKS(500)
#define CP_TRACE_BUF_FULL_INTVL             MS_TO_TICKS(50)

enum CP_SYS_EVENT_T {
    CP_SYS_EVENT_NONE = 0,
    CP_SYS_EVENT_CRASH_START,
    CP_SYS_EVENT_CRASH_END,
    CP_SYS_EVENT_TRACE_FLUSH,
    CP_SYS_EVENT_TRACE_BUF_FULL,
};

static bool ram_inited = false;
static bool cp_accel_inited = false;
static struct cp_task_env_tag cp_task_env;
static CP_BSS_LOC volatile struct cp_env_tag cp_env;

static CP_BSS_LOC volatile enum CP_SYS_EVENT_T cp_sys_evt;
static CP_BSS_LOC bool cp_in_crash;
static CP_BSS_LOC volatile uint8_t cp_in_sleep;
static CP_BSS_LOC uint32_t cp_buf_full_time;
static CP_BSS_LOC uint8_t req_event = 0, pending_event = 0;


#if (CP_CPU_USAGE_INTVL_MS > 0)
static CP_BSS_LOC enum HAL_CMU_FREQ_T stats_freq;
static CP_BSS_LOC uint32_t stats_interval;

static CP_BSS_LOC uint16_t stats_converted_freq_mhz;
static CP_BSS_LOC uint32_t stats_accumulated_mips;
static CP_BSS_LOC uint32_t stats_average_mips;
static CP_BSS_LOC uint32_t stats_start_time_of_current_freq;
static CP_BSS_LOC uint32_t sleep_time_of_current_freq;
static CP_BSS_LOC bool     stats_is_freq_updated = false;
static CP_BSS_LOC uint32_t stats_start_time;
static CP_BSS_LOC uint32_t sleep_time;
#if (CP_CPU_USAGE_TRACE_INTVL_MS > 0)
static CP_BSS_LOC uint32_t stats_trace_interval;
static CP_BSS_LOC uint32_t stats_trace_time;;
#endif
static uint8_t sleep_ratio;
#endif

extern uint32_t __StackTop[];
extern uint32_t __cp_stack_top[];
extern uint32_t __cp_stack_limit[];

static CP_TEXT_SRAM_LOC int send_sys_ctrl_cp2mcu(uint32_t event)
{
    return hal_mcu2cp_send_cp(HAL_MCU2CP_ID_1, HAL_MCU2CP_MSG_TYPE_0, (unsigned char *)event, 0);
}

static CP_TEXT_SRAM_LOC void cp_trace_crash_notify(enum HAL_TRACE_STATE_T state)
{
    uint32_t time;

    if (state == HAL_TRACE_STATE_CRASH_ASSERT_START || state == HAL_TRACE_STATE_CRASH_FAULT_START) {
        cp_in_crash = true;
        cp_sys_evt = CP_SYS_EVENT_CRASH_START;
        mpu_close();
        send_sys_ctrl_cp2mcu(0);

        time = hal_sys_timer_get();
        while (cp_sys_evt == CP_SYS_EVENT_CRASH_START &&
                hal_sys_timer_get() - time < CP_CRASH_START_TIMEOUT);
    } else {
        cp_sys_evt = CP_SYS_EVENT_CRASH_END;
    }
}

static CP_TEXT_SRAM_LOC void cp_trace_buffer_ctrl(enum HAL_TRACE_BUF_STATE_T buf_state)
{
    uint32_t time;

    if (cp_sys_evt != CP_SYS_EVENT_NONE) {
        return;
    }

    time = hal_sys_timer_get();

    if (buf_state == HAL_TRACE_BUF_STATE_FLUSH) {
        cp_sys_evt = CP_SYS_EVENT_TRACE_FLUSH;
        if (!cp_in_crash) {
            send_sys_ctrl_cp2mcu(0);
        }

        while (cp_sys_evt == CP_SYS_EVENT_TRACE_FLUSH &&
                hal_sys_timer_get() - time < CP_TRACE_FLUSH_TIMEOUT);
    } else if (buf_state == HAL_TRACE_BUF_STATE_FULL || buf_state == HAL_TRACE_BUF_STATE_NEAR_FULL) {
        if (time - cp_buf_full_time >= CP_TRACE_BUF_FULL_INTVL) {
            cp_buf_full_time = time;
            if (!cp_in_crash) {
                cp_sys_evt = CP_SYS_EVENT_TRACE_BUF_FULL;
                send_sys_ctrl_cp2mcu(0);
            }
        }
    }
}

static SRAM_TEXT_LOC unsigned int cp2mcu_sys_arrived(const unsigned char *data, unsigned int len)
{
    uint32_t time;
    uint8_t task_id = 0;

    if (cp_sys_evt == CP_SYS_EVENT_TRACE_FLUSH) {
        TRACE_FLUSH();
        cp_sys_evt = CP_SYS_EVENT_NONE;
    } else if (cp_sys_evt == CP_SYS_EVENT_TRACE_BUF_FULL) {
        TRACE(0, " ");
        cp_sys_evt = CP_SYS_EVENT_NONE;
    } else if (cp_sys_evt == CP_SYS_EVENT_CRASH_START) {
        cp_sys_evt = CP_SYS_EVENT_NONE;

        TRACE(0, " ");
        TRACE(0, "CP Crash starts ...");
        UNLOCK_CP_PROCESS(); //Forced release lock

        // Wait CP crash dump finishes in interrupt context
        time = hal_sys_timer_get();
        while (cp_sys_evt != CP_SYS_EVENT_CRASH_END &&
                hal_sys_timer_get() - time < CP_CRASH_DUMP_TIMEOUT) {
            if (cp_sys_evt == CP_SYS_EVENT_TRACE_FLUSH) {
                TRACE_FLUSH();
                cp_sys_evt = CP_SYS_EVENT_NONE;
            }
        }

        for (task_id = 0; task_id < CP_TASK_MAX; task_id++) {
            if (cp_task_env.p_desc[task_id].mcu_sys_ctrl_hdlr) {
                cp_task_env.p_desc[task_id].mcu_sys_ctrl_hdlr(CP_SYS_EVENT_CRASH_END);
            }
        }

        TRACE(0, "CP Crash ends ...");
        TRACE(0, " ");
#ifdef ASSERT_ON_CP_CRASH
        ASSERT(false, "Assert on CP crash");
#endif
    }

    return len;
}

static CP_TEXT_SRAM_LOC unsigned int mcu2cp_msg_arrived(const unsigned char *data, unsigned int len)
{
    uint8_t task_id = CP_TASK_ID_GET(*data);
    uint8_t event_type = CP_EVENT_GET(*data);

    cp_env.cp_msg[task_id][event_type] = 1;
    cp_env.cp_msg_recv = true;

    if (cp_task_env.p_desc[task_id].cp_evt_hdlr) {
        cp_task_env.p_desc[task_id].cp_evt_hdlr((uint32_t)data);
    }

    return len;
}

static CP_TEXT_SRAM_LOC void mcu2cp_msg_sent(const unsigned char *data, unsigned int len)
{
    // TRACE(1, "mcu2cp_msg_sent,pending count = %d", cp_env.mcu2cp_tx_count);

    if (cp_env.mcu2cp_tx_count > 1) {
        cp_env.mcu2cp_tx_count--;
        pending_event = cp_env.mcu2cp_tx_pending[0];
        hal_mcu2cp_send_mcu(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, &pending_event, 1);

        for (uint8_t index = 0;index < cp_env.mcu2cp_tx_count-1;index++) {
            cp_env.mcu2cp_tx_pending[index] = cp_env.mcu2cp_tx_pending[index+1];
        }
    } else {
        cp_env.mcu2cp_tx_count = 0;
    }
}

#if defined(__ARM_ARCH_8M_MAIN__)

#if defined(ROMX_BASE) && (ROMX_BASE != ROM_BASE)
#define CP_CODE_MAP_BASE                    ROMX_BASE
#define CP_ROM_DATA_MAP_BASE                ROM_BASE
#ifdef ROM_EXT_SIZE
#define CP_ROM_DATA_MAP_SIZE                (ROM_SIZE + ROM_EXT_SIZE)
#else
#define CP_ROM_DATA_MAP_SIZE                ROM_SIZE
#endif
#elif (ROM_BASE)
#define CP_CODE_MAP_BASE                    ROM_BASE
#else
#define CP_CODE_MAP_BASE                    (0x800)
#endif
#define CP_CODE_MAP_SIZE                    (RAMX_BASE + RAM_TOTAL_SIZE - CP_CODE_MAP_BASE)

static CP_DATA_LOC const  mpu_regions_t mpu_table_cp[] = {
    {CP_CODE_MAP_BASE,      CP_CODE_MAP_SIZE,       MPU_ATTR_EXEC,          MAIR_ATTR_INT_SRAM},
#ifdef CP_ROM_DATA_MAP_BASE
    // ROM data is located at a different address range from ROM code
    {CP_ROM_DATA_MAP_BASE,  CP_ROM_DATA_MAP_SIZE,   MPU_ATTR_READ,          MAIR_ATTR_INT_SRAM},
#endif
    {RAM_BASE,              RAM_TOTAL_SIZE,         MPU_ATTR_READ_WRITE,    MAIR_ATTR_INT_SRAM},
#if ((RAMCP_BASE < RAM_BASE) || (RAMCP_BASE >= RAM_BASE + RAM_TOTAL_SIZE))
    {RAMCP_BASE,            RAMCP_SIZE,             MPU_ATTR_READ_WRITE,    MAIR_ATTR_INT_SRAM},
#endif
#ifdef MEM_POOL_BASE
    {MEM_POOL_BASE,         MEM_POOL_SIZE,          MPU_ATTR_READ_WRITE,    MAIR_ATTR_INT_SRAM},
#endif
    {CMU_BASE,              0x01000000,             MPU_ATTR_READ_WRITE,    MAIR_ATTR_DEVICE},
};
#else
static CP_DATA_LOC const  mpu_regions_t mpu_table_cp[] = {
    {0,                 0x800,      MPU_ATTR_NO_ACCESS},
    {FLASHX_BASE,       0x4000000,  MPU_ATTR_NO_ACCESS},
    {FLASH_BASE,        0x4000000,  MPU_ATTR_NO_ACCESS},
    {FLASH_NC_BASE,     0x4000000,  MPU_ATTR_NO_ACCESS},
};
#endif

#if (CP_CPU_USAGE_INTVL_MS > 0)
CP_TEXT_SRAM_LOC
static void cp_accel_refresh_accumulated_mips(uint32_t currentTime)
{
    uint32_t lock = int_lock_global();

    uint32_t active_time = currentTime - stats_start_time_of_current_freq - sleep_time_of_current_freq;

    stats_accumulated_mips += (uint32_t)(active_time*stats_converted_freq_mhz);
    int_unlock_global(lock);
}
#endif

static CP_TEXT_SRAM_LOC NOINLINE void accel_loop(void)
{
    uint32_t lock;
    uint8_t task_index = 0, event_index = 0;
    bool msg_flag = false;
    uint8_t msg[CP_TASK_MAX][CP_EVENT_MAX];
#if (CP_CPU_USAGE_INTVL_MS > 0)
    uint32_t time;
    uint32_t interval;
#endif

    mpu_setup_cp(mpu_table_cp, ARRAY_SIZE(mpu_table_cp));

    while (1) {
        lock = int_lock_global();

        msg_flag = cp_env.cp_msg_recv;
        cp_env.cp_msg_recv = false;
        memcpy(msg, (uint8_t *)cp_env.cp_msg, sizeof(cp_env.cp_msg));
        memset((uint8_t *)cp_env.cp_msg, 0, sizeof(cp_env.cp_msg));
#if !defined(SPECIFIC_FREQ_POWER_CONSUMPTION_MEASUREMENT_ENABLE)
        if (false == msg_flag) {
#if (CP_CPU_USAGE_INTVL_MS > 0)
            time = hal_sys_timer_get();
#endif
            cp_in_sleep = true;
            SCB->SCR = SCB_SCR_SLEEPDEEP_Msk;
            __DSB();
            __WFI();
            cp_in_sleep = false;
#if (CP_CPU_USAGE_INTVL_MS > 0)
            if (stats_interval) {
                uint32_t interval = hal_sys_timer_get() - time;
                sleep_time += interval;
                sleep_time_of_current_freq += interval;
            }
#endif
        }

#endif

#if (CP_CPU_USAGE_INTVL_MS > 0)
        time = hal_sys_timer_get();
        if (stats_interval) {
            interval = time - stats_start_time;
            if (interval >= stats_interval) {
                if (sleep_time > UINT32_MAX / 100) {
                    sleep_ratio = (uint64_t)sleep_time * 100 / interval;
                } else {
                    sleep_ratio = sleep_time * 100 / interval;
                }
                sleep_time = 0;
                stats_start_time = time;
                cp_accel_refresh_accumulated_mips(time);
                stats_average_mips = stats_accumulated_mips*1000/interval;
                stats_start_time_of_current_freq = time;
                sleep_time_of_current_freq = 0;
                stats_accumulated_mips = 0;
            }            
        }
#endif

        int_unlock_global(lock);

#if (CP_CPU_USAGE_INTVL_MS > 0) && (CP_CPU_USAGE_TRACE_INTVL_MS > 0)
        if (stats_interval && stats_trace_interval) {
            if (time - stats_trace_time >= stats_trace_interval) {
#ifdef USE_TRACE_ID
                TRACE(0, "CP CPU USAGE: busy=%d sleep=%d", (100 - sleep_ratio), sleep_ratio);
#else
                static const char CP_DATA_LOC usage_str[] = "CP CPU USAGE: busy=%d sleep=%d";
                TRACE(0, usage_str, (100 - sleep_ratio), sleep_ratio);
#endif
                stats_trace_time = time;
            }
        }
#endif

        if (msg_flag) {
            for (task_index = 0; task_index < CP_TASK_MAX; task_index++) {
                for (event_index = 0; event_index < CP_EVENT_MAX; event_index++) {
                    LOCK_CP_PROCESS();
                    if ((msg[task_index][event_index]) && (cp_task_env.p_desc[task_index].cp_work_main)) {
                        cp_task_env.p_desc[task_index].cp_work_main(event_index);
                    }
                    UNLOCK_CP_PROCESS();
                }
            }
        }
    }
}

extern void rtos_init();
static unsigned int cp2mcu_msg_arrived(const unsigned char *data, unsigned int len);
extern bool a2dp_cp_is_initialized(void);

void cp_accel_inform_freq_change(uint32_t cpu_freq)
{
#if (CP_CPU_USAGE_INTVL_MS > 0)
    if (cp_accel_init_done() && a2dp_cp_is_initialized())
    {
        if (stats_freq != cpu_freq)
        {
            stats_freq = cpu_freq;
            stats_is_freq_updated = true;
            TRACE(0, "update freq %d", cpu_freq);
        }
    }
#endif
}

#if (CP_CPU_USAGE_INTVL_MS > 0)
static SRAM_TEXT_LOC uint16_t cp_accel_convert_sysfreq_to_mhz(enum HAL_CMU_FREQ_T cpu_freq)
{
    switch(cpu_freq)
    {
        case HAL_CMU_FREQ_32K:
            return 0;
        case HAL_CMU_FREQ_26M:
            return 24;
        case HAL_CMU_FREQ_52M:
            return 48;
        case HAL_CMU_FREQ_78M:
            return 72;
        case HAL_CMU_FREQ_104M:
            return 96;
        case HAL_CMU_FREQ_208M:
            return SYSTEM_FREQ_208M;
        default:
            return 0;
    }
}

void cp_accel_reset_mips_stats(void)
{
    uint32_t lock = int_lock();
    sleep_time_of_current_freq = 0;
    stats_accumulated_mips = 0;
    stats_freq = hal_sysfreq_get();
    stats_converted_freq_mhz = cp_accel_convert_sysfreq_to_mhz(stats_freq);
    stats_is_freq_updated = false;
    int_unlock(lock);

    TRACE(0, "reset mips %d-%d", stats_freq, stats_converted_freq_mhz);
}

CP_TEXT_SRAM_LOC void cp_accel_refresh_freq_handler(void)
{
    if (stats_is_freq_updated && (stats_interval > 0))
    {
        uint32_t time = hal_sys_timer_get();
        uint32_t lock = int_lock_global();
        stats_is_freq_updated = false;
        cp_accel_refresh_accumulated_mips(time);

        switch(stats_freq)
        {
            case HAL_CMU_FREQ_32K:
                stats_converted_freq_mhz = 0;
                break;
            case HAL_CMU_FREQ_26M:
                stats_converted_freq_mhz = 24;
                break;
            case HAL_CMU_FREQ_52M:
                stats_converted_freq_mhz = 48;
                break;
            case HAL_CMU_FREQ_78M:
                stats_converted_freq_mhz = 72;
                break;
            case HAL_CMU_FREQ_104M:
                stats_converted_freq_mhz = 96;
                break;
            case HAL_CMU_FREQ_208M:
                stats_converted_freq_mhz = SYSTEM_FREQ_208M;
                break;
            default:
                stats_converted_freq_mhz = 0;
                break;
        }
    
        stats_start_time_of_current_freq = time;
        sleep_time_of_current_freq = 0;
        int_unlock_global(lock);
    }
}
#endif

static void accel_main(void)
{
#ifdef CORE0_RUN_CP_TASK
    NVIC_InitVectors();
    SystemInit();
#else
    TRACE(1, "%s", __func__);
#endif

    system_cp_init(!ram_inited);

#ifndef CORE_SLEEP_POWER_DOWN
    ram_inited = true;
#endif

    memset((uint8_t *)&cp_env, 0, sizeof(cp_env));

#ifdef CORE0_RUN_CP_TASK
    hal_mcu2cp_open_cp(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, cp2mcu_msg_arrived, mcu2cp_msg_sent, false);
    hal_mcu2cp_open_cp(HAL_MCU2CP_ID_1, HAL_MCU2CP_MSG_TYPE_0, cp2mcu_sys_arrived, NULL, false);
#else
    hal_trace_open_cp(cp_trace_buffer_ctrl, cp_trace_crash_notify);
    hal_mcu2cp_open_cp(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, mcu2cp_msg_arrived, NULL, false);
    hal_mcu2cp_open_cp(HAL_MCU2CP_ID_1, HAL_MCU2CP_MSG_TYPE_0, NULL, NULL, false);
#endif

    hal_mcu2cp_start_recv_cp(HAL_MCU2CP_ID_0);
    //hal_mcu2cp_start_recv_cp(HAL_MCU2CP_ID_1);

#if (CP_CPU_USAGE_INTVL_MS > 0)
    stats_interval = MS_TO_TICKS(CP_CPU_USAGE_INTVL_MS);
    stats_start_time = hal_sys_timer_get();
    stats_start_time_of_current_freq = stats_start_time;
    stats_freq = 0;
    sleep_time = 0;
    sleep_time_of_current_freq = 0;
    stats_accumulated_mips = 0;
#if (CP_CPU_USAGE_TRACE_INTVL_MS > 0)
    stats_trace_interval = MS_TO_TICKS(CP_CPU_USAGE_TRACE_INTVL_MS);
    stats_trace_time = stats_start_time;
#endif
    sleep_ratio = 0;
#endif

    cp_accel_inited = true;

#ifdef CORE0_RUN_CP_TASK
    rtos_init(); //never return
#else
    accel_loop();
#endif
}

static SRAM_TEXT_LOC unsigned int cp2mcu_msg_arrived(const unsigned char *data, unsigned int len)
{
    uint8_t task_id = CP_TASK_ID_GET((uint32_t)data);
    // TRACE(2, "%s, task_id = %d", __func__, task_id);

    if (task_id >= CP_TASK_MAX) {
        return -1;
    }

    if (cp_task_env.p_desc[task_id].mcu_evt_hdlr) {
        cp_task_env.p_desc[task_id].mcu_evt_hdlr((uint32_t)data);
    }

    return len;
}

int cp_accel_open(enum CP_TASK_TYPE task_id, struct cp_task_desc const * p_task_desc)
{
    uint32_t lock;
    int ret = 0;

    if ((task_id >= CP_TASK_MAX) || (p_task_desc == NULL)) {
        TRACE(1, "%s task id error", __func__);
        return -1;
    }

    TRACE(1, "%s, task id = %d, cp_state = %d init %d", __func__, task_id, cp_task_env.p_desc[task_id].cp_accel_state, cp_accel_inited);

    lock = int_lock();

    if (cp_task_env.p_desc[task_id].cp_accel_state != CP_ACCEL_STATE_CLOSED) {
        TRACE(1, "%s cp_accel_state error", __func__);
        ret = -2;
        goto _exit;
    }

    cp_task_env.p_desc[task_id].cp_accel_state = CP_ACCEL_STATE_OPENING;
    cp_task_env.p_desc[task_id].cp_work_main = p_task_desc->cp_work_main;
    cp_task_env.p_desc[task_id].cp_evt_hdlr = p_task_desc->cp_evt_hdlr;
    cp_task_env.p_desc[task_id].mcu_evt_hdlr = p_task_desc->mcu_evt_hdlr;
    cp_task_env.p_desc[task_id].mcu_sys_ctrl_hdlr = p_task_desc->mcu_sys_ctrl_hdlr;

    if (false == cp_accel_inited) {
#ifdef CORE_SLEEP_POWER_DOWN
        hal_chip_wake_lock(HAL_CHIP_WAKE_LOCK_USER_CP);
#endif

#ifndef CORE0_RUN_CP_TASK
        hal_cmu_cp_enable(RAMCP_BASE + RAMCP_SIZE, (uint32_t)accel_main);

        hal_mcu2cp_open_mcu(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, cp2mcu_msg_arrived, mcu2cp_msg_sent, false);
        hal_mcu2cp_open_mcu(HAL_MCU2CP_ID_1, HAL_MCU2CP_MSG_TYPE_0, cp2mcu_sys_arrived, NULL, false);

        hal_mcu2cp_start_recv_mcu(HAL_MCU2CP_ID_0);
        hal_mcu2cp_start_recv_mcu(HAL_MCU2CP_ID_1);
#endif
    }

    cp_task_env.p_desc[task_id].cp_accel_state = CP_ACCEL_STATE_OPENED;

_exit:
    int_unlock(lock);

    return ret;
}

int cp_accel_close(enum CP_TASK_TYPE task_id)
{
    uint8_t i = 0;
    uint32_t lock;

    TRACE(4, "%s, task id = %d, cp_state = %d init %d", __func__,
                task_id, cp_task_env.p_desc[task_id].cp_accel_state, cp_accel_inited);

    lock = int_lock();
    LOCK_CP_PROCESS();

    if (cp_task_env.p_desc[task_id].cp_accel_state == CP_ACCEL_STATE_CLOSED) {
        goto _exit;
    }

    cp_task_env.p_desc[task_id].cp_accel_state = CP_ACCEL_STATE_CLOSING;
    cp_task_env.p_desc[task_id].cp_work_main = NULL;
    cp_task_env.p_desc[task_id].cp_evt_hdlr = NULL;
    cp_task_env.p_desc[task_id].mcu_evt_hdlr = NULL;

    for (i=0; i<CP_TASK_MAX; i++) {
        if (cp_task_env.p_desc[i].cp_accel_state == CP_ACCEL_STATE_OPENED || \
            cp_task_env.p_desc[i].cp_accel_state == CP_ACCEL_STATE_OPENING) {
            goto _exit;
        }
    }

    if (cp_accel_inited) {
#ifndef CORE0_RUN_CP_TASK
        cp_accel_inited = false;
        hal_mcu2cp_close_mcu(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0);
        hal_mcu2cp_close_mcu(HAL_MCU2CP_ID_1, HAL_MCU2CP_MSG_TYPE_0);

        hal_cmu_cp_disable();

        hal_mcu2cp_close_cp(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0);
        hal_mcu2cp_close_cp(HAL_MCU2CP_ID_1, HAL_MCU2CP_MSG_TYPE_0);

        hal_trace_close_cp();

        system_cp_term();
#endif

#if (CP_CPU_USAGE_INTVL_MS > 0)
        sleep_ratio = 0;
#endif

#ifdef CORE_SLEEP_POWER_DOWN
        hal_chip_wake_unlock(HAL_CHIP_WAKE_LOCK_USER_CP);
#endif
    }

_exit:
    cp_task_env.p_desc[task_id].cp_accel_state = CP_ACCEL_STATE_CLOSED;

    UNLOCK_CP_PROCESS();
    int_unlock(lock);

    return 0;
}
#ifdef CORE0_RUN_CP_TASK
static void SRAM_TEXT_LOC cp_accel_set_sp(void)
{
    __set_MSP((uint32_t)__cp_stack_top);
    __set_MSPLIM((uint32_t)__cp_stack_limit);
    __DSB();
}

#ifdef CORE_SLEEP_POWER_DOWN
static HAL_POWER_DOWN_WAKEUP_HANDLER wakeup_func;
static void accel_wakeup_main(void)
{
    //doesn't need restore nvic here
    system_cp_init(!ram_inited);

    memset((uint8_t *)&cp_env, 0, sizeof(cp_env));

    hal_mcu2cp_open_cp(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, cp2mcu_msg_arrived, mcu2cp_msg_sent, false);
    hal_mcu2cp_open_cp(HAL_MCU2CP_ID_1, HAL_MCU2CP_MSG_TYPE_0, cp2mcu_sys_arrived, NULL, false);

    hal_mcu2cp_start_recv_cp(HAL_MCU2CP_ID_0);
    //hal_mcu2cp_start_recv_cp(HAL_MCU2CP_ID_1);

    cp_accel_inited = true;

    wakeup_func();//never return

    SAFE_PROGRAM_STOP();
}

void SRAM_TEXT_LOC cp_accel_wakeup(HAL_POWER_DOWN_WAKEUP_HANDLER wake_fn)
{
    cp_accel_set_sp();
    NVIC_InitVectors_cp();
    SystemInit_cp();

    cp_accel_inited = false;
    wakeup_func = wake_fn;

    uint32_t tmp_stack[128];
    hal_cmu_cp_enable((uint32_t)(&tmp_stack), (uint32_t)accel_wakeup_main);
    while (cp_accel_init_done() == 0) {
        hal_sys_timer_delay_us(100);
    }

    hal_mcu2cp_open_mcu(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, mcu2cp_msg_arrived, NULL, false);
    hal_mcu2cp_open_mcu(HAL_MCU2CP_ID_1, HAL_MCU2CP_MSG_TYPE_0, NULL, NULL, false);

    hal_mcu2cp_start_recv_mcu(HAL_MCU2CP_ID_0);
    hal_mcu2cp_start_recv_mcu(HAL_MCU2CP_ID_1);

    hal_trace_open_cp(cp_trace_buffer_ctrl, cp_trace_crash_notify);
    accel_loop();//never return
}
#endif
void SRAM_TEXT_LOC cp_accel_init_for_rtos(void)
{
    cp_accel_inited = false;
    cp_accel_set_sp();
    NVIC_InitVectors_cp();
    SystemInit_cp();

    hal_cmu_cp_enable((uint32_t)__StackTop, (uint32_t)accel_main);
    //wait CP ready
    while (cp_accel_init_done() == 0) {
        hal_sys_timer_delay_us(100);
    }

    hal_mcu2cp_open_mcu(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, mcu2cp_msg_arrived, NULL, false);
    hal_mcu2cp_open_mcu(HAL_MCU2CP_ID_1, HAL_MCU2CP_MSG_TYPE_0, NULL, NULL, false);

    hal_mcu2cp_start_recv_mcu(HAL_MCU2CP_ID_0);
    hal_mcu2cp_start_recv_mcu(HAL_MCU2CP_ID_1);

    hal_trace_open_cp(cp_trace_buffer_ctrl, cp_trace_crash_notify);
    accel_loop();//never return
}
#endif

void SRAM_TEXT_LOC cp_accel_init(void)
{
#ifndef CORE0_RUN_CP_TASK
    cp_accel_inited = false;
#endif
}

int SRAM_TEXT_LOC cp_accel_init_done(void)
{
    return cp_accel_inited;
}

int cp_accel_send_event_mcu2cp(uint8_t event)
{
    uint32_t lock;

    if ((false == cp_accel_inited) || (cp_env.mcu2cp_tx_count > MAX_CP_MSG_NUM)) {
        TRACE(2,
              "send_evt error, cp_accel_inited = %d, event pending count = %d",
              cp_accel_inited,
              cp_env.mcu2cp_tx_count);

        TRACE(2,
              "send evt task_id = %d, event = %d",
              CP_TASK_ID_GET(event),
              CP_EVENT_GET(event));
        return -1;
    }

    // TRACE(1, "current CP tx count:%d", cp_env.mcu2cp_tx_count);

    lock = int_lock();

    if (cp_env.mcu2cp_tx_count > 0) {
        cp_env.mcu2cp_tx_pending[cp_env.mcu2cp_tx_count - 1] = event;
        cp_env.mcu2cp_tx_count++;
    } else {
        req_event = event;
        cp_env.mcu2cp_tx_count = 1;
#ifdef CORE0_RUN_CP_TASK
        hal_mcu2cp_send_cp(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, &req_event, 1);
#else
        hal_mcu2cp_send_mcu(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, &req_event, 1);
#endif
    }

    int_unlock(lock);

    return 0;
}

int CP_TEXT_SRAM_LOC cp_accel_send_event_cp2mcu(uint8_t event)
{
#ifdef CORE0_RUN_CP_TASK
    return hal_mcu2cp_send_mcu(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, (unsigned char *)(uint32_t)event, 0);
#else
    return hal_mcu2cp_send_cp(HAL_MCU2CP_ID_0, HAL_MCU2CP_MSG_TYPE_0, (unsigned char *)(uint32_t)event, 0);
#endif
}

int SRAM_TEXT_LOC cp_accel_busy(enum CP_TASK_TYPE task_id)
{
    if (cp_task_env.p_desc[task_id].cp_accel_state != CP_ACCEL_STATE_CLOSED) {
        if (cp_task_env.p_desc[task_id].cp_accel_state == CP_ACCEL_STATE_OPENED && cp_in_sleep &&
#ifdef CORE0_RUN_CP_TASK
                !hal_mcu2cp_local_irq_pending_mcu(HAL_MCU2CP_ID_0)
#else
                !hal_mcu2cp_local_irq_pending_cp(HAL_MCU2CP_ID_0)
#endif
                ) {
            return false;
        }
        return true;
    }

    return false;
}

int cp_accel_get_cpu_usage(struct CP_ACCEL_CPU_USAGE_T *usage)
{
    int ret = -1;
#if (CP_CPU_USAGE_INTVL_MS > 0)
    uint8_t sleep;

    if (!usage) {
        return 1;
    }

    if (cp_accel_inited) {
        sleep = sleep_ratio;
        usage->sleep = sleep;
        usage->busy = 100 - sleep;
        usage->averageMips = stats_average_mips;
        ret = 0;
    } else {
        ret = 2;
    }
#endif

    return ret;
}

#endif

