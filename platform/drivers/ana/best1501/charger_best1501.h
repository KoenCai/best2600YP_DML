/***************************************************************************
 *
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
 *
 ****************************************************************************/
#ifndef __CHARGER_BEST1501_1802_H__
#define __CHARGER_BEST1501_1802_H__

#ifdef __cplusplus
extern "C" {
#endif

enum CHARGER_STATUS_E {
    CHARGER_NO_CHARGING = 0x00,
    CHARGER_PRE_CHARGING,
    CHARGER_CHARGING_CC,
};

enum CHARGER_OPERATION_STATUS_E {
    CHARGER_OPERATION_SUCCESS = 0x0,
    CHARGER_OPERATION_FAIL,
};

enum CHARGER_CHARGING_CURRENT_E{
    CHARGER_CHARGING_CURRENT_5MA = 0,
    CHARGER_CHARGING_CURRENT_10MA = 1,
    CHARGER_CHARGING_CURRENT_15MA = 2,
    CHARGER_CHARGING_CURRENT_20MA = 3,
    CHARGER_CHARGING_CURRENT_25MA = 4,
    CHARGER_CHARGING_CURRENT_30MA = 5,
    CHARGER_CHARGING_CURRENT_35MA = 6,
    CHARGER_CHARGING_CURRENT_40MA = 7,
    CHARGER_CHARGING_CURRENT_45MA = 8,
    CHARGER_CHARGING_CURRENT_50MA = 9,
    CHARGER_CHARGING_CURRENT_55MA = 10,
    CHARGER_CHARGING_CURRENT_60MA = 11,
    CHARGER_CHARGING_CURRENT_65MA = 12,
    CHARGER_CHARGING_CURRENT_70MA = 13,
    CHARGER_CHARGING_CURRENT_75MA = 14,
    CHARGER_CHARGING_CURRENT_80MA = 15,
    CHARGER_CHARGING_CURRENT_90MA = 16,
    CHARGER_CHARGING_CURRENT_100MA = 17,
    CHARGER_CHARGING_CURRENT_110MA = 18,
    CHARGER_CHARGING_CURRENT_120MA = 19,
    CHARGER_CHARGING_CURRENT_130MA = 20,
    CHARGER_CHARGING_CURRENT_140MA = 21,
    CHARGER_CHARGING_CURRENT_150MA = 22,
    CHARGER_CHARGING_CURRENT_160MA = 23,
    CHARGER_CHARGING_CURRENT_170MA = 24,
    CHARGER_CHARGING_CURRENT_180MA = 25,
    CHARGER_CHARGING_CURRENT_190MA = 26,
    CHARGER_CHARGING_CURRENT_200MA = 27,
    CHARGER_CHARGING_CURRENT_210MA = 28,
    CHARGER_CHARGING_CURRENT_220MA = 29,
    CHARGER_CHARGING_CURRENT_230MA = 30,
    CHARGER_CHARGING_CURRENT_240MA = 31,
    CHARGER_CHARGING_CURRENT_260MA = 32,
    CHARGER_CHARGING_CURRENT_280MA = 33,
    CHARGER_CHARGING_CURRENT_300MA = 34,
    CHARGER_CHARGING_CURRENT_320MA = 35,

    CHARGER_CHARGING_CURRENT_QTY,
};

enum CHARGER_PRECHARGING_CURRENT_E{
    CHARGER_PRECHARGING_CURRENT_0P5MA = 0,
    CHARGER_PRECHARGING_CURRENT_1MA = 1,
    CHARGER_PRECHARGING_CURRENT_1P5MA = 2,
    CHARGER_PRECHARGING_CURRENT_2MA = 3,
    CHARGER_PRECHARGING_CURRENT_2P5MA = 4,
    CHARGER_PRECHARGING_CURRENT_3MA = 5,
    CHARGER_PRECHARGING_CURRENT_3P5MA = 6,
    CHARGER_PRECHARGING_CURRENT_4MA = 7,
    CHARGER_PRECHARGING_CURRENT_4P5MA = 8,
    CHARGER_PRECHARGING_CURRENT_5MA = 9,
    CHARGER_PRECHARGING_CURRENT_5P5MA = 10,
    CHARGER_PRECHARGING_CURRENT_6MA = 11,
    CHARGER_PRECHARGING_CURRENT_6P5MA = 12,
    CHARGER_PRECHARGING_CURRENT_7MA = 13,
    CHARGER_PRECHARGING_CURRENT_7P5MA = 14,
    CHARGER_PRECHARGING_CURRENT_8MA = 15,
    CHARGER_PRECHARGING_CURRENT_9MA = 16,
    CHARGER_PRECHARGING_CURRENT_10MA = 17,
    CHARGER_PRECHARGING_CURRENT_11MA = 18,
    CHARGER_PRECHARGING_CURRENT_12MA = 19,
    CHARGER_PRECHARGING_CURRENT_13MA = 20,
    CHARGER_PRECHARGING_CURRENT_14MA = 21,
    CHARGER_PRECHARGING_CURRENT_15MA = 22,
    CHARGER_PRECHARGING_CURRENT_16MA = 23,
    CHARGER_PRECHARGING_CURRENT_17MA = 24,
    CHARGER_PRECHARGING_CURRENT_18MA = 25,
    CHARGER_PRECHARGING_CURRENT_19MA = 26,
    CHARGER_PRECHARGING_CURRENT_20MA = 27,
    CHARGER_PRECHARGING_CURRENT_21MA = 28,
    CHARGER_PRECHARGING_CURRENT_22MA = 29,
    CHARGER_PRECHARGING_CURRENT_23MA = 30,
    CHARGER_PRECHARGING_CURRENT_24MA = 31,
    CHARGER_PRECHARGING_CURRENT_26MA = 32,
    CHARGER_PRECHARGING_CURRENT_28MA = 33,
    CHARGER_PRECHARGING_CURRENT_30MA = 34,
    CHARGER_PRECHARGING_CURRENT_32MA = 35,

    CHARGER_PRECHARGING_CURRENT_QTY,
};

enum CHARGER_CHARGING_TERMINATION_CURRENT_E{
    CHARGER_CHARGING_TERMINATION_CURRENT_2MA = 0,
    CHARGER_CHARGING_TERMINATION_CURRENT_4MA = 4,
    CHARGER_CHARGING_TERMINATION_CURRENT_8MA = 8,

    CHARGER_CHARGING_TERMINATION_CURRENT_QTY,
};

enum CHARGER_FULL_CHARGER_VOLTAGE_E{
    CHARGER_FULL_CHARGER_VOLTAGE_4V = 27,
    CHARGER_FULL_CHARGER_VOLTAGE_4P1V = 33,
    CHARGER_FULL_CHARGER_VOLTAGE_4P2V = 39,
    CHARGER_FULL_CHARGER_VOLTAGE_4P35V = 47,
    CHARGER_FULL_CHARGER_VOLTAGE_4P4V = 50,

    CHARGER_FULL_CHARGER_VOLTAGE_QTY,
};

enum CHARGER_RECHARGER_VOLTAGE_E{
    CHARGER_RECHARGER_VOLTAGE_DROP_4P5_PERCENT = 1,
    CHARGER_RECHARGER_VOLTAGE_DROP_3P5_PERCENT = 2,
    CHARGER_RECHARGER_VOLTAGE_DROP_2P5_PERCENT = 4,

    CHARGER_RECHARGER_VOLTAGE_DROP_QTY,
};

enum CHARGER_ILIMIT_CURRENT_LEVEL_E{
    CHARGER_ILIMIT_CURRENT_LEVEL_250MA = 1,
    CHARGER_ILIMIT_CURRENT_LEVEL_400MA = 2,

    CHARGER_ILIMIT_CURRENT_LEVEL_QTY,
};

enum CHARGER_VSEL_PRE2CC_VOLTAGE_E{
    CHARGER_VSEL_PRE2CC_VOLTAGE_2P4V = 0,
    CHARGER_VSEL_PRE2CC_VOLTAGE_2P5V = 1,
    CHARGER_VSEL_PRE2CC_VOLTAGE_2P6V = 2,
    CHARGER_VSEL_PRE2CC_VOLTAGE_2P7V = 3,
    CHARGER_VSEL_PRE2CC_VOLTAGE_2P8V = 4,
    CHARGER_VSEL_PRE2CC_VOLTAGE_2P9V = 5,
    CHARGER_VSEL_PRE2CC_VOLTAGE_3P0V = 6,
    CHARGER_VSEL_PRE2CC_VOLTAGE_3P1V = 7,

    CHARGER_VSEL_PRE2CC_VOLTAGE_QTY,
};

enum CHARGER_BAT_TEMP_STATUS_E {
    CHARGER_BAT_TEMP_COLD     = 0,   //[~, 0]
    CHARGER_BAT_TEMP_COOL     = 1,   //[0, 10]
    CHARGER_BAT_TEMP_NORMAL   = 2,   //[10, 45]
    CHARGER_BAT_TEMP_WARM     = 3,   //[45, 60]
    CHARGER_BAT_TEMP_HOT      = 4,   //[60, ~]
    CHARGER_BAT_TEMP_NONE     = 5,   //Unknown status
 };

int charger_enter_shipping_mode(bool shipping_mode);
int charger_status_get(void);
int charger_full_voltage_set(enum CHARGER_FULL_CHARGER_VOLTAGE_E full_voltage);
int charger_precharging_current_set(enum CHARGER_PRECHARGING_CURRENT_E precharging_current);
int charger_charging_current_set(enum CHARGER_CHARGING_CURRENT_E charging_current);
int charger_vbat_div_adc_enable(bool enable);
int charger_temp_status_get(void);
int charger_enable_charging(void);
int charger_disable_charging(void);
bool charger_charging_timeout(void);
int charger_recharger_voltage_set(enum CHARGER_RECHARGER_VOLTAGE_E recharger_voltage);
int charger_charging_termination_current_set(enum CHARGER_CHARGING_TERMINATION_CURRENT_E charging_terminaton_current);
int charger_is_charging_full(void);
int charger_enable_NTC(void);
int charger_disable_NTC(void);
int charger_ilimit_current_set(enum CHARGER_ILIMIT_CURRENT_LEVEL_E ilimit_current);
int charger_ovp_get(void);
int charger_precharging_voltage_set(enum CHARGER_VSEL_PRE2CC_VOLTAGE_E precharging_voltage);
int charger_full_voltage_get(enum CHARGER_FULL_CHARGER_VOLTAGE_E* full_voltage);
int charger_precharging_current_get(enum CHARGER_PRECHARGING_CURRENT_E* precharging_current);
int charger_charging_current_get(enum CHARGER_CHARGING_CURRENT_E* charging_current);
int charger_recharger_voltage_get(enum CHARGER_RECHARGER_VOLTAGE_E* recharger_voltage);
int charger_charging_termination_current_get(enum CHARGER_CHARGING_TERMINATION_CURRENT_E* charging_terminaton_current);
int charger_ilimit_current_get(enum CHARGER_ILIMIT_CURRENT_LEVEL_E* ilimit_current);
int charger_precharging_voltage_get(enum CHARGER_VSEL_PRE2CC_VOLTAGE_E* precharging_voltage);

#ifdef __cplusplus
}
#endif

#endif
