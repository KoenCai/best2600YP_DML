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
#ifndef __SENSOR_HUB_CAP_CALCULATE_H__
#define __SENSOR_HUB_CAP_CALCULATE_H__

#ifdef __cplusplus
extern "C" {
#endif

// touch debug
#define TOUCH_TRACE TRACE

// touch key
#define FS          5
#define CHNUM       4   // channel numbers
#define REPNUM      2   // rep numbers.
#define USED_NUM    2   //calculate numbers.

#define KEYDOWN_TH1             (-46)   //(-36)//
#define KEYDOWN_TH2             (-38)   //(-28)//
#define KEY_CLICK_MAX_DURATION  (350)   // 350ms
#define KEY_CLICK_MIN_DURATION  (50)    // 50ms
#define KEY_DOUBLE_INTERVAL     (400)   // double key interval
#define KEY_LONG_INTERVAL       (1000)  // 1000ms
#define KEY_LONGLONG_INTERVAL   (5000)  // 5000ms
#define DC_RESET_INTERVAL       (20000) // 20000ms
#define SECOND_CLICK_TH_FACTOR  (0.7)
// end touch key

// wear detect
// #define FS_EAR              5     /* default is FS 5Hz */
#define OFF_EAR             0
#define ON_EAR              1
#define EDGE_UP             -1
#define EDGE_DOWN           1
#define LASTDATAN           (FS * 4)          /* this Macro is used to adjust calculation time */
#define EDGE_WIN            (20 * FS / 100)
#define EDGE_TH1            (EDGE_WIN * 80)       /* this Macro is used to adjust calculation sensitivity */
#define MID_HOP             1
#define FAR_HOP             2
#define PRE_FACTOR          fabs(0.5)
#define UNSTABLE_INTERVAL   (FS * 30)
#define PAUSE_INTERVAL      (FS * 2)
#define THRESHOLD_HIGH      15
#define THRESHOLD_LOW       5
// end wear detect

// touch key states.
enum key_state
{
    KEY_STATE_NONE          = 0,
    KEY_STATE_PRESSED       = 1,
    KEY_STATE_WAIT_RELEASE  = 2
};

enum work_mode
{
    TOUCH_MODE = 0,   /* touch mode */
    WEAR_MODE  = 1    /* wear mode */
};

enum chan_mode
{
    SINGLE_END   = 0,   /* Single-ended input */
    DIFFERENTIAL = 1    /* Differential input */
};

struct CAPSENSOR_CHANNELS_CFG_T
{
    uint8_t work_mode;      /* 0: touch mode; 1: wear mode */
    uint8_t channels;       /* channels select */
    uint8_t chan_mode;      /* 0: single-end; 1: differential */
    uint16_t sample_freq;   /* sample frequency */
};

struct cap_state_t
{
    uint8_t pre_state;
    uint8_t cur_state;
};

/* ===================== Extern Function declaration ===================== */
/* dc calculate */
double dc_value(unsigned short xin);

/* read key value */
uint8_t key_read(short xin, double dc, unsigned char *key, uint8_t channels);

/* detect wear status */
void edge_detect1(unsigned short xin1, uint8_t *state1, uint8_t channels);

#ifdef __cplusplus
}
#endif

#endif /* __SENSOR_HUB_CAP_CALCULATE_H__ */
