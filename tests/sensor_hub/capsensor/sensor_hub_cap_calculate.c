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
#include "hal_trace.h"
#include "hal_timer.h"
#include "hwtimer_list.h"
#include "sensor_hub_touch_core.h"
#include "sensor_hub_cap_calculate.h"
#include "math.h"
#include "app_key.h"

// Global variables
static bool reset_flag = true;

// Extern global variables

/* ===================== Local Function declaration ===================== */
/* calculate key value */
static uint8_t key_driver(short xin, double dc, uint8_t pre_state, uint8_t channels);

/***************************************************************************
 * @brief calculate dc value
 *
 * @param xin sum of cap data
 * @return double dc value
 ***************************************************************************/
double dc_value(unsigned short xin)
{
    static double dc       = 0;
    static int count_ini   = 0;
    static uint32_t reset_start = 0;
    static uint32_t reset_stop  = 0;

    if (reset_flag == true)
    {
        count_ini = 0;
        reset_flag = false;
    }

    if (count_ini == 0)
    {
        dc = (double)xin;
        count_ini++;
    }
    else if (count_ini < 8)
    {
        dc = dc * (double)0.875 + xin * (double)0.125;
        count_ini++;
    }
    else
    {
        if (fabs(xin - dc) < (-KEYDOWN_TH2))
        {
            dc = dc * (double)0.99 + xin * (double)0.01;
            reset_start = hal_sys_timer_get();
        }
        else
        {
            reset_stop = hal_sys_timer_get();
            if (TICKS_TO_MS(reset_stop - reset_start) >= DC_RESET_INTERVAL)   //ms
            {
                count_ini   = 8;//0;
                reset_start = hal_sys_timer_get();
            }
        }
    }
    return dc;
}

/***************************************************************************
 * @brief calculate the states of touch button
 *
 * @param xin sum of cap data
 * @param dc value from dc_value()
 * @param pre_state the Previous state
 * @param channels used channels
 * @return uint8_t touch button event
 ***************************************************************************/
static uint8_t key_driver(short xin, double dc, uint8_t pre_state, uint8_t channels)
{
    uint8_t key_return = APP_KEY_EVENT_NONE;
    float t1;
    float keydown_threshold;
    static float key_pressure_sum[CHNUM] = {0};
    static uint32_t timer_start[CHNUM]   = {0};
    static uint32_t timer_stop[CHNUM]    = {0};
    static bool longlong_press_flag      = 0;

    cap_state[channels].pre_state = pre_state;

    switch(cap_state[channels].cur_state)
    {
        case KEY_STATE_NONE:
            if (cap_state[channels].pre_state == KEY_STATE_PRESSED)    // detect touch pressed.
            {
                keydown_threshold = KEYDOWN_TH1 * SECOND_CLICK_TH_FACTOR;
            }
            else
            {
                keydown_threshold = KEYDOWN_TH1;
            }

            if((xin - dc) < (double)keydown_threshold)
            {
                // TOUCH_TRACE(1, "<0> touch pressed = %f < %f", (xin - dc), (double)keydown_threshold);
                t1 = (float)(xin - dc) / KEYDOWN_TH1;
                if(t1 > 3) t1 = 3;
                key_pressure_sum[channels] = t1;

                // TOUCH_TRACE(1, "<0> touch pressed channels= %d", channels);
                cap_state[channels].cur_state = KEY_STATE_PRESSED;
                timer_start[channels] = hal_sys_timer_get();
            }
            break;
        case KEY_STATE_PRESSED:
            keydown_threshold = KEYDOWN_TH2 * SECOND_CLICK_TH_FACTOR;
            if((xin - dc) < (double)keydown_threshold)
            {
                // TOUCH_TRACE(1, "<1> touch pressed = %f < %f", (xin - dc), (double)keydown_threshold);
                t1 = (float)(xin - dc) / KEYDOWN_TH1;
                if(t1 > 3) t1 = 3;
                key_pressure_sum[channels] += t1;

                timer_stop[channels] = hal_sys_timer_get();

                if (TICKS_TO_MS(timer_stop[channels] - timer_start[channels]) >= KEY_LONG_INTERVAL)
                {
                    // TOUCH_TRACE(1, "KEY_LONG_INTERVAL %d", (timer_stop[channels] - timer_start[channels]));
                    key_return = APP_KEY_EVENT_LONGPRESS;
                    cap_state[channels].cur_state = KEY_STATE_WAIT_RELEASE;

                    timer_start[channels] = hal_sys_timer_get();
                }
            }
            else
            {
                // TOUCH_TRACE(1, "<1> touch pressed = %f > %f", (xin - dc), (double)keydown_threshold);
                if (cap_state[channels].pre_state == KEY_STATE_PRESSED)
                {
                    keydown_threshold = 2 * (FS / 100) * SECOND_CLICK_TH_FACTOR;   //FS
                }
                else
                {
                    keydown_threshold = 2 * (FS / 100);    //FS
                }

                if(key_pressure_sum[channels] < keydown_threshold)
                {
                    // TOUCH_TRACE(1, "key_pressure_sum[%d] %f < %f!", channels, (double)key_pressure_sum[channels], (double)keydown_threshold);
                    cap_state[channels].cur_state = KEY_STATE_NONE;
                    timer_start[channels] = hal_sys_timer_get();
                }
                else
                {
                    timer_stop[channels] = hal_sys_timer_get();
                    // TOUCH_TRACE(1, "timer = %d key_click!", TICKS_TO_MS(timer_stop[channels] - timer_start[channels]));
                    if ((TICKS_TO_MS(timer_stop[channels] - timer_start[channels]) < KEY_CLICK_MAX_DURATION) &&
                        (TICKS_TO_MS(timer_stop[channels] - timer_start[channels]) > KEY_CLICK_MIN_DURATION))
                    {
                        key_return = APP_KEY_EVENT_CLICK;
                    }
                    else
                    {
                        // TOUCH_TRACE(1, "KEY_STATE_PRESSED-->APP_KEY_EVENT_UP!");
                        key_return = APP_KEY_EVENT_UP;
                    }
                    cap_state[channels].cur_state = KEY_STATE_NONE;
                    timer_start[channels] = hal_sys_timer_get();
                }
            }
            break;
        case KEY_STATE_WAIT_RELEASE:
            if ((xin - dc) > KEYDOWN_TH2)
            {
                reset_flag          = false;
                longlong_press_flag = false;
                key_return          = APP_KEY_EVENT_UP;
                cap_state[channels].cur_state = KEY_STATE_NONE;
                timer_start[channels] = hal_sys_timer_get();
                // TOUCH_TRACE(1, "APP_KEY_EVENT_UP!");
            }
            else
            {
                timer_stop[channels] = hal_sys_timer_get();
                if (TICKS_TO_MS(timer_stop[channels] - timer_start[channels]) >= ((KEY_LONGLONG_INTERVAL - KEY_LONG_INTERVAL)))
                {
                    if (longlong_press_flag == false)
                    {
                        reset_flag          = false;
                        longlong_press_flag = true;
                        cap_state[channels].cur_state = KEY_STATE_NONE;
                        key_return    = APP_KEY_EVENT_LONGLONGPRESS;
                        // TOUCH_TRACE(1, "KEY_LONGLONG_INTERVAL");
                    }
                    timer_start[channels] = hal_sys_timer_get();
                }
            }
            break;
        default :
            TRACE(0, "unknown state!");
            break;
    }

    return key_return;
}   /* end key_driver */


/***************************************************************************
 * @brief get touch key result
 *
 * @param xin sum of cap data
 * @param dc value from dc_value()
 * @param key storage key value
 * @param channels used channels
 * @return uint8_t return the final button event
 ***************************************************************************/
uint8_t key_read(short xin, double dc, uint8_t *key, uint8_t channels)
{
    uint8_t key_return = APP_KEY_EVENT_NONE;
    static uint32_t timer_start[CHNUM] = {0};
    static uint32_t timer_stop[CHNUM]  = {0};

    *key = key_driver(xin, dc, cap_state[channels].pre_state, channels);

    switch(cap_state[channels].pre_state)
    {
        case APP_KEY_EVENT_NONE:
            if (*key == APP_KEY_EVENT_CLICK)
            {
                timer_start[channels] = hal_sys_timer_get();
                cap_state[channels].pre_state = APP_KEY_EVENT_CLICK;
                // TOUCH_TRACE(1, "first click!");
            }
            else
            {
                // TOUCH_TRACE(1, "first click! %d", key_return);
                key_return = *key;
            }
            break;
        case APP_KEY_EVENT_CLICK:
            if (*key == APP_KEY_EVENT_CLICK)
            {
                key_return = APP_KEY_EVENT_DOUBLECLICK;
                reset_flag = false;
                cap_state[channels].pre_state = APP_KEY_EVENT_NONE;
                timer_stop[channels] = hal_sys_timer_get();
                // TOUCH_TRACE(1, " double click times =%d", TICKS_TO_MS(timer_stop[channels] - timer_start[channels]));
            }
            else
            {
                timer_stop[channels] = hal_sys_timer_get();
                if (TICKS_TO_MS(timer_stop[channels] - timer_start[channels]) >= KEY_DOUBLE_INTERVAL)
                {
                    key_return = APP_KEY_EVENT_CLICK;
                    reset_flag = false;
                    cap_state[channels].pre_state = APP_KEY_EVENT_NONE;
                    // TOUCH_TRACE(1, "one click times =%d", TICKS_TO_MS(timer_stop[channels] - timer_start[channels]));
                }
            }
            break;
    }

    return key_return;
}   /* end key_read */


/***************************************************************************
 * @brief wear state detect function
 *
 * @param xin1 sum of cap data
 * @param state1 current wearing status
 * @param channels used channels
 ***************************************************************************/
void edge_detect1(unsigned short xin1, uint8_t *state1, uint8_t channels)
{
    static bool initial1[CHNUM] = {1, 1, 1, 1};
    uint16_t i;
    static short lastdata[LASTDATAN][CHNUM];
    int far_data = 0;
    int mid_data = 0;
    int cur_data = 0;
    static short counti[CHNUM] = {0};
    static unsigned int time_counti[CHNUM] = {0};
    static double pre_diff1[CHNUM] = {EDGE_TH1, EDGE_TH1, EDGE_TH1, EDGE_TH1};
    static double max_hop1[CHNUM]  = {EDGE_TH1, EDGE_TH1, EDGE_TH1, EDGE_TH1};
    static short hop1[CHNUM] = {0};

    double u1 = 0.5;
    static bool intial_u1[CHNUM] = {1, 1, 1, 1};

    time_counti[channels]++;

    if(initial1[channels] == 1)
    {
        for(i = 0; i < LASTDATAN; i++) lastdata[i][channels] = 0;
        initial1[channels] = 0;
    }

    for(i = LASTDATAN - 1; i > 0; i--) lastdata[i][channels] = lastdata[i - 1][channels];
    lastdata[0][channels] = xin1;


    if(counti[channels] < LASTDATAN)
    {
        counti[channels]++;
        return;
    }

    for(i = 0; i < EDGE_WIN; i++)
    {
        cur_data += lastdata[i][channels];
        far_data += lastdata[LASTDATAN - 1 -i][channels];
        mid_data += lastdata[i + LASTDATAN / 2][channels];
    }

    if(time_counti[channels] < PAUSE_INTERVAL)
    {
        if(hop1[channels] == MID_HOP)
        {
            if(max_hop1[channels] < fabs(cur_data - mid_data))
                max_hop1[channels] = fabs(cur_data - mid_data);
        }

        if (hop1[channels] == FAR_HOP)
        {
            if(max_hop1[channels] < fabs(cur_data - far_data))
                max_hop1[channels] = fabs(cur_data - far_data);
        }
        return;
    }

    if(time_counti[channels] == PAUSE_INTERVAL)
    {
        if(intial_u1[channels] == 0)
            pre_diff1[channels] = pre_diff1[channels] * (1 - u1) + u1 * max_hop1[channels];
        else
        {
            pre_diff1[channels] = max_hop1[channels];
            intial_u1[channels] = 0;
        }
    }

    if(cur_data - mid_data <= -EDGE_TH1)
    {
        if(*state1 == OFF_EAR)
        {
            if((time_counti[channels] > FS * UNSTABLE_INTERVAL) || (fabs(cur_data - mid_data) > pre_diff1[channels] * PRE_FACTOR))
            {
                *state1 = ON_EAR;
                time_counti[channels] = 0;
                hop1[channels] = MID_HOP;
                max_hop1[channels] = fabs(cur_data - mid_data);
                return;
            }
        }
    }

    if(cur_data - mid_data > EDGE_TH1)
    {
        if(*state1 == ON_EAR)
        {
            if((time_counti[channels] > FS * UNSTABLE_INTERVAL) || (fabs(cur_data - mid_data) > pre_diff1[channels] * PRE_FACTOR))
            {
                *state1 = OFF_EAR;
                time_counti[channels] = 0;
                hop1[channels] = MID_HOP;
                max_hop1[channels] = fabs(cur_data - mid_data);
                return;
            }
        }
    }

    if(cur_data - far_data <= -EDGE_TH1)
    {
        if(*state1 == OFF_EAR)
        {
            if((time_counti[channels] > FS * UNSTABLE_INTERVAL) || (fabs(cur_data - far_data) > pre_diff1[channels] * PRE_FACTOR))
            {
                *state1 = ON_EAR;
                time_counti[channels] = 0;
                hop1[channels] = FAR_HOP;
                max_hop1[channels] = fabs(cur_data - far_data);
                return;
            }
        }
    }

    if(cur_data - far_data > EDGE_TH1)
    {
        if(*state1 == ON_EAR)
        {
            if((time_counti[channels] > FS * UNSTABLE_INTERVAL) || (fabs(cur_data - far_data) > pre_diff1[channels] * PRE_FACTOR))
            {
                *state1 = OFF_EAR;
                time_counti[channels] = 0;
                hop1[channels] = FAR_HOP;
                max_hop1[channels] = fabs(cur_data - far_data);
                return;
            }
        }
    }
}   /* end multi-channel edge detect */
