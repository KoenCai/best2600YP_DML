/***************************************************************************
 *
 * Copyright 2015-2020 BES.
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
#include "pmu.h"
#include CHIP_SPECIFIC_HDR(pmu_itn)
#include "analog.h"
#include "cmsis.h"
#include "cmsis_nvic.h"
#include "hal_aud.h"
#include "hal_cache.h"
#include "hal_chipid.h"
#include "hal_cmu.h"
#include "hal_location.h"
#include "hal_timer.h"
#include "hal_trace.h"
#include "hal_sysfreq.h"
#include "tgt_hardware.h"
#include "usbphy.h"
#include "hal_psc.h"

#define PMU_LED_VIA_PWM
#define DCDC_ULP_LP_ON
//#define PMU1800_LED_PWM_VIA_BREATH
//#define BT_RCOSC_CAL

// LDO soft start interval is about 1000 us
#define PMU_LDO_PU_STABLE_TIME_US       1800
#define PMU_DCDC_PU_STABLE_TIME_US      100
#define PMU_VANA_STABLE_TIME_US         10
#define PMU_VCORE_STABLE_TIME_US        10

#ifdef IBRT_DUAL_ANT_CTRL
#define IO_VOLT_ACTIVE_NORMAL           PMU_IO_2_6V
#define IO_VOLT_ACTIVE_RISE             PMU_IO_2_6V
#define IO_VOLT_SLEEP                   PMU_IO_2_6V
#else
#define IO_VOLT_ACTIVE_NORMAL           PMU_IO_1_8V
#define IO_VOLT_ACTIVE_RISE             PMU_IO_1_8V
#define IO_VOLT_SLEEP                   PMU_IO_1_8V
#endif
#define IO_VOLT_ACTIVE_NORMAL_1803      PMU_IO_3_3V
#define IO_VOLT_ACTIVE_RISE_1803        PMU_IO_3_3V
#define IO_VOLT_SLEEP_1803              PMU_IO_3_3V

#define HPPA_RAMP_UP_VOLT_MV            1950

#define PMU_DCDC_ANA_2_1V               0xF0
#define PMU_DCDC_ANA_2_0V               0xE0
#define PMU_DCDC_ANA_1_9V               0xD0
#define PMU_DCDC_ANA_1_8V               0xC0
#define PMU_DCDC_ANA_1_7V               0xB0
#define PMU_DCDC_ANA_1_6V               0xA0
#define PMU_DCDC_ANA_1_5V               0x90
#define PMU_DCDC_ANA_1_4V               0x80
#define PMU_DCDC_ANA_1_35V              0x77
#define PMU_DCDC_ANA_1_3V               0x70
#define PMU_DCDC_ANA_1_2V               0x60
#define PMU_DCDC_ANA_1_1V               0x50
#define PMU_DCDC_ANA_1_0V               0x40

#define PMU_DCDC_ANA_SLEEP_1_3V         PMU_DCDC_ANA_1_3V
#define PMU_DCDC_ANA_SLEEP_1_2V         PMU_DCDC_ANA_1_2V
#define PMU_DCDC_ANA_SLEEP_1_1V         PMU_DCDC_ANA_1_1V
#define PMU_DCDC_ANA_SLEEP_1_0V         PMU_DCDC_ANA_1_0V

#define PMU_VBUCK2ANA_1_5V              0x30
#define PMU_VBUCK2ANA_1_4V              0x2C
#define PMU_VBUCK2ANA_1_3V              0x28
#define PMU_VBUCK2ANA_1_2V              0x24
#define PMU_VBUCK2ANA_1_1V              0x20
#define PMU_VBUCK2ANA_1_0V              0x1C

#define PMU_DCDC_DIG_1_1V               0xFF
#define PMU_DCDC_DIG_1_05V              0xF0
#define PMU_DCDC_DIG_1_0V               0xE0
#define PMU_DCDC_DIG_0_95V              0xD0
#define PMU_DCDC_DIG_0_9V               0xC0
#define PMU_DCDC_DIG_0_85V              0xB0
#define PMU_DCDC_DIG_0_8V               0xA0
#define PMU_DCDC_DIG_0_75V              0x90
#define PMU_DCDC_DIG_0_7V               0x80
#define PMU_DCDC_DIG_0_65V              0x70
#define PMU_DCDC_DIG_0_6V               0x60
#define PMU_DCDC_DIG_0_55V              0x50
#define PMU_DCDC_DIG_0_5V               0x40
#define PMU_DCDC_DIG_0_45V              0x30
#define PMU_DCDC_DIG_0_4V               0x20

#define PMU_VDIG_1_2V                   0x24
#define PMU_VDIG_1_1V                   0x20
#define PMU_VDIG_1_05V                  0x1E
#define PMU_VDIG_1_0V                   0x1C
#define PMU_VDIG_0_95V                  0x1A
#define PMU_VDIG_0_9V                   0x18
#define PMU_VDIG_0_85V                  0x16
#define PMU_VDIG_0_8V                   0x14
#define PMU_VDIG_0_75V                  0x12
#define PMU_VDIG_0_7V                   0x10
#define PMU_VDIG_0_65V                  0x0E
#define PMU_VDIG_0_6V                   0x0C
#define PMU_VDIG_0_55V                  0x0A
#define PMU_VDIG_0_5V                   0x08
#define PMU_VDIG_0_45V                  0x06
#define PMU_VDIG_0_4V                   0x04
#define PMU_VDIG_MAX                    PMU_VDIG_1_2V

#define PMU_IO_3_3V                     0x15
#define PMU_IO_3_2V                     0x14
#define PMU_IO_3_1V                     0x13
#define PMU_IO_3_0V                     0x12
#define PMU_IO_2_9V                     0x11
#define PMU_IO_2_8V                     0x10
#define PMU_IO_2_7V                     0xF
#define PMU_IO_2_6V                     0xE
#define PMU_IO_2_5V                     0xD
#define PMU_IO_2_4V                     0xC
#define PMU_IO_2_3V                     0xB
#define PMU_IO_2_2V                     0xA
#define PMU_IO_2_1V                     0x9
#define PMU_IO_2_0V                     0x8
#define PMU_IO_1_9V                     0x7
#define PMU_IO_1_8V                     0x6
#define PMU_IO_1_7V                     0x5

#define PMU_VMEM_2_8V                   0x10
#define PMU_VMEM_2_0V                   0x8
#define PMU_VMEM_1_9V                   0x7
#define PMU_VMEM_1_8V                   0x6
#define PMU_VMEM_1_7V                   0x5
#define PMU_VMEM_1_6V                   0x4

#define PMU_CODEC_2_7V                  0xF
#define PMU_CODEC_2_6V                  0xE
#define PMU_CODEC_2_5V                  0xD
#define PMU_CODEC_2_4V                  0xC
#define PMU_CODEC_2_3V                  0xB
#define PMU_CODEC_2_2V                  0xA
#define PMU_CODEC_2_1V                  0x9
#define PMU_CODEC_2_0V                  0x8
#define PMU_CODEC_1_9V                  0x7
#define PMU_CODEC_1_8V                  0x6
#define PMU_CODEC_1_7V                  0x5
#define PMU_CODEC_1_6V                  0x4
#define PMU_CODEC_1_5V                  0x3

#define PMU_HPPA2CODEC_2_3V             0x1C
#define PMU_HPPA2CODEC_2_2V             0x18
#define PMU_HPPA2CODEC_2_1V             0x14
#define PMU_HPPA2CODEC_2_0V             0x10
#define PMU_HPPA2CODEC_1_95V            0xE
#define PMU_HPPA2CODEC_1_9V             0xC
#define PMU_HPPA2CODEC_1_8V             0x8
#define PMU_HPPA2CODEC_1_7V             0x4
#define PMU_HPPA2CODEC_1_6V             0x0

#define PMU_USB_3_4V                    0xA
#define PMU_USB_3_3V                    0x9
#define PMU_USB_3_2V                    0x8
#define PMU_USB_3_1V                    0x7
#define PMU_USB_3_0V                    0x6
#define PMU_USB_2_9V                    0x5
#define PMU_USB_2_8V                    0x4
#define PMU_USB_2_7V                    0x3
#define PMU_USB_2_6V                    0x2

#define PMU_VHPPA_2_0V                  0x8
#define PMU_VHPPA_1_9V                  0x7
#define PMU_VHPPA_1_8V                  0x6
#define PMU_VHPPA_1_7V                  0x5
#define PMU_VHPPA_1_6V                  0x4
#define PMU_VHPPA_1_5V                  0x3

#define PMU_DCDC_HPPA_2_1V              0xF0
#define PMU_DCDC_HPPA_2_0V              0xE0
#define PMU_DCDC_HPPA_1_95V             0xD8
#define PMU_DCDC_HPPA_1_9V              0xD0
#define PMU_DCDC_HPPA_1_8V              0xC0
#define PMU_DCDC_HPPA_1_7V              0xB0
#define PMU_DCDC_HPPA_1_6V              0xA0
#define PMU_DCDC_HPPA_1_5V              0x90
#define PMU_DCDC_HPPA_1_4V              0x80
#define PMU_DCDC_HPPA_1_3V              0x70
#define PMU_DCDC_HPPA_1_2V              0x60

#define PMU_VMIC_3_3V                   0x1B
#define PMU_VMIC_3_2V                   0x1A
#define PMU_VMIC_3_1V                   0x19
#define PMU_VMIC_3_0V                   0x18
#define PMU_VMIC_2_9V                   0x17
#define PMU_VMIC_2_8V                   0x16
#define PMU_VMIC_2_7V                   0x15
#define PMU_VMIC_2_6V                   0x14
#define PMU_VMIC_2_5V                   0xF
#define PMU_VMIC_2_4V                   0xE
#define PMU_VMIC_2_3V                   0xD
#define PMU_VMIC_2_2V                   0xC
#define PMU_VMIC_2_1V                   0xB
#define PMU_VMIC_2_0V                   0xA
#define PMU_VMIC_1_9V                   (((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) ? 0x09 : 0xA)
#define PMU_VMIC_1_8V                   (((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) ? 0x8  : 0x9)
#define PMU_VMIC_1_7V                   (((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) ? 0x7  : 0x8)
#define PMU_VMIC_1_6V                   (((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) ? 0x7  : 0x6)

#define PMU_VMIC_RES_3_3V               0xF
#define PMU_VMIC_RES_2_8V               0xA
#define PMU_VMIC_RES_2_2V               0x4

// 00   PMU_REG_METAL_ID
#define CHIPID_SHIFT                        4
#define CHIPID_MASK                         (0xFFF << CHIPID_SHIFT)
#define CHIPID(n)                           BITFIELD_VAL(CHIPID, n)
#define REVID_SHIFT                         0
#define REVID_MASK                          (0xF << REVID_SHIFT)
#define REVID(n)                            BITFIELD_VAL(REVID, n)

// 02   PMU_REG_POWER_KEY_CFG
#define REG_PU_VBAT_DIV                     (1 << 15)
#define PU_LPO_DR                           (1 << 14)
#define PU_LPO_REG                          (1 << 13)
#define POWERKEY_WAKEUP_OSC_EN              (1 << 12)
#define RTC_POWER_ON_EN                     (1 << 11)
#define PU_ALL_REG                          (1 << 10)
#define CLK_32K_SEL_SHIFT                   8
#define CLK_32K_SEL_MASK                    (0x3 << CLK_32K_SEL_SHIFT)
#define CLK_32K_SEL(n)                      BITFIELD_VAL(CLK_32K_SEL, n)
#define DEEPSLEEP_MODE_DIGI_DR              (1 << 7)
#define DEEPSLEEP_MODE_DIGI_REG             (1 << 6)
#define REG_VCORE_SSTIME_MODE_SHIFT         4
#define REG_VCORE_SSTIME_MODE_MASK          (0x3 << REG_VCORE_SSTIME_MODE_SHIFT)
#define REG_VCORE_SSTIME_MODE(n)            BITFIELD_VAL(REG_VCORE_SSTIME_MODE, n)
#define REG_LDO_SS_BIAS_EN_DR               (1 << 3)
#define REG_LDO_SS_BIAS_EN                  (1 << 2)
#define REG_LP_EN_VHPPA_DSLEEP              (1 << 1)
#define REG_LP_EN_VHPPA_NORMAL              (1 << 0)

// 03   PMU_REG_BIAS_CFG
#define REG_VSENSE_SEL_VMEM                 (1 << 15)
#define PU_LP_BIAS_LDO_DSLEEP               (1 << 14)
#define PU_LP_BIAS_LDO_DR                   (1 << 13)
#define PU_LP_BIAS_LDO_REG                  (1 << 12)
#define PU_BIAS_LDO_DR                      (1 << 11)
#define PU_BIAS_LDO_REG                     (1 << 10)
#define BG_VBG_SEL_DR                       (1 << 9)
#define BG_VBG_SEL_REG                      (1 << 8)
#define BG_CONSTANT_GM_BIAS_DR              (1 << 7)
#define BG_CONSTANT_GM_BIAS_REG             (1 << 6)
#define BG_CORE_EN_DR                       (1 << 5)
#define BG_CORE_EN_REG                      (1 << 4)
#define REG_PU_LDO_VANA_DR                  (1 << 3)
#define REG_PU_LDO_VANA                     (1 << 2)
#define BG_NOTCH_EN_DR                      (1 << 1)
#define BG_NOTCH_EN_REG                     (1 << 0)

#define REG_PU_LDO_VANA_REG                 REG_PU_LDO_VANA

// 05   PMU_REG_CHARGER_CFG
#define REG_PU_LDO_VRTC_RF_DSLEEP           (1 << 15)
#define REG_PU_LDO_VRTC_RF_DR               (1 << 14)
#define REG_PU_LDO_VRTC_RF_REG              (1 << 13)
#define REG_CHARGE_OUT_INTR_MSK             (1 << 12)
#define REG_CHARGE_IN_INTR_MSK              (1 << 11)
#define REG_AC_ON_OUT_EN                    (1 << 10)
#define REG_AC_ON_IN_EN                     (1 << 9)
#define REG_CHARGE_INTR_EN                  (1 << 8)
#define REG_AC_ON_DB_VALUE_SHIFT            0
#define REG_AC_ON_DB_VALUE_MASK             (0xFF << REG_AC_ON_DB_VALUE_SHIFT)
#define REG_AC_ON_DB_VALUE(n)               BITFIELD_VAL(REG_AC_ON_DB_VALUE, n)

// 07   PMU_REG_ANA_CFG
#define REG_LIGHT_LOAD_VANA_BUCK_LDO        (1 << 15)
#define REG_PULLDOWN_VANA_LDO               (1 << 14)
#define LP_EN_VANA_LDO_DSLEEP               (1 << 13)
#define LP_EN_VANA_LDO_DR                   (1 << 12)
#define LP_EN_VANA_LDO_REG                  (1 << 11)
#define REG_PU_LDO_VANA_DSLEEP              (1 << 10)
#define LDO_ANA_VBIT_DSLEEP_SHIFT           5
#define LDO_ANA_VBIT_DSLEEP_MASK            (0x1F << LDO_ANA_VBIT_DSLEEP_SHIFT)
#define LDO_ANA_VBIT_DSLEEP(n)              BITFIELD_VAL(LDO_ANA_VBIT_DSLEEP, n)
#define LDO_ANA_VBIT_NORMAL_SHIFT           0
#define LDO_ANA_VBIT_NORMAL_MASK            (0x1F << LDO_ANA_VBIT_NORMAL_SHIFT)
#define LDO_ANA_VBIT_NORMAL(n)              BITFIELD_VAL(LDO_ANA_VBIT_NORMAL, n)

#define LDO_VANA_VBIT_DSLEEP_SHIFT          LDO_ANA_VBIT_DSLEEP_SHIFT
#define LDO_VANA_VBIT_DSLEEP_MASK           LDO_ANA_VBIT_DSLEEP_MASK
#define LDO_VANA_VBIT_NORMAL_SHIFT          LDO_ANA_VBIT_NORMAL_SHIFT
#define LDO_VANA_VBIT_NORMAL_MASK           LDO_ANA_VBIT_NORMAL_MASK

// 08   PMU_REG_DIG_CFG
#define REG_DCDC_CLK_SE_EN                  (1 << 15)
#define LP_EN_VCORE_LDO_DSLEEP              (1 << 14)
#define LP_EN_VCORE_LDO_DR                  (1 << 13)
#define LP_EN_VCORE_LDO_REG                 (1 << 12)
#define LDO_DIG_VBIT_DSLEEP_SHIFT           6
#define LDO_DIG_VBIT_DSLEEP_MASK            (0x3F << LDO_DIG_VBIT_DSLEEP_SHIFT)
#define LDO_DIG_VBIT_DSLEEP(n)              BITFIELD_VAL(LDO_DIG_VBIT_DSLEEP, n)
#define LDO_DIG_VBIT_NORMAL_SHIFT           0
#define LDO_DIG_VBIT_NORMAL_MASK            (0x3F << LDO_DIG_VBIT_NORMAL_SHIFT)
#define LDO_DIG_VBIT_NORMAL(n)              BITFIELD_VAL(LDO_DIG_VBIT_NORMAL, n)

#define LDO_VCORE_VBIT_DSLEEP_SHIFT         LDO_DIG_VBIT_DSLEEP_SHIFT
#define LDO_VCORE_VBIT_DSLEEP_MASK          LDO_DIG_VBIT_DSLEEP_MASK
#define LDO_VCORE_VBIT_NORMAL_SHIFT         LDO_DIG_VBIT_NORMAL_SHIFT
#define LDO_VCORE_VBIT_NORMAL_MASK          LDO_DIG_VBIT_NORMAL_MASK

// Real bit is in REG_101 -- but we never change the default value
#define REG_PU_LDO_VCORE_DSLEEP             0

// 09   PMU_REG_IO_CFG
#define LP_EN_VIO_LDO_DSLEEP                (1 << 15)
#define LP_EN_VIO_LDO_DR                    (1 << 14)
#define LP_EN_VIO_LDO_REG                   (1 << 13)
#define REG_PU_LDO_VIO_DSLEEP               (1 << 12)
#define REG_PU_LDO_VIO_DR                   (1 << 11)
#define REG_PU_LDO_VIO                      (1 << 10)
#define LDO_VIO_VBIT_NORMAL_SHIFT           5
#define LDO_VIO_VBIT_NORMAL_MASK            (0x1F << LDO_VIO_VBIT_NORMAL_SHIFT)
#define LDO_VIO_VBIT_NORMAL(n)              BITFIELD_VAL(LDO_VIO_VBIT_NORMAL, n)
#define LDO_VIO_VBIT_DSLEEP_SHIFT           0
#define LDO_VIO_VBIT_DSLEEP_MASK            (0x1F << LDO_VIO_VBIT_DSLEEP_SHIFT)
#define LDO_VIO_VBIT_DSLEEP(n)              BITFIELD_VAL(LDO_VIO_VBIT_DSLEEP, n)

#define REG_PU_LDO_VIO_REG                  REG_PU_LDO_VIO

// 0A   PMU_REG_MEM_CFG
#define LP_EN_VMEM_LDO_DSLEEP               (1 << 15)
#define LP_EN_VMEM_LDO_DR                   (1 << 14)
#define LP_EN_VMEM_LDO_REG                  (1 << 13)
#define REG_PU_LDO_VMEM_DSLEEP              (1 << 12)
#define REG_PU_LDO_VMEM_DR                  (1 << 11)
#define REG_PU_LDO_VMEM_REG                 (1 << 10)
#define LDO_VMEM_VBIT_NORMAL_SHIFT          5
#define LDO_VMEM_VBIT_NORMAL_MASK           (0x1F << LDO_VMEM_VBIT_NORMAL_SHIFT)
#define LDO_VMEM_VBIT_NORMAL(n)             BITFIELD_VAL(LDO_VMEM_VBIT_NORMAL, n)
#define LDO_VMEM_VBIT_DSLEEP_SHIFT          0
#define LDO_VMEM_VBIT_DSLEEP_MASK           (0x1F << LDO_VMEM_VBIT_DSLEEP_SHIFT)
#define LDO_VMEM_VBIT_DSLEEP(n)             BITFIELD_VAL(LDO_VMEM_VBIT_DSLEEP, n)

// 0B   PMU_REG_GP_CFG
#define LP_EN_VGP_LDO_DSLEEP                (1 << 15)
#define LP_EN_VGP_LDO_DR                    (1 << 14)
#define LP_EN_VGP_LDO_REG                   (1 << 13)
#define REG_PU_LDO_VGP_DSLEEP               (1 << 12)
#define REG_PU_LDO_VGP_DR                   (1 << 11)
#define REG_PU_LDO_VGP_REG                  (1 << 10)
#define LDO_VGP_VBIT_NORMAL_SHIFT           5
#define LDO_VGP_VBIT_NORMAL_MASK            (0x1F << LDO_VGP_VBIT_NORMAL_SHIFT)
#define LDO_VGP_VBIT_NORMAL(n)              BITFIELD_VAL(LDO_VGP_VBIT_NORMAL, n)
#define LDO_VGP_VBIT_DSLEEP_SHIFT           0
#define LDO_VGP_VBIT_DSLEEP_MASK            (0x1F << LDO_VGP_VBIT_DSLEEP_SHIFT)
#define LDO_VGP_VBIT_DSLEEP(n)              BITFIELD_VAL(LDO_VGP_VBIT_DSLEEP, n)

// 0C   PMU_REG_USB_CFG
#define REG_LDO_VANA_LV_MODE                (1 << 15)
#define REG_PULLDOWN_VUSB                   (1 << 14)
#define LP_EN_VUSB_LDO_DSLEEP               (1 << 13)
#define LP_EN_VUSB_LDO_DR                   (1 << 12)
#define LP_EN_VUSB_LDO_REG                  (1 << 11)
#define PU_LDO_VUSB_DSLEEP                  (1 << 10)
#define PU_LDO_VUSB_DR                      (1 << 9)
#define PU_LDO_VUSB_REG                     (1 << 8)
#define LDO_VUSB_VBIT_NORMAL_SHIFT          4
#define LDO_VUSB_VBIT_NORMAL_MASK           (0xF << LDO_VUSB_VBIT_NORMAL_SHIFT)
#define LDO_VUSB_VBIT_NORMAL(n)             BITFIELD_VAL(LDO_VUSB_VBIT_NORMAL, n)
#define LDO_VUSB_VBIT_DSLEEP_SHIFT          0
#define LDO_VUSB_VBIT_DSLEEP_MASK           (0xF << LDO_VUSB_VBIT_DSLEEP_SHIFT)
#define LDO_VUSB_VBIT_DSLEEP(n)             BITFIELD_VAL(LDO_VUSB_VBIT_DSLEEP, n)

#define REG_PU_LDO_VUSB_DSLEEP              PU_LDO_VUSB_DSLEEP
#define REG_PU_LDO_VUSB_DR                  PU_LDO_VUSB_DR
#define REG_PU_LDO_VUSB_REG                 PU_LDO_VUSB_REG


#define LP_EN_VUSB1802_LDO_DSLEEP               (1 << 15)
#define LP_EN_VUSB1802_LDO_DR                   (1 << 14)
#define LP_EN_VUSB1802_LDO_REG                  (1 << 13)
#define PU_LDO_VUSB1802_DSLEEP                  (1 << 12)
#define PU_LDO_VUSB1802_DR                      (1 << 11)
#define PU_LDO_VUSB1802_REG                     (1 << 10)
#define LDO_VUSB1802_VBIT_NORMAL_SHIFT          5
#define LDO_VUSB1802_VBIT_NORMAL_MASK           (0x1F << LDO_VUSB1802_VBIT_NORMAL_SHIFT)
#define LDO_VUSB1802_VBIT_NORMAL(n)             BITFIELD_VAL(LDO_VUSB1802_VBIT_NORMAL, n)
#define LDO_VUSB1802_VBIT_DSLEEP_SHIFT          0
#define LDO_VUSB1802_VBIT_DSLEEP_MASK           (0x1F << LDO_VUSB1802_VBIT_DSLEEP_SHIFT)
#define LDO_VUSB1802_VBIT_DSLEEP(n)             BITFIELD_VAL(LDO_VUSB1802_VBIT_DSLEEP, n)

#define REG_PU_LDO_VUSB1802_DSLEEP              PU_LDO_VUSB1802_DSLEEP
#define REG_PU_LDO_VUSB1802_DR                  PU_LDO_VUSB1802_DR
#define REG_PU_LDO_VUSB1802_REG                 PU_LDO_VUSB1802_REG

// 0D   PMU_REG_BAT2DIG_CFG
#define LP_EN_VBAT2VCORE_LDO_DR             (1 << 15)
#define LP_EN_VBAT2VCORE_LDO                (1 << 14)
#define REG_PU_VBAT2VCORE_LDO_DR            (1 << 13)
#define REG_PU_VBAT2VCORE_LDO               (1 << 12)
#define LDO_VBAT2VCORE_VBIT_DSLEEP_SHIFT    6
#define LDO_VBAT2VCORE_VBIT_DSLEEP_MASK     (0x3F << LDO_VBAT2VCORE_VBIT_DSLEEP_SHIFT)
#define LDO_VBAT2VCORE_VBIT_DSLEEP(n)       BITFIELD_VAL(LDO_VBAT2VCORE_VBIT_DSLEEP, n)
#define LDO_VBAT2VCORE_VBIT_NORMAL_SHIFT    0
#define LDO_VBAT2VCORE_VBIT_NORMAL_MASK     (0x3F << LDO_VBAT2VCORE_VBIT_NORMAL_SHIFT)
#define LDO_VBAT2VCORE_VBIT_NORMAL(n)       BITFIELD_VAL(LDO_VBAT2VCORE_VBIT_NORMAL, n)

#define LP_EN_VBAT2VCORE_LDO_REG            LP_EN_VBAT2VCORE_LDO
#define REG_PU_LDO_VBAT2VCORE_DR            REG_PU_VBAT2VCORE_LDO_DR
#define REG_PU_LDO_VBAT2VCORE_REG           REG_PU_VBAT2VCORE_LDO
// Real bit is in REG_101 -- but we never change the default value
#define REG_PU_LDO_VBAT2VCORE_DSLEEP        0

// 0E   PMU_REG_HPPA_LDO_EN
#define REG_PULL_DOWN_VHPPA                 (1 << 15)
#define REG_PU_LDO_VHPPA_DSLEEP             (1 << 14)
#define REG_PU_LDO_VHPPA_EN                 (1 << 13)
#define REG_RES_SEL_VHPPA_SHIFT             8
#define REG_RES_SEL_VHPPA_MASK              (0x1F << REG_RES_SEL_VHPPA_SHIFT)
#define REG_RES_SEL_VHPPA(n)                BITFIELD_VAL(REG_RES_SEL_VHPPA, n)
#define REG_LP_BIAS_SEL_LDO_SHIFT           6
#define REG_LP_BIAS_SEL_LDO_MASK            (0x3 << REG_LP_BIAS_SEL_LDO_SHIFT)
#define REG_LP_BIAS_SEL_LDO(n)              BITFIELD_VAL(REG_LP_BIAS_SEL_LDO, n)
#define REG_BYPASS_VBUCK2ANA                (1 << 5)
#define REG_PULLDOWN_VBUCK2ANA              (1 << 4)
#define REG_PULLDOWN_VIO                    (1 << 3)
#define REG_PULLDOWN_VMEM                   (1 << 2)
#define REG_PULLDOWN_VCORE                  (1 << 1)
#define REG_PULLDOWN_VHPPA2VCODEC           (1 << 0)

// 0F   PMU_REG_HPPA2CODEC_CFG
#define PU_LDO_VCODEC_DSLEEP                (1 << 15)
#define REG_BYPASS_VCORE                    (1 << 14)
#define REG_PU_LDO_VHPPA2VCODEC_DR          (1 << 13)
#define REG_PU_LDO_VHPPA2VCODEC             (1 << 12)
#define LP_EN_VHPPA2VCODEC_LDO_DR           (1 << 11)
#define LP_EN_VHPPA2VCODEC_LDO              (1 << 10)
#define LDO_VHPPA2VCODEC_VBIT_DSLEEP_SHIFT  5
#define LDO_VHPPA2VCODEC_VBIT_DSLEEP_MASK   (0x1F << LDO_VHPPA2VCODEC_VBIT_DSLEEP_SHIFT)
#define LDO_VHPPA2VCODEC_VBIT_DSLEEP(n)     BITFIELD_VAL(LDO_VHPPA2VCODEC_VBIT_DSLEEP, n)
#define LDO_VHPPA2VCODEC_VBIT_NORMAL_SHIFT  0
#define LDO_VHPPA2VCODEC_VBIT_NORMAL_MASK   (0x1F << LDO_VHPPA2VCODEC_VBIT_NORMAL_SHIFT)
#define LDO_VHPPA2VCODEC_VBIT_NORMAL(n)     BITFIELD_VAL(LDO_VHPPA2VCODEC_VBIT_NORMAL, n)

#define REG_PU_LDO_VHPPA2VCODEC_REG         REG_PU_LDO_VHPPA2VCODEC
#define LP_EN_VHPPA2VCODEC_LDO_REG          LP_EN_VHPPA2VCODEC_LDO
#define REG_PU_LDO_VCODEC_DSLEEP            PU_LDO_VCODEC_DSLEEP

// 10   PMU_REG_CODEC_CFG
#define PU_LDO_VCODEC_DR                    (1 << 15)
#define PU_LDO_VCODEC_REG                   (1 << 14)
#define LP_EN_VCODEC_LDO_DSLEEP             (1 << 13)
#define LP_EN_VCODEC_LDO_DR                 (1 << 12)
#define LP_EN_VCODEC_LDO_REG                (1 << 11)
#define REG_PULLDOWN_VCODEC                 (1 << 10)
#define LDO_VCODEC_VBIT_NORMAL_SHIFT        5
#define LDO_VCODEC_VBIT_NORMAL_MASK         (0x1F << LDO_VCODEC_VBIT_NORMAL_SHIFT)
#define LDO_VCODEC_VBIT_NORMAL(n)           BITFIELD_VAL(LDO_VCODEC_VBIT_NORMAL, n)
#define LDO_VCODEC_VBIT_DSLEEP_SHIFT        0
#define LDO_VCODEC_VBIT_DSLEEP_MASK         (0x1F << LDO_VCODEC_VBIT_DSLEEP_SHIFT)
#define LDO_VCODEC_VBIT_DSLEEP(n)           BITFIELD_VAL(LDO_VCODEC_VBIT_DSLEEP, n)

#define REG_PU_LDO_VCODEC_DR                PU_LDO_VCODEC_DR
#define REG_PU_LDO_VCODEC_REG               PU_LDO_VCODEC_REG

// 15   PMU_REG_DCDC_DIG_EN
#define REG_UVLO_SEL_SHIFT                  14
#define REG_UVLO_SEL_MASK                   (0x3 << REG_UVLO_SEL_SHIFT)
#define REG_UVLO_SEL(n)                     BITFIELD_VAL(REG_UVLO_SEL, n)
#define REG_PU_LDO_DIG_DR                   (1 << 13)
#define REG_PU_LDO_DIG_REG                  (1 << 12)
#define REG_DCDC1_PFM_SEL_DSLEEP            (1 << 11)
#define REG_DCDC1_PFM_SEL_NORMAL            (1 << 10)
#define REG_DCDC1_ULP_MODE_DSLEEP           (1 << 9)
#define REG_DCDC1_ULP_MODE_NORMAL           (1 << 8)
#define REG_DCDC1_BURST_MODE_SEL_DSLEEP     (1 << 7)
#define REG_DCDC1_BURST_MODE_SEL_NORMAL     (1 << 6)
#define REG_DCDC1_VREF_SEL_DSLEEP_SHIFT     4
#define REG_DCDC1_VREF_SEL_DSLEEP_MASK      (0x3 << REG_DCDC1_VREF_SEL_DSLEEP_SHIFT)
#define REG_DCDC1_VREF_SEL_DSLEEP(n)        BITFIELD_VAL(REG_DCDC1_VREF_SEL_DSLEEP, n)
#define REG_DCDC1_VREF_SEL_NORMAL_SHIFT     2
#define REG_DCDC1_VREF_SEL_NORMAL_MASK      (0x3 << REG_DCDC1_VREF_SEL_NORMAL_SHIFT)
#define REG_DCDC1_VREF_SEL_NORMAL(n)        BITFIELD_VAL(REG_DCDC1_VREF_SEL_NORMAL, n)
#define REG_PU_DCDC1_DR                     (1 << 1)
#define REG_PU_DCDC1                        (1 << 0)

#define REG_PU_LDO_VCORE_DR                 REG_PU_LDO_DIG_DR
#define REG_PU_LDO_VCORE_REG                REG_PU_LDO_DIG_REG

// 17   PMU_REG_DCDC_ANA_CFG_17
#define PMU1802_1807_DCDC2_DELTAV_SHIFT     8
#define PMU1802_1807_DCDC2_DELTAV_MASK      (0x3F << PMU1802_1807_DCDC2_DELTAV_SHIFT)
#define PMU1802_1807_DCDC2_DELTAV(n)        BITFIELD_VAL(PMU1802_1807_DCDC2_DELTAV, n)

#define PMU1800_1803_DCDC2_DELTAV_SHIFT     8
#define PMU1800_1803_DCDC2_DELTAV_MASK      (0xF << PMU1800_1803_DCDC2_DELTAV_SHIFT)
#define PMU1800_1803_DCDC2_DELTAV(n)        BITFIELD_VAL(PMU1800_1803_DCDC2_DELTAV, n)

// 19   PMU_REG_DCDC_ANA_CFG_19
#define DCDC2_IX2_ERR                       (1 << 15)
#define DCDC2_IX2_IRCOMP                    (1 << 14)
#define DCDC2_LP_ERR                        (1 << 13)
#define DCDC2_LP_VCOMP                      (1 << 12)
#define DCDC2_PULLDOWN_EN                   (1 << 11)
#define DCDC2_SLOPE_DOUBLE                  (1 << 10)
#define DCDC2_SLOPE_EN_BURST                (1 << 9)
#define DCDC2_SOFT_START_EN                 (1 << 8)
#define DCDC2_SS_TIME_SEL                   (1 << 7)
#define DCDC2_SYNC_DISABLE                  (1 << 6)
#define DCDC2_TEST_MODE_EN                  (1 << 5)
#define DCDC2_ZCD_CAL_BIT_SHIFT             1
#define DCDC2_ZCD_CAL_BIT_MASK              (0xF << DCDC2_ZCD_CAL_BIT_SHIFT)
#define DCDC2_ZCD_CAL_BIT(n)                BITFIELD_VAL(DCDC2_ZCD_CAL_BIT, n)
#define DCDC2_REG_BYPASS                    (1 << 0)

// 1B   PMU_REG_DCDC_HPPA_CFG_1B
#define PMU1802_1807_DCDC3_DELTAV_SHIFT     8
#define PMU1802_1807_DCDC3_DELTAV_MASK      (0x3F << PMU1802_1807_DCDC3_DELTAV_SHIFT)
#define PMU1802_1807_DCDC3_DELTAV(n)        BITFIELD_VAL(PMU1802_1807_DCDC3_DELTAV, n)

#define PMU1800_1803_DCDC3_DELTAV_SHIFT     8
#define PMU1800_1803_DCDC3_DELTAV_MASK      (0xF << PMU1800_1803_DCDC3_DELTAV_SHIFT)
#define PMU1800_1803_DCDC3_DELTAV(n)        BITFIELD_VAL(PMU1800_1803_DCDC3_DELTAV, n)

// 1D   PMU_REG_DCDC_HPPA_CFG_1D
#define DCDC3_IX2_ERR                       (1 << 15)
#define DCDC3_IX2_IRCOMP                    (1 << 14)
#define DCDC3_LP_ERR                        (1 << 13)
#define DCDC3_LP_VCOMP                      (1 << 12)
#define DCDC3_PULLDOWN_EN                   (1 << 11)
#define DCDC3_SLOPE_DOUBLE                  (1 << 10)
#define DCDC3_SLOPE_EN_BURST                (1 << 9)
#define DCDC3_SOFT_START_EN                 (1 << 8)
#define DCDC3_SS_TIME_SEL                   (1 << 7)
#define DCDC3_SYNC_DISABLE                  (1 << 6)
#define DCDC3_TEST_MODE_EN                  (1 << 5)
#define DCDC3_ZCD_CAL_BIT_SHIFT             1
#define DCDC3_ZCD_CAL_BIT_MASK              (0xF << DCDC3_ZCD_CAL_BIT_SHIFT)
#define DCDC3_ZCD_CAL_BIT(n)                BITFIELD_VAL(DCDC3_ZCD_CAL_BIT, n)
#define DCDC3_REG_BYPASS                    (1 << 0)

// 21   PMU_REG_PWR_SEL
#define REG_PMU_VSEL1_SHIFT                 13
#define REG_PMU_VSEL1_MASK                  (0x7 << REG_PMU_VSEL1_SHIFT)
#define REG_PMU_VSEL1(n)                    BITFIELD_VAL(REG_PMU_VSEL1, n)
#define REG_POWER_SEL_CNT_SHIFT             8
#define REG_POWER_SEL_CNT_MASK              (0x1F << REG_POWER_SEL_CNT_SHIFT)
#define REG_POWER_SEL_CNT(n)                BITFIELD_VAL(REG_POWER_SEL_CNT, n)
#define REG_PWR_SEL_DR                      (1 << 7)
#define REG_PWR_SEL                         (1 << 6)
#define CLK_BG_EN                           (1 << 5)
#define CLK_BG_DIV_VALUE_SHIFT              0
#define CLK_BG_DIV_VALUE_MASK               (0x1F << CLK_BG_DIV_VALUE_SHIFT)
#define CLK_BG_DIV_VALUE(n)                 BITFIELD_VAL(CLK_BG_DIV_VALUE, n)

// 26   PMU_REG_INT_MASK
#define RESET_EN_VCORE_LOW                  (1 << 15)
#define RTC_INT1_MSK                        (1 << 14)
#define RTC_INT0_MSK                        (1 << 13)
#define KEY_ERR1_INTR_MSK                   (1 << 12)
#define KEY_ERR0_INTR_MSK                   (1 << 11)
#define KEY_PRESS_INTR_MSK                  (1 << 10)
#define KEY_RELEASE_INTR_MSK                (1 << 9)
#define SAMPLE_DONE_INTR_MSK                (1 << 8)
#define CHAN_DATA_INTR_MSK_SHIFT            0
#define CHAN_DATA_INTR_MSK_MASK             (0xFF << CHAN_DATA_INTR_MSK_SHIFT)
#define CHAN_DATA_INTR_MSK(n)               BITFIELD_VAL(CHAN_DATA_INTR_MSK, n)

// 27   PMU_REG_INT_EN
#define REG_BG_SLEEP_MSK                    (1 << 15)
#define RTC_INT_EN_1                        (1 << 14)
#define RTC_INT_EN_0                        (1 << 13)
#define KEY_ERR1_INTR_EN                    (1 << 12)
#define KEY_ERR0_INTR_EN                    (1 << 11)
#define KEY_PRESS_INTR_EN                   (1 << 10)
#define KEY_RELEASE_INTR_EN                 (1 << 9)
#define SAMPLE_DONE_INTR_EN                 (1 << 8)
#define CHAN_DATA_INTR_EN_SHIFT             0
#define CHAN_DATA_INTR_EN_MASK              (0xFF << CHAN_DATA_INTR_EN_SHIFT)
#define CHAN_DATA_INTR_EN(n)                BITFIELD_VAL(CHAN_DATA_INTR_EN, n)

// 34   PMU_REG_DCDC_DIG_CFG_34
#define PMU1802_1807_DCDC1_DELTAV_SHIFT     8
#define PMU1802_1807_DCDC1_DELTAV_MASK      (0x3F << PMU1802_1807_DCDC1_DELTAV_SHIFT)
#define PMU1802_1807_DCDC1_DELTAV(n)        BITFIELD_VAL(PMU1802_1807_DCDC1_DELTAV, n)

#define PMU1800_1803_DCDC1_DELTAV_SHIFT     8
#define PMU1800_1803_DCDC1_DELTAV_MASK      (0xF << PMU1800_1803_DCDC1_DELTAV_SHIFT)
#define PMU1800_1803_DCDC1_DELTAV(n)        BITFIELD_VAL(PMU1800_1803_DCDC1_DELTAV, n)

// 36   PMU_REG_DCDC_DIG_CFG_36
#define DCDC1_IX2_ERR                       (1 << 15)
#define DCDC1_IX2_IRCOMP                    (1 << 14)
#define DCDC1_LP_ERR                        (1 << 13)
#define DCDC1_LP_VCOMP                      (1 << 12)
#define DCDC1_PULLDOWN_EN                   (1 << 11)
#define DCDC1_SLOPE_DOUBLE                  (1 << 10)
#define DCDC1_SLOPE_EN_BURST                (1 << 9)
#define DCDC1_SOFT_START_EN                 (1 << 8)
#define DCDC1_SS_TIME_SEL                   (1 << 7)
#define DCDC1_SYNC_DISABLE                  (1 << 6)
#define DCDC1_TEST_MODE_EN                  (1 << 5)
#define DCDC1_ZCD_CAL_BIT_SHIFT             1
#define DCDC1_ZCD_CAL_BIT_MASK              (0xF << DCDC1_ZCD_CAL_BIT_SHIFT)
#define DCDC1_ZCD_CAL_BIT(n)                BITFIELD_VAL(DCDC1_ZCD_CAL_BIT, n)
#define DCDC1_REG_BYPASS                    (1 << 0)

// 38   PMU_REG_MIC_BIAS_E
#define REG_MIC_BIASE_CHANSEL_SHIFT         14
#define REG_MIC_BIASE_CHANSEL_MASK          (0x3 << REG_MIC_BIASE_CHANSEL_SHIFT)
#define REG_MIC_BIASE_CHANSEL(n)            BITFIELD_VAL(REG_MIC_BIASE_CHANSEL, n)
#define REG_MIC_BIASE_EN                    (1 << 13)
#define REG_MIC_BIASE_ENLPF                 (1 << 12)
#define REG_MIC_BIASE_LPFSEL_SHIFT          10
#define REG_MIC_BIASE_LPFSEL_MASK           (0x3 << REG_MIC_BIASE_LPFSEL_SHIFT)
#define REG_MIC_BIASE_LPFSEL(n)             BITFIELD_VAL(REG_MIC_BIASE_LPFSEL, n)
#define REG_MIC_BIASE_VSEL_SHIFT            5
#define REG_MIC_BIASE_VSEL_MASK             (0x1F << REG_MIC_BIASE_VSEL_SHIFT)
#define REG_MIC_BIASE_VSEL(n)               BITFIELD_VAL(REG_MIC_BIASE_VSEL, n)
#define REG_MIC_LDOE_RES_SHIFT              1
#define REG_MIC_LDOE_RES_MASK               (0xF << REG_MIC_LDOE_RES_SHIFT)
#define REG_MIC_LDOE_RES(n)                 BITFIELD_VAL(REG_MIC_LDOE_RES, n)
#define REG_MIC_LDOE_LOOPCTRL               (1 << 0)

// 39   PMU_REG_MIC_LDO_EN
#define PMU1800_REG_LED_IO1_AIO_EN          (1 << 15)
#define PMU1800_REG_LED_IO2_AIO_EN          (1 << 14)
#define PMU1800_REG_LED_IO1_RX_EN           (1 << 13)
#define PMU1800_REG_LED_IO2_RX_EN           (1 << 12)
#define PMU1800_REG_MIC_LDOA_EN             (1 << 11)
#define PMU1800_REG_MIC_LDOB_EN             (1 << 10)
#define PMU1800_REG_MIC_LDOC_EN             (1 << 9)
#define PMU1800_REG_MIC_LDOD_EN             (1 << 8)
#define PMU1800_REG_MIC_LDOE_EN             (1 << 7)
#define PMU1800_DCDC1_OFFSET_CURRENT_EN     (1 << 6)
#define PMU1800_DCDC2_OFFSET_CURRENT_EN     (1 << 5)
#define PMU1800_DCDC3_OFFSET_CURRENT_EN     (1 << 4)
#define PMU1800_DCDC4_OFFSET_CURRENT_EN     (1 << 3)

#define PMU1803_CLK_32K_COUNT_EN_TRIGGER    (1 << 12)
#define PMU1803_REG_LED_IO1_AIO_EN          (1 << 11)
#define PMU1803_REG_LED_IO2_AIO_EN          (1 << 10)
#define PMU1803_REG_LED_IO1_RX_EN           (1 << 9)
#define PMU1803_REG_LED_IO2_RX_EN           (1 << 8)
#define PMU1803_REG_MIC_LDOA_EN             (1 << 7)
#define PMU1803_REG_MIC_LDOB_EN             (1 << 6)
#define PMU1803_REG_MIC_LDOC_EN             (1 << 5)
#define PMU1803_REG_SAR_HIGH_AVDD18_EN      (1 << 4)
#define PMU1803_DCDC1_OFFSET_CURRENT_EN     (1 << 3)
#define PMU1803_DCDC2_OFFSET_CURRENT_EN     (1 << 2)
#define PMU1803_DCDC3_OFFSET_CURRENT_EN     (1 << 1)
#define PMU1803_DCDC4_OFFSET_CURRENT_EN     (1 << 0)

#define REG_LED_IO1_AIO_EN                  ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_LED_IO1_AIO_EN : PMU1800_REG_LED_IO1_AIO_EN)
#define REG_LED_IO2_AIO_EN                  ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_LED_IO2_AIO_EN : PMU1800_REG_LED_IO2_AIO_EN)
#define REG_LED_IO1_RX_EN                   ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_LED_IO1_RX_EN : PMU1800_REG_LED_IO1_RX_EN)
#define REG_LED_IO2_RX_EN                   ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_LED_IO2_RX_EN : PMU1800_REG_LED_IO2_RX_EN)
#define REG_MIC_LDOA_EN                     ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_MIC_LDOA_EN : PMU1800_REG_MIC_LDOA_EN)
#define REG_MIC_LDOB_EN                     ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_MIC_LDOB_EN : PMU1800_REG_MIC_LDOB_EN)
#define REG_MIC_LDOC_EN                     ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_MIC_LDOC_EN : PMU1800_REG_MIC_LDOC_EN)
#define REG_MIC_LDOD_EN                     ((pmu_type == PMU_CHIP_TYPE_1803) ? 0 : PMU1800_REG_MIC_LDOD_EN)
#define REG_MIC_LDOE_EN                     ((pmu_type == PMU_CHIP_TYPE_1803) ? 0 : PMU1800_REG_MIC_LDOE_EN)
#define DCDC1_OFFSET_CURRENT_EN             ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_DCDC1_OFFSET_CURRENT_EN : PMU1800_DCDC1_OFFSET_CURRENT_EN)
#define DCDC2_OFFSET_CURRENT_EN             ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_DCDC2_OFFSET_CURRENT_EN : PMU1800_DCDC2_OFFSET_CURRENT_EN)
#define DCDC3_OFFSET_CURRENT_EN             ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_DCDC3_OFFSET_CURRENT_EN : PMU1800_DCDC3_OFFSET_CURRENT_EN)
#define DCDC4_OFFSET_CURRENT_EN             ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_DCDC4_OFFSET_CURRENT_EN : PMU1800_DCDC4_OFFSET_CURRENT_EN)

// 3A   PMU_REG_LED_CFG
#define REG_LED_IO1_IBIT_SHIFT              14
#define REG_LED_IO1_IBIT_MASK               (0x3 << REG_LED_IO1_IBIT_SHIFT)
#define REG_LED_IO1_IBIT(n)                 BITFIELD_VAL(REG_LED_IO1_IBIT, n)
#define REG_LED_IO1_OENB                    (1 << 13)
#define REG_LED_IO1_PDEN                    (1 << 12)
#define REG_LED_IO1_PU                      (1 << 11)
#define REG_LED_IO1_PUEN                    (1 << 10)
#define REG_LED_IO1_SEL_SHIFT               8
#define REG_LED_IO1_SEL_MASK                (0x3 << REG_LED_IO1_SEL_SHIFT)
#define REG_LED_IO1_SEL(n)                  BITFIELD_VAL(REG_LED_IO1_SEL, n)
#define REG_LED_IO2_IBIT_SHIFT              6
#define REG_LED_IO2_IBIT_MASK               (0x3 << REG_LED_IO2_IBIT_SHIFT)
#define REG_LED_IO2_IBIT(n)                 BITFIELD_VAL(REG_LED_IO2_IBIT, n)
#define REG_LED_IO2_OENB                    (1 << 5)
#define REG_LED_IO2_PDEN                    (1 << 4)
#define REG_LED_IO2_PU                      (1 << 3)
#define REG_LED_IO2_PUEN                    (1 << 2)
#define REG_LED_IO2_SEL_SHIFT               0
#define REG_LED_IO2_SEL_MASK                (0x3 << REG_LED_IO2_SEL_SHIFT)
#define REG_LED_IO2_SEL(n)                  BITFIELD_VAL(REG_LED_IO2_SEL, n)

// 3B   PMU_REG_MIC_BIAS_A
#define REG_MIC_BIASA_CHANSEL_SHIFT         14
#define REG_MIC_BIASA_CHANSEL_MASK          (0x3 << REG_MIC_BIASA_CHANSEL_SHIFT)
#define REG_MIC_BIASA_CHANSEL(n)            BITFIELD_VAL(REG_MIC_BIASA_CHANSEL, n)
#define REG_MIC_BIASA_EN                    (1 << 13)
#define REG_MIC_BIASA_ENLPF                 (1 << 12)
#define REG_MIC_BIASA_LPFSEL_SHIFT          10
#define REG_MIC_BIASA_LPFSEL_MASK           (0x3 << REG_MIC_BIASA_LPFSEL_SHIFT)
#define REG_MIC_BIASA_LPFSEL(n)             BITFIELD_VAL(REG_MIC_BIASA_LPFSEL, n)
#define REG_MIC_BIASA_VSEL_SHIFT            5
#define REG_MIC_BIASA_VSEL_MASK             (0x1F << REG_MIC_BIASA_VSEL_SHIFT)
#define REG_MIC_BIASA_VSEL(n)               BITFIELD_VAL(REG_MIC_BIASA_VSEL, n)
#define REG_MIC_LDOA_RES_SHIFT              1
#define REG_MIC_LDOA_RES_MASK               (0xF << REG_MIC_LDOA_RES_SHIFT)
#define REG_MIC_LDOA_RES(n)                 BITFIELD_VAL(REG_MIC_LDOA_RES, n)
#define REG_MIC_LDOA_LOOPCTRL               (1 << 0)

// 3C   PMU_REG_MIC_BIAS_B
#define REG_MIC_BIASB_CHANSEL_SHIFT         14
#define REG_MIC_BIASB_CHANSEL_MASK          (0x3 << REG_MIC_BIASB_CHANSEL_SHIFT)
#define REG_MIC_BIASB_CHANSEL(n)            BITFIELD_VAL(REG_MIC_BIASB_CHANSEL, n)
#define REG_MIC_BIASB_EN                    (1 << 13)
#define REG_MIC_BIASB_ENLPF                 (1 << 12)
#define REG_MIC_BIASB_LPFSEL_SHIFT          10
#define REG_MIC_BIASB_LPFSEL_MASK           (0x3 << REG_MIC_BIASB_LPFSEL_SHIFT)
#define REG_MIC_BIASB_LPFSEL(n)             BITFIELD_VAL(REG_MIC_BIASB_LPFSEL, n)
#define REG_MIC_BIASB_VSEL_SHIFT            5
#define REG_MIC_BIASB_VSEL_MASK             (0x1F << REG_MIC_BIASB_VSEL_SHIFT)
#define REG_MIC_BIASB_VSEL(n)               BITFIELD_VAL(REG_MIC_BIASB_VSEL, n)
#define REG_MIC_LDOB_RES_SHIFT              1
#define REG_MIC_LDOB_RES_MASK               (0xF << REG_MIC_LDOB_RES_SHIFT)
#define REG_MIC_LDOB_RES(n)                 BITFIELD_VAL(REG_MIC_LDOB_RES, n)
#define REG_MIC_LDOB_LOOPCTRL               (1 << 0)

// 3D   PMU_REG_MIC_BIAS_C
#define REG_MIC_BIASC_CHANSEL_SHIFT         14
#define REG_MIC_BIASC_CHANSEL_MASK          (0x3 << REG_MIC_BIASC_CHANSEL_SHIFT)
#define REG_MIC_BIASC_CHANSEL(n)            BITFIELD_VAL(REG_MIC_BIASC_CHANSEL, n)
#define REG_MIC_BIASC_EN                    (1 << 13)
#define REG_MIC_BIASC_ENLPF                 (1 << 12)
#define REG_MIC_BIASC_LPFSEL_SHIFT          10
#define REG_MIC_BIASC_LPFSEL_MASK           (0x3 << REG_MIC_BIASC_LPFSEL_SHIFT)
#define REG_MIC_BIASC_LPFSEL(n)             BITFIELD_VAL(REG_MIC_BIASC_LPFSEL, n)
#define REG_MIC_BIASC_VSEL_SHIFT            5
#define REG_MIC_BIASC_VSEL_MASK             (0x1F << REG_MIC_BIASC_VSEL_SHIFT)
#define REG_MIC_BIASC_VSEL(n)               BITFIELD_VAL(REG_MIC_BIASC_VSEL, n)
#define REG_MIC_LDOC_RES_SHIFT              1
#define REG_MIC_LDOC_RES_MASK               (0xF << REG_MIC_LDOC_RES_SHIFT)
#define REG_MIC_LDOC_RES(n)                 BITFIELD_VAL(REG_MIC_LDOC_RES, n)
#define REG_MIC_LDOC_LOOPCTRL               (1 << 0)

// 3E   PMU_REG_MIC_BIAS_D
#define REG_MIC_BIASD_CHANSEL_SHIFT         14
#define REG_MIC_BIASD_CHANSEL_MASK          (0x3 << REG_MIC_BIASD_CHANSEL_SHIFT)
#define REG_MIC_BIASD_CHANSEL(n)            BITFIELD_VAL(REG_MIC_BIASD_CHANSEL, n)
#define REG_MIC_BIASD_EN                    (1 << 13)
#define REG_MIC_BIASD_ENLPF                 (1 << 12)
#define REG_MIC_BIASD_LPFSEL_SHIFT          10
#define REG_MIC_BIASD_LPFSEL_MASK           (0x3 << REG_MIC_BIASD_LPFSEL_SHIFT)
#define REG_MIC_BIASD_LPFSEL(n)             BITFIELD_VAL(REG_MIC_BIASD_LPFSEL, n)
#define REG_MIC_BIASD_VSEL_SHIFT            5
#define REG_MIC_BIASD_VSEL_MASK             (0x1F << REG_MIC_BIASD_VSEL_SHIFT)
#define REG_MIC_BIASD_VSEL(n)               BITFIELD_VAL(REG_MIC_BIASD_VSEL, n)
#define REG_MIC_LDOD_RES_SHIFT              1
#define REG_MIC_LDOD_RES_MASK               (0xF << REG_MIC_LDOD_RES_SHIFT)
#define REG_MIC_LDOD_RES(n)                 BITFIELD_VAL(REG_MIC_LDOD_RES, n)
#define REG_MIC_LDOD_LOOPCTRL               (1 << 0)

// 3F   PMU_REG_EFUSE_CTRL
#define TSMC_EFUSE_A_SHIFT                  0
#define TSMC_EFUSE_A_MASK                   (0x3F << TSMC_EFUSE_A_SHIFT)
#define TSMC_EFUSE_A(n)                     BITFIELD_VAL(TSMC_EFUSE_A, n)
#define TSMC_EFUSE_CLK_EN                   (1 << 8)
#define TSMC_EFUSE_NR                       (1 << 9)
#define TSMC_EFUSE_MR                       (1 << 10)
#define MANUAL_MODE                         (1 << 11)
#define TSMC_EFUSE_PGENB                    (1 << 12)
#define REG_MARGIN_READ                     (1 << 13)

// 40   PMU_REG_EFUSE_SEL
#define TSMC_EFUSE_STROBE_TRIG              (1 << 0)
#define TSMC_EFUSE_STROBE_WIDTH_SHIFT       1
#define TSMC_EFUSE_STROBE_WIDTH_MASK        (0xFF << TSMC_EFUSE_STROBE_WIDTH_SHIFT)
#define TSMC_EFUSE_STROBE_WIDTH(n)          BITFIELD_VAL(TSMC_EFUSE_STROBE_WIDTH, n)
#define REG_READ_STROBE_WIDTH_SHIFT         9
#define REG_READ_STROBE_WIDTH_MASK          (0xF << REG_READ_STROBE_WIDTH_SHIFT)
#define REG_READ_STROBE_WIDTH(n)            BITFIELD_VAL(REG_READ_STROBE_WIDTH, n)
#define EFUSE_SEL_SHIFT                     13
#define EFUSE_SEL_MASK                      (0x7 << EFUSE_SEL_SHIFT)
#define EFUSE_SEL(n)                        BITFIELD_VAL(EFUSE_SEL, n)

// 41   PMU_REG_DCDC1_OFFSET
#define DCDC1_OFFSET_BIT_SHIFT              8
#define DCDC1_OFFSET_BIT_MASK               (0xFF << DCDC1_OFFSET_BIT_SHIFT)
#define DCDC1_OFFSET_BIT(n)                 BITFIELD_VAL(DCDC1_OFFSET_BIT, n)
#define DCDC2_OFFSET_BIT_SHIFT              0
#define DCDC2_OFFSET_BIT_MASK               (0xFF << DCDC2_OFFSET_BIT_SHIFT)
#define DCDC2_OFFSET_BIT(n)                 BITFIELD_VAL(DCDC2_OFFSET_BIT, n)

// 43   PMU_REG_BUCK2ANA_CFG
#define REG_PU_VBUCK2ANA_LDO_DR             (1 << 15)
#define REG_PU_VBUCK2ANA_LDO                (1 << 14)
#define LP_EN_VBUCK2ANA_LDO_DR              (1 << 13)
#define LP_EN_VBUCK2ANA_LDO                 (1 << 12)
#define LDO_VBUCK2ANA_VBIT_DSLEEP_SHIFT     6
#define LDO_VBUCK2ANA_VBIT_DSLEEP_MASK      (0x3F << LDO_VBUCK2ANA_VBIT_DSLEEP_SHIFT)
#define LDO_VBUCK2ANA_VBIT_DSLEEP(n)        BITFIELD_VAL(LDO_VBUCK2ANA_VBIT_DSLEEP, n)
#define LDO_VBUCK2ANA_VBIT_NORMAL_SHIFT     0
#define LDO_VBUCK2ANA_VBIT_NORMAL_MASK      (0x3F << LDO_VBUCK2ANA_VBIT_NORMAL_SHIFT)
#define LDO_VBUCK2ANA_VBIT_NORMAL(n)        BITFIELD_VAL(LDO_VBUCK2ANA_VBIT_NORMAL, n)

#define REG_PU_LDO_VBUCK2ANA_DR             REG_PU_VBUCK2ANA_LDO_DR
#define REG_PU_LDO_VBUCK2ANA_REG            REG_PU_VBUCK2ANA_LDO
#define LP_EN_VBUCK2ANA_LDO_REG             LP_EN_VBUCK2ANA_LDO
// No pu dsleep cfg
#define REG_PU_LDO_VBUCK2ANA_DSLEEP         0

// 44   PMU_REG_SLEEP_CFG
#define PMIC_TMODE_1300_SHIFT               0
#define PMIC_TMODE_1300_MASK                (0x7 << PMIC_TMODE_1300_SHIFT)
#define PMIC_TMODE_1300(n)                  BITFIELD_VAL(PMIC_TMODE_1300, n)
#define POWERON_PRESS_EN                    (1 << 3)
#define POWERON_RELEASE_EN                  (1 << 4)
#define SLEEP_ALLOW                         (1 << 5)
#define REG_VHPPA2VCODEC_SLP_ON             (1 << 6)

#define REG_PU_LDO_VHPPA2VCODEC_DSLEEP      REG_VHPPA2VCODEC_SLP_ON

// 46   PMU_REG_DCDC_DIG_VOLT
#define REG_DCDC1_VBIT_DSLEEP_SHIFT         8
#define REG_DCDC1_VBIT_DSLEEP_MASK          (0xFF << REG_DCDC1_VBIT_DSLEEP_SHIFT)
#define REG_DCDC1_VBIT_DSLEEP(n)            BITFIELD_VAL(REG_DCDC1_VBIT_DSLEEP, n)
#define REG_DCDC1_VBIT_NORMAL_SHIFT         0
#define REG_DCDC1_VBIT_NORMAL_MASK          (0xFF << REG_DCDC1_VBIT_NORMAL_SHIFT)
#define REG_DCDC1_VBIT_NORMAL(n)            BITFIELD_VAL(REG_DCDC1_VBIT_NORMAL, n)
#define MAX_DCDC1_VBIT_VAL                  (REG_DCDC1_VBIT_NORMAL_MASK >> REG_DCDC1_VBIT_NORMAL_SHIFT)

// 47   PMU_REG_DCDC_ANA_VOLT
#define REG_DCDC2_VBIT_DSLEEP_SHIFT         8
#define REG_DCDC2_VBIT_DSLEEP_MASK          (0xFF << REG_DCDC2_VBIT_DSLEEP_SHIFT)
#define REG_DCDC2_VBIT_DSLEEP(n)            BITFIELD_VAL(REG_DCDC2_VBIT_DSLEEP, n)
#define REG_DCDC2_VBIT_NORMAL_SHIFT         0
#define REG_DCDC2_VBIT_NORMAL_MASK          (0xFF << REG_DCDC2_VBIT_NORMAL_SHIFT)
#define REG_DCDC2_VBIT_NORMAL(n)            BITFIELD_VAL(REG_DCDC2_VBIT_NORMAL, n)
#define MAX_DCDC2_VBIT_VAL                  (REG_DCDC2_VBIT_NORMAL_MASK >> REG_DCDC2_VBIT_NORMAL_SHIFT)

// 48   PMU_REG_DCDC_HPPA_VOLT
#define REG_DCDC3_VBIT_DSLEEP_SHIFT         8
#define REG_DCDC3_VBIT_DSLEEP_MASK          (0xFF << REG_DCDC3_VBIT_DSLEEP_SHIFT)
#define REG_DCDC3_VBIT_DSLEEP(n)            BITFIELD_VAL(REG_DCDC3_VBIT_DSLEEP, n)
#define REG_DCDC3_VBIT_NORMAL_SHIFT         0
#define REG_DCDC3_VBIT_NORMAL_MASK          (0xFF << REG_DCDC3_VBIT_NORMAL_SHIFT)
#define REG_DCDC3_VBIT_NORMAL(n)            BITFIELD_VAL(REG_DCDC3_VBIT_NORMAL, n)
#define MAX_DCDC3_VBIT_VAL                  (REG_DCDC3_VBIT_NORMAL_MASK >> REG_DCDC3_VBIT_NORMAL_SHIFT)

// 49   PMU_REG_DCDC4_VOLT
#define REG_DCDC4_VBIT_DSLEEP_SHIFT         8
#define REG_DCDC4_VBIT_DSLEEP_MASK          (0xFF << REG_DCDC4_VBIT_DSLEEP_SHIFT)
#define REG_DCDC4_VBIT_DSLEEP(n)            BITFIELD_VAL(REG_DCDC4_VBIT_DSLEEP, n)
#define REG_DCDC4_VBIT_NORMAL_SHIFT         0
#define REG_DCDC4_VBIT_NORMAL_MASK          (0xFF << REG_DCDC4_VBIT_NORMAL_SHIFT)
#define REG_DCDC4_VBIT_NORMAL(n)            BITFIELD_VAL(REG_DCDC4_VBIT_NORMAL, n)

// 4A   PMU_REG_DCDC_ANA_EN
#define REG_DCDC2_PFM_SEL_DSLEEP            (1 << 11)
#define REG_DCDC2_PFM_SEL_NORMAL            (1 << 10)
#define REG_DCDC2_ULP_MODE_DSLEEP           (1 << 9)
#define REG_DCDC2_ULP_MODE_NORMAL           (1 << 8)
#define REG_DCDC2_BURST_MODE_SEL_DSLEEP     (1 << 7)
#define REG_DCDC2_BURST_MODE_SEL_NORMAL     (1 << 6)
#define REG_DCDC2_VREF_SEL_DSLEEP_SHIFT     4
#define REG_DCDC2_VREF_SEL_DSLEEP_MASK      (0x3 << REG_DCDC2_VREF_SEL_DSLEEP_SHIFT)
#define REG_DCDC2_VREF_SEL_DSLEEP(n)        BITFIELD_VAL(REG_DCDC2_VREF_SEL_DSLEEP, n)
#define REG_DCDC2_VREF_SEL_NORMAL_SHIFT     2
#define REG_DCDC2_VREF_SEL_NORMAL_MASK      (0x3 << REG_DCDC2_VREF_SEL_NORMAL_SHIFT)
#define REG_DCDC2_VREF_SEL_NORMAL(n)        BITFIELD_VAL(REG_DCDC2_VREF_SEL_NORMAL, n)
#define REG_PU_DCDC2_DR                     (1 << 1)
#define REG_PU_DCDC2                        (1 << 0)

// 4B   PMU_REG_DCDC_HPPA_EN
#define REG_DCDC3_PFM_SEL_DSLEEP            (1 << 11)
#define REG_DCDC3_PFM_SEL_NORMAL            (1 << 10)
#define REG_DCDC3_ULP_MODE_DSLEEP           (1 << 9)
#define REG_DCDC3_ULP_MODE_NORMAL           (1 << 8)
#define REG_DCDC3_BURST_MODE_SEL_DSLEEP     (1 << 7)
#define REG_DCDC3_BURST_MODE_SEL_NORMAL     (1 << 6)
#define REG_DCDC3_VREF_SEL_DSLEEP_SHIFT     4
#define REG_DCDC3_VREF_SEL_DSLEEP_MASK      (0x3 << REG_DCDC3_VREF_SEL_DSLEEP_SHIFT)
#define REG_DCDC3_VREF_SEL_DSLEEP(n)        BITFIELD_VAL(REG_DCDC3_VREF_SEL_DSLEEP, n)
#define REG_DCDC3_VREF_SEL_NORMAL_SHIFT     2
#define REG_DCDC3_VREF_SEL_NORMAL_MASK      (0x3 << REG_DCDC3_VREF_SEL_NORMAL_SHIFT)
#define REG_DCDC3_VREF_SEL_NORMAL(n)        BITFIELD_VAL(REG_DCDC3_VREF_SEL_NORMAL, n)
#define REG_PU_DCDC3_DR                     (1 << 1)
#define REG_PU_DCDC3                        (1 << 0)

// 4C   PMU_REG_DCDC4_EN
#define REG_DCDC4_PFM_SEL_DSLEEP            (1 << 11)
#define REG_DCDC4_PFM_SEL_NORMAL            (1 << 10)
#define REG_DCDC4_ULP_MODE_DSLEEP           (1 << 9)
#define REG_DCDC4_ULP_MODE_NORMAL           (1 << 8)
#define REG_DCDC4_BURST_MODE_SEL_DSLEEP     (1 << 7)
#define REG_DCDC4_BURST_MODE_SEL_NORMAL     (1 << 6)
#define REG_DCDC4_VREF_SEL_DSLEEP_SHIFT     4
#define REG_DCDC4_VREF_SEL_DSLEEP_MASK      (0x3 << REG_DCDC4_VREF_SEL_DSLEEP_SHIFT)
#define REG_DCDC4_VREF_SEL_DSLEEP(n)        BITFIELD_VAL(REG_DCDC4_VREF_SEL_DSLEEP, n)
#define REG_DCDC4_VREF_SEL_NORMAL_SHIFT     2
#define REG_DCDC4_VREF_SEL_NORMAL_MASK      (0x3 << REG_DCDC4_VREF_SEL_NORMAL_SHIFT)
#define REG_DCDC4_VREF_SEL_NORMAL(n)        BITFIELD_VAL(REG_DCDC4_VREF_SEL_NORMAL, n)
#define REG_PU_DCDC4_DR                     (1 << 1)
#define REG_PU_DCDC4                        (1 << 0)

// 4F   PMU_REG_POWER_OFF
#define SOFT_POWER_OFF                      (1 << 0)
#define HARDWARE_POWER_OFF_EN               (1 << 1)
#define AC_ON_EN                            (1 << 2)
#define RC_CAL_READY                        (1 << 3)
#define KEY_START                           (1 << 4)
#define GPADC_START                         (1 << 5)
#define EFUSE_READ_DONE                     (1 << 6)
#define EFUSE_READ_BUSY                     (1 << 7)
#define VBAT_OVP                            (1 << 8)
#define VCORE_LOW                           (1 << 9)
#define REG_LED_IO1_DATA_IN                 (1 << 10)
#define REG_LED_IO2_DATA_IN                 (1 << 11)
#define RD_EFUSE_REG                        (1 << 12)
#define REG_WDT_LOAD                        (1 << 13)

#define EFUSE_READ_TRIG_SHIFT               (12)
#define EFUSE_READ_TRIG_MASK                (0xF << EFUSE_READ_TRIG_SHIFT)
#define EFUSE_READ_TRIG(n)                  BITFIELD_VAL(EFUSE_READ_TRIG, n)
#define EFUSE_READ_TRIG_WORD                (0xA)

// 52   PMU_REG_INT_STATUS
#define RD_RES1                             (1 << 15)
#define RTC_INT_1                           (1 << 14)
#define RTC_INT_0                           (1 << 13)
#define KEY_ERR1_INTR                       (1 << 12)
#define KEY_ERR0_INTR                       (1 << 11)
#define KEY_PRESS_INTR                      (1 << 10)
#define KEY_RELEASE_INTR                    (1 << 9)
#define SAMPLE_PERIOD_DONE_INTR             (1 << 8)
#define CHAN_DATA_VALID_INTR_SHIFT          0
#define CHAN_DATA_VALID_INTR_MASK           (0xFF << CHAN_DATA_VALID_INTR_SHIFT)
#define CHAN_DATA_VALID_INTR(n)             BITFIELD_VAL(CHAN_DATA_VALID_INTR, n)

// 53   PMU_REG_INT_MSKED_STATUS
// 51   PMU_REG_INT_CLR
#define PMIC_CODEC_PWM                      (1 << 15)
#define RTC_INT1_MSKED                      (1 << 14)
#define RTC_INT0_MSKED                      (1 << 13)
#define KEY_ERR1_INTR_MSKED                 (1 << 12)
#define KEY_ERR0_INTR_MSKED                 (1 << 11)
#define KEY_PRESS_INTR_MSKED                (1 << 10)
#define KEY_RELEASE_INTR_MSKED              (1 << 9)
#define SAMPLE_DONE_INTR_MSKED              (1 << 8)
#define CHAN_DATA_INTR_MSKED_SHIFT          0
#define CHAN_DATA_INTR_MSKED_MASK           (0xFF << CHAN_DATA_INTR_MSKED_SHIFT)
#define CHAN_DATA_INTR_MSKED(n)             BITFIELD_VAL(CHAN_DATA_INTR_MSKED, n)

// 5E   PMU_REG_CHARGER_STATUS
#define DIG_PU_VHPPA                        (1 << 15)
#define DIG_PU_VRTC_RF                      (1 << 14)
#define DIG_PU_VCODEC                       (1 << 13)
#define DIG_PU_VUSB                         (1 << 12)
#define POWER_ON_RELEASE                    (1 << 11)
#define POWER_ON_PRESS                      (1 << 10)
#define POWER_ON                            (1 << 9)
#define DEEPSLEEP_MODE                      (1 << 8)
//#define PMU_LDO_ON                          (1 << 7)
#define PU_OSC_OUT                          (1 << 6)
#define UVLO_LV                             (1 << 5)
#define AC_ON_DET_OUT_MASKED                (1 << 4)
#define AC_ON_DET_IN_MASKED                 (1 << 3)
#define AC_ON                               (1 << 2)
#define AC_ON_DET_OUT                       (1 << 1)
#define AC_ON_DET_IN                        (1 << 0)

// 6F PMU1803_REG_PMIC_UART_CFG
#define REG_PMIC_UART_OENB_SEL              (1 << 11)
#define REG_PMIC_UART_OENB2                 (1 << 10)
#define REG_PMIC_UART_TX2                   (1 << 9)
#define REG_PMIC_UART_DR2                   (1 << 8)
#define REG_LED_IO2_OENB_PRE                (1 << 7)
#define REG_LED_IO1_OENB_PRE                (1 << 6)
#define REG_PMIC_UART_OENB1                 (1 << 5)
#define REG_PMIC_UART_TX1                   (1 << 4)
#define REG_PMIC_UART_DR1                   (1 << 3)
#define REG_UART_LED2_SEL                   (1 << 2)
#define REG_UART_LED1_SEL                   (1 << 1)
#define REG_GPIO_I_SEL                      (1 << 0)

// 101  PMU_REG_DCDC_RAMP_EN
#define REG_MIC_BIASA_IX2                   (1 << 15)
#define REG_MIC_BIASB_IX2                   (1 << 14)
#define REG_MIC_BIASC_IX2                   (1 << 13)
#define REG_MIC_BIASD_IX2                   (1 << 12)
#define REG_MIC_BIASE_IX2                   (1 << 11)
#define REG_PULLDOWN_VGP                    (1 << 10)
#define REG_PU_LDO_DIG_DSLEEP               (1 << 9)
#define LDO_VHPPA2VCODEC_BYPASS             (1 << 8)
#define LDO_VGP_LV_MODE                     (1 << 7)
#define IPTAT_EN                            (1 << 6)
#define REG_PU_AVDD25_ANA                   (1 << 5)
#define REG_DCDC1_RAMP_EN                   (1 << 4)
#define REG_DCDC2_RAMP_EN                   (1 << 3)
#define REG_DCDC3_RAMP_EN                   (1 << 2)
#define REG_PU_VMEM_DELAY_DR                (1 << 1)
#define REG_PU_VMEM_DELAY                   (1 << 0)

// 102  PMU_REG_RESERVED_PMU
#define REG_ENABLE_CHARGER                  (1 << 13)

// 107  PMU_REG_SUBCNT_DATA
#define SUBCNT_DATA2_SHIFT                  8
#define SUBCNT_DATA2_MASK                   (0xFF << SUBCNT_DATA2_SHIFT)
#define SUBCNT_DATA2(n)                     BITFIELD_VAL(SUBCNT_DATA2, n)
#define SUBCNT_DATA3_SHIFT                  0
#define SUBCNT_DATA3_MASK                   (0xFF << SUBCNT_DATA3_SHIFT)
#define SUBCNT_DATA3(n)                     BITFIELD_VAL(SUBCNT_DATA3, n)

// 108  PMU_REG_PWM_BR_EN
#define TG_SUBCNT_D2_ST_SHIFT               9
#define TG_SUBCNT_D2_ST_MASK                (0x7F << TG_SUBCNT_D2_ST_SHIFT)
#define TG_SUBCNT_D2_ST(n)                  BITFIELD_VAL(TG_SUBCNT_D2_ST, n)
#define REG_PWM2_BR_EN                      (1 << 8)
#define TG_SUBCNT_D3_ST_SHIFT               1
#define TG_SUBCNT_D3_ST_MASK                (0x7F << TG_SUBCNT_D3_ST_SHIFT)
#define TG_SUBCNT_D3_ST(n)                  BITFIELD_VAL(TG_SUBCNT_D3_ST, n)
#define REG_PWM3_BR_EN                      (1 << 0)

// 109  PMU_REG_PWM_EN
#define REG_LED0_OUT                        (1 << 15)
#define REG_LED1_OUT                        (1 << 14)
#define PWM_SELECT_EN_SHIFT                 12
#define PWM_SELECT_EN_MASK                  (0x3 << PWM_SELECT_EN_SHIFT)
#define PWM_SELECT_EN(n)                    BITFIELD_VAL(PWM_SELECT_EN, n)
#define PWM_SELECT_INV_SHIFT                10
#define PWM_SELECT_INV_MASK                 (0x3 << PWM_SELECT_INV_SHIFT)
#define PWM_SELECT_INV(n)                   BITFIELD_VAL(PWM_SELECT_INV, n)
#define REG_CLK_PWM_DIV_SHIFT               5
#define REG_CLK_PWM_DIV_MASK                (0x1F << REG_CLK_PWM_DIV_SHIFT)
#define REG_CLK_PWM_DIV(n)                  BITFIELD_VAL(REG_CLK_PWM_DIV, n)

// 10B  PMU_REG_WDT_TIMER
#define REG_WDT_TIMER_SHIFT                 0
#define REG_WDT_TIMER_MASK                  (0xFFFF << REG_WDT_TIMER_SHIFT)
#define REG_WDT_TIMER(n)                    BITFIELD_VAL(REG_WDT_TIMER, n)

// 10C  PMU_REG_WDT_CFG
#define REG_HW_RESET_TIME_SHIFT             10
#define REG_HW_RESET_TIME_MASK              (0x3F << REG_HW_RESET_TIME_SHIFT)
#define REG_HW_RESET_TIME(n)                BITFIELD_VAL(REG_HW_RESET_TIME, n)
#define REG_HW_RESET_EN                     (1 << 9)
#define REG_WDT_RESET_EN                    (1 << 8)
#define REG_WDT_EN                          (1 << 7)
#define BG_R_TEMP_SHIFT                     4
#define BG_R_TEMP_MASK                      (0x7 << BG_R_TEMP_SHIFT)
#define BG_R_TEMP(n)                        BITFIELD_VAL(BG_R_TEMP, n)
#define BG_TRIM_VBG_SHIFT                   1
#define BG_TRIM_VBG_MASK                    (0x7 << BG_TRIM_VBG_SHIFT)
#define BG_TRIM_VBG(n)                      BITFIELD_VAL(BG_TRIM_VBG, n)
#define SAR_PU_OVP                          (1 << 0)

// 80   PMU_REG_RF_80
#define RF_80_PAGE_IDX                      (1 << 15)
#define RF_80_S_DONE_SHIFT                  4
#define RF_80_S_DONE_MASK                   (0x7FF << RF_80_S_DONE_SHIFT)
#define RF_80_S_DONE(n)                     BITFIELD_VAL(RF_80_S_DONE, n)
#define RF_80_REVID_SHIFT                   0
#define RF_80_REVID_MASK                    (0xF << RF_80_REVID_SHIFT)
#define RF_80_REVID(n)                      BITFIELD_VAL(RF_80_REVID, n)

// BD   PMU_REG_RF_BD
#define REG_BBPLL_FREQ_34_32_SHIFT          0
#define REG_BBPLL_FREQ_34_32_MASK           (0x7 << REG_BBPLL_FREQ_34_32_SHIFT)
#define REG_BBPLL_FREQ_34_32(n)             BITFIELD_VAL(REG_BBPLL_FREQ_34_32, n)
#define REG_BBPLL_FREQ_EN                   (1 << 3)

// 116  PMU_REG_PU_LDO
#define REG_PU_LDO_DIG_L_DSLEEP             (1 << 15)
#define REG_PU_LDO_DIG_M_DSLEEP             (1 << 14)
#define REG_PU_LDO_DIG_L_DR                 (1 << 13)
#define REG_PU_LDO_DIG_L_REG                (1 << 12)
#define REG_PU_LDO_DIG_M_DR                 (1 << 11)
#define REG_PU_LDO_DIG_M_REG                (1 << 10)
#define REG_PU_VMEM_RET_LDO                 (1 << 9)
#define REG_PU_VMEM_RET_LDO_DR              (1 << 8)
#define REG_PU_VIO_RET_LDO                  (1 << 7)
#define REG_PU_VIO_RET_LDO_DR               (1 << 6)
#define REG_PU_VDCDC_RET_LDO                (1 << 5)
#define REG_PU_VDCDC_RET_LDO_DR             (1 << 4)
#define REG_PU_VCORE_RET_LDO                (1 << 3)
#define REG_PU_VCORE_RET_LDO_DR             (1 << 2)
#define REG_PU_VBAT2VCORE_RET_LDO           (1 << 1)
#define REG_PU_VBAT2VCORE_RET_LDO_DR        (1 << 0)

// 11A  PMU_REG_LDO_VCORE_L
#define REG_VCORE_L_BYPASS_DLY_CNT_SHIFT    8
#define REG_VCORE_L_BYPASS_DLY_CNT_MASK     (0xFF << REG_VCORE_L_BYPASS_DLY_CNT_SHIFT)
#define REG_VCORE_L_BYPASS_DLY_CNT(n)       BITFIELD_VAL(REG_VCORE_L_BYPASS_DLY_CNT, n)
#define REG_LDO_VCORE_L_BYPASS              (1 << 2)
#define REG_LDO_VCORE_L_BYPASS_DR           (1 << 1)
//#define REG_PU_LDO_VBAT2VCORE_DSLEEP        (1 << 0)

// 11C  PMU_REG_DCDC1_EDGE
#define REG_DCDC1_EDGE_CON_P_SHIFT          3
#define REG_DCDC1_EDGE_CON_P_MASK           (0x3 << REG_DCDC1_EDGE_CON_P_SHIFT)
#define REG_DCDC1_EDGE_CON_P(n)             BITFIELD_VAL(REG_DCDC1_EDGE_CON_P, n)
#define REG_DCDC1_EDGE_CON_N_SHIFT          5
#define REG_DCDC1_EDGE_CON_N_MASK           (0x3 << REG_DCDC1_EDGE_CON_N_SHIFT)
#define REG_DCDC1_EDGE_CON_N(n)             BITFIELD_VAL(REG_DCDC1_EDGE_CON_N, n)

// 11D  PMU_REG_DCDC2_EDGE
#define REG_DCDC2_EDGE_CON_P_SHIFT          3
#define REG_DCDC2_EDGE_CON_P_MASK           (0x3 << REG_DCDC2_EDGE_CON_P_SHIFT)
#define REG_DCDC2_EDGE_CON_P(n)             BITFIELD_VAL(REG_DCDC2_EDGE_CON_P, n)
#define REG_DCDC2_EDGE_CON_N_SHIFT          5
#define REG_DCDC2_EDGE_CON_N_MASK           (0x3 << REG_DCDC2_EDGE_CON_N_SHIFT)
#define REG_DCDC2_EDGE_CON_N(n)             BITFIELD_VAL(REG_DCDC2_EDGE_CON_N, n)

// 11E  PMU_REG_DCDC3_EDGE
#define REG_DCDC3_EDGE_CON_P_SHIFT          3
#define REG_DCDC3_EDGE_CON_P_MASK           (0x3 << REG_DCDC3_EDGE_CON_P_SHIFT)
#define REG_DCDC3_EDGE_CON_P(n)             BITFIELD_VAL(REG_DCDC3_EDGE_CON_P, n)
#define REG_DCDC3_EDGE_CON_N_SHIFT          5
#define REG_DCDC3_EDGE_CON_N_MASK           (0x3 << REG_DCDC3_EDGE_CON_N_SHIFT)
#define REG_DCDC3_EDGE_CON_N(n)             BITFIELD_VAL(REG_DCDC3_EDGE_CON_N, n)


enum PMU_REG_T {
    PMU_REG_METAL_ID            = EXTPMU_REG(0x00),
    PMU_REG_POWER_KEY_CFG       = EXTPMU_REG(0x02),
    PMU_REG_BIAS_CFG            = EXTPMU_REG(0x03),
    PMU_REG_CHARGER_CFG         = EXTPMU_REG(0x05),
    PMU_REG_ANA_CFG             = EXTPMU_REG(0x07),
    PMU_REG_DIG_CFG             = EXTPMU_REG(0x08),
    PMU_REG_IO_CFG              = EXTPMU_REG(0x09),
    PMU_REG_MEM_CFG             = EXTPMU_REG(0x0A),
    PMU_REG_GP_CFG              = EXTPMU_REG(0x0B),
    PMU_REG_USB_CFG             = EXTPMU_REG(0x0C),
    PMU_REG_BAT2DIG_CFG         = EXTPMU_REG(0x0D),
    PMU_REG_HPPA_LDO_EN         = EXTPMU_REG(0x0E),
    PMU_REG_HPPA2CODEC_CFG      = EXTPMU_REG(0x0F),
    PMU_REG_CODEC_CFG           = EXTPMU_REG(0x10),
    PMU_REG_DCDC_DIG_EN         = EXTPMU_REG(0x15),
    PMU_REG_DCDC_ANA_CFG_16     = EXTPMU_REG(0x16),
    PMU_REG_DCDC_ANA_CFG_17     = EXTPMU_REG(0x17),
    PMU_REG_DCDC_ANA_CFG_18     = EXTPMU_REG(0x18),
    PMU_REG_DCDC_ANA_CFG_19     = EXTPMU_REG(0x19),
    PMU_REG_DCDC_HPPA_CFG_1A    = EXTPMU_REG(0x1A),
    PMU_REG_DCDC_HPPA_CFG_1B    = EXTPMU_REG(0x1B),
    PMU_REG_DCDC_HPPA_CFG_1C    = EXTPMU_REG(0x1C),
    PMU_REG_DCDC_HPPA_CFG_1D    = EXTPMU_REG(0x1D),
    PMU_REG_PWR_SEL             = EXTPMU_REG(0x21),
    PMU_REG_INT_MASK            = EXTPMU_REG(0x26),
    PMU_REG_INT_EN              = EXTPMU_REG(0x27),
    PMU_REG_RTC_LOAD_LOW        = EXTPMU_REG(0x2D),
    PMU_REG_RTC_LOAD_HIGH       = EXTPMU_REG(0x2E),
    PMU_REG_RTC_MATCH1_LOW      = EXTPMU_REG(0x31),
    PMU_REG_RTC_MATCH1_HIGH     = EXTPMU_REG(0x32),
    PMU_REG_DCDC_DIG_CFG_33     = EXTPMU_REG(0x33),
    PMU_REG_DCDC_DIG_CFG_34     = EXTPMU_REG(0x34),
    PMU_REG_DCDC_DIG_CFG_35     = EXTPMU_REG(0x35),
    PMU_REG_DCDC_DIG_CFG_36     = EXTPMU_REG(0x36),
    PMU_REG_RTC_DIV_1HZ         = EXTPMU_REG(0x37),
    PMU_REG_MIC_BIAS_E          = EXTPMU_REG(0x38),
    PMU_REG_MIC_LDO_EN          = EXTPMU_REG(0x39),
    PMU_REG_LED_CFG             = EXTPMU_REG(0x3A),
    PMU_REG_MIC_BIAS_A          = EXTPMU_REG(0x3B),
    PMU_REG_MIC_BIAS_B          = EXTPMU_REG(0x3C),
    PMU_REG_MIC_BIAS_C          = EXTPMU_REG(0x3D),
    PMU_REG_MIC_BIAS_D          = EXTPMU_REG(0x3E),
    PMU_REG_EFUSE_CTRL          = EXTPMU_REG(0x3F),
    PMU_REG_EFUSE_SEL           = EXTPMU_REG(0x40),
    PMU_REG_DCDC1_OFFSET        = EXTPMU_REG(0x41),
    PMU_REG_BUCK2ANA_CFG        = EXTPMU_REG(0x43),
    PMU_REG_SLEEP_CFG           = EXTPMU_REG(0x44),
    PMU_REG_DCDC_DIG_VOLT       = EXTPMU_REG(0x46),
    PMU_REG_DCDC_ANA_VOLT       = EXTPMU_REG(0x47),
    PMU_REG_DCDC_HPPA_VOLT      = EXTPMU_REG(0x48),
    PMU_REG_DCDC4_VOLT          = EXTPMU_REG(0x49),
    PMU_REG_DCDC_ANA_EN         = EXTPMU_REG(0x4A),
    PMU_REG_DCDC_HPPA_EN        = EXTPMU_REG(0x4B),
    PMU_REG_POWER_OFF           = EXTPMU_REG(0x4F),
    PMU_REG_INT_CLR             = EXTPMU_REG(0x51),
    PMU_REG_INT_STATUS          = EXTPMU_REG(0x52),
    PMU_REG_INT_MSKED_STATUS    = EXTPMU_REG(0x53),
    PMU_REG_RTC_VAL_LOW         = EXTPMU_REG(0x54),
    PMU_REG_RTC_VAL_HIGH        = EXTPMU_REG(0x55),
    PMU_REG_CHARGER_STATUS      = EXTPMU_REG(0x5E),

#define PMU_REG_DCDC_RAMP_EN    ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_DCDC_RAMP_EN    : PMU1800_REG_DCDC_RAMP_EN)
#define PMU_REG_PWM2_TOGGLE     ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_PWM2_TOGGLE     : PMU1800_REG_PWM2_TOGGLE)
#define PMU_REG_PWM3_TOGGLE     ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_PWM3_TOGGLE     : PMU1800_REG_PWM3_TOGGLE)
#define PMU_REG_PWM2_ST1        ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_PWM2_ST1        : PMU1800_REG_PWM2_ST1)
#define PMU_REG_PWM3_ST1        ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_PWM3_ST1        : PMU1800_REG_PWM3_ST1)
#define PMU_REG_SUBCNT_DATA     ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_SUBCNT_DATA     : PMU1800_REG_SUBCNT_DATA)
#define PMU_REG_PWM_BR_EN       ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_PWM_BR_EN       : PMU1800_REG_PWM_BR_EN)
#define PMU_REG_PWM_EN          ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_PWM_EN          : PMU1800_REG_PWM_EN)
#define PMU_REG_WDT_TIMER       ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_WDT_TIMER       : PMU1800_REG_WDT_TIMER)
#define PMU_REG_WDT_CFG         ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_WDT_CFG         : PMU1800_REG_WDT_CFG)
#define PMU_REG_EFUSE_VAL_START ((pmu_type == PMU_CHIP_TYPE_1803) ? PMU1803_REG_EFUSE_VAL_START : PMU1800_REG_EFUSE_VAL_START)

    PMU1800_REG_DCDC_RAMP_EN    = EXTPMU_REG(0x101),
    PMU1800_REG_PWM2_TOGGLE     = EXTPMU_REG(0x103),
    PMU1800_REG_PWM3_TOGGLE     = EXTPMU_REG(0x104),
    PMU1800_REG_PWM2_ST1        = EXTPMU_REG(0x105),
    PMU1800_REG_PWM3_ST1        = EXTPMU_REG(0x106),
    PMU1800_REG_SUBCNT_DATA     = EXTPMU_REG(0x107),
    PMU1800_REG_PWM_BR_EN       = EXTPMU_REG(0x108),
    PMU1800_REG_PWM_EN          = EXTPMU_REG(0x109),
    PMU1800_REG_WDT_TIMER       = EXTPMU_REG(0x10B),
    PMU1800_REG_WDT_CFG         = EXTPMU_REG(0x10C),
    PMU1800_REG_EFUSE_VAL_START = EXTPMU_REG(0x148),

    PMU1803_REG_DCDC_RAMP_EN    = EXTPMU_REG(0x60),
    PMU1803_REG_PWM2_TOGGLE     = EXTPMU_REG(0x62),
    PMU1803_REG_PWM3_TOGGLE     = EXTPMU_REG(0x63),
    PMU1803_REG_PWM2_ST1        = EXTPMU_REG(0x64),
    PMU1803_REG_PWM3_ST1        = EXTPMU_REG(0x65),
    PMU1803_REG_SUBCNT_DATA     = EXTPMU_REG(0x66),
    PMU1803_REG_PWM_BR_EN       = EXTPMU_REG(0x67),
    PMU1803_REG_PWM_EN          = EXTPMU_REG(0x68),
    PMU1803_REG_WDT_TIMER       = EXTPMU_REG(0x6A),
    PMU1803_REG_WDT_CFG         = EXTPMU_REG(0x6B),
    PMU1803_REG_PMIC_UART_CFG   = EXTPMU_REG(0x6F),
    PMU1803_REG_EFUSE_VAL_START = EXTPMU_REG(0xA0),

    PMU_REG_RESERVED_PMU        = EXTPMU_REG(0x102),
    PMU_REG_PU_LDO              = EXTPMU_REG(0x116),
    PMU_REG_LDO_VCORE_L         = EXTPMU_REG(0x11A),

    PMU_REG_DCDC1_EDGE          = EXTPMU_REG(0x11C),
    PMU_REG_DCDC2_EDGE          = EXTPMU_REG(0x11D),
    PMU_REG_DCDC3_EDGE          = EXTPMU_REG(0x11E),


    PMU_REG_MODULE_START        = PMU_REG_ANA_CFG,

    PMU_REG_ANA_60              = ANA_REG(0x60),
    PMU_REG_ANA_36A             = ANA_REG(0x36A),
    PMU_REG_ANA_380             = ANA_REG(0x380),

    PMU_REG_RF_80               = RF_REG(0x80),
    PMU_REG_RF_8E               = RF_REG(0x8E),
    PMU_REG_RF_90               = RF_REG(0x90),
    PMU_REG_RF_91               = RF_REG(0x91),
    PMU_REG_RF_92               = RF_REG(0x92),
    PMU_REG_RF_93               = RF_REG(0x93),
    PMU_REG_RF_94               = RF_REG(0x94),
    PMU_REG_RF_95               = RF_REG(0x95),
    PMU_REG_RF_96               = RF_REG(0x96),
    PMU_REG_RF_97               = RF_REG(0x97),
    PMU_REG_RF_98               = RF_REG(0x98),
    PMU_REG_RF_99               = RF_REG(0x99),
    PMU_REG_RF_9B               = RF_REG(0x9B),
    PMU_REG_RF_9D               = RF_REG(0x9D),
    PMU_REG_RF_C4               = RF_REG(0xC4),
    PMU_REG_RF_E9               = RF_REG(0xE9),
    PMU_REG_RF_FF               = RF_REG(0xFF),
    PMU_REG_RF_2A5              = RF_REG(0x2A5),
    PMU_REG_RF_2CE              = RF_REG(0x2CE),
    PMU_REG_RF_384              = RF_REG(0x384),
    PMU_REG_RF_385              = RF_REG(0x385),
    PMU_REG_RF_386              = RF_REG(0x386),
    PMU_REG_RF_388              = RF_REG(0x388),
    PMU_REG_RF_39C              = RF_REG(0x39C),
    PMU_REG_RF_39E              = RF_REG(0x39E),
    PMU_REG_RF_39F              = RF_REG(0x39F),
};

enum PMU_VCORE_REQ_T {
    PMU_VCORE_FLASH_WRITE_ENABLED   = (1 << 0),
    PMU_VCORE_FLASH_FREQ_HIGH       = (1 << 1),
    PMU_VCORE_USB_HS_ENABLED        = (1 << 2),
    PMU_VCORE_SYS_FREQ_MEDIUM_LOW   = (1 << 3),
    PMU_VCORE_SYS_FREQ_MEDIUM       = (1 << 4),
    PMU_VCORE_SYS_FREQ_HIGH         = (1 << 5),
    PMU_VCORE_RS_FREQ_HIGH          = (1 << 6),
    PMU_VCORE_RS_ADC_FREQ_HIGH      = (1 << 7),
    PMU_VCORE_IIR_FREQ_HIGH         = (1 << 8),
    PMU_VCORE_IIR_EQ_FREQ_HIGH      = (1 << 9),
    PMU_VCORE_FIR_FREQ_HIGH         = (1 << 10),
};

enum PMU_VUSB_REQ_T {
    PMU_VUSB_REQ_INIT               = (1 << 0),
    PMU_VUSB_REQ_USB                = (1 << 1),
    PMU_VUSB_REQ_LBRT               = (1 << 2),
};

union BOOT_SETTINGS_T {
    struct {
        unsigned short usb_dld_dis      :1;
        unsigned short uart_dld_en      :1;
        unsigned short trace_en         :1;
        unsigned short pll_dis          :1;
        unsigned short uart_baud_div2   :1;
        unsigned short sec_freq_div2    :1;
        unsigned short crystal_freq     :2;
        unsigned short timeout_div      :2;
        unsigned short uart_connected   :1;
        unsigned short uart_1p8v        :1;
        unsigned short sec_eng_dis      :1;
        unsigned short sec_boot_struct  :1;
        unsigned short sec_check_ver    :1;
        unsigned short reserved         :1;
    };
    unsigned short reg;
};

#ifndef NO_EXT_PMU

enum PMU_MODUAL_T {
    PMU_ANA,
    PMU_DIG,
    PMU_IO,
    PMU_MEM,
    PMU_GP,
    PMU_USB,
    PMU_BAT2DIG,
    PMU_HPPA2CODEC,
    PMU_CODEC,
    PMU_BUCK2ANA,
    PMU_USB1802,

    PMU_MODULE_QTY,
};

struct PMU_MODULE_CFG_T {
    unsigned short pu_dr;
    unsigned short pu;
    unsigned short lp_en_dr;
    unsigned short lp_en;
    unsigned short pu_dsleep;
    unsigned short vbit_dsleep_mask;
    unsigned short vbit_dsleep_shift;
    unsigned short vbit_normal_mask;
    unsigned short vbit_normal_shift;
};

#define PMU_MOD_CFG_VAL(m)              { \
    REG_PU_LDO_V##m##_DR, REG_PU_LDO_V##m##_REG, \
    LP_EN_V##m##_LDO_DR, LP_EN_V##m##_LDO_REG, \
    REG_PU_LDO_V##m##_DSLEEP, \
    LDO_V##m##_VBIT_DSLEEP_MASK, LDO_V##m##_VBIT_DSLEEP_SHIFT, \
    LDO_V##m##_VBIT_NORMAL_MASK, LDO_V##m##_VBIT_NORMAL_SHIFT }

static const struct PMU_MODULE_CFG_T pmu_module_cfg[] = {
    PMU_MOD_CFG_VAL(ANA),
    PMU_MOD_CFG_VAL(CORE),
    PMU_MOD_CFG_VAL(IO),
    PMU_MOD_CFG_VAL(MEM),
    PMU_MOD_CFG_VAL(GP),
    PMU_MOD_CFG_VAL(USB),
    PMU_MOD_CFG_VAL(BAT2VCORE),
    PMU_MOD_CFG_VAL(HPPA2VCODEC),
    PMU_MOD_CFG_VAL(CODEC),
    PMU_MOD_CFG_VAL(BUCK2ANA),
    PMU_MOD_CFG_VAL(USB1802),
};

#ifdef ANC_PROD_TEST
#define OPT_TYPE
#else
#define OPT_TYPE                        const
#endif

static OPT_TYPE bool vcodec_off =
#ifdef VCODEC_OFF
    true;
#else
    false;
#endif
static OPT_TYPE uint8_t ana_act_dcdc =
#ifdef VANA_1P2V
    PMU_DCDC_ANA_1_2V;
#elif defined(VANA_1P35V)
    PMU_DCDC_ANA_1_35V;
#elif defined(VANA_1P4V)
    PMU_DCDC_ANA_1_4V;
#elif defined(VANA_1P5V)
    PMU_DCDC_ANA_1_5V;
#elif defined(VANA_1P6V)
    PMU_DCDC_ANA_1_6V;
#else
    PMU_DCDC_ANA_1_3V;
#endif
static OPT_TYPE POSSIBLY_UNUSED uint16_t vcodec_mv = (uint16_t)(VCODEC_VOLT * 1000);
static OPT_TYPE POSSIBLY_UNUSED uint16_t vhppa_mv = (uint16_t)(VHPPA_VOLT * 1000);

int pmu_ext_get_efuse(enum PMU_EXT_EFUSE_PAGE_T page, unsigned short *efuse);

static enum PMU_CHIP_TYPE_T BOOT_BSS_LOC pmu_type;

static enum HAL_CHIP_METAL_ID_T BOOT_BSS_LOC pmu_metal_id;

static enum PMU_POWER_MODE_T BOOT_BSS_LOC pmu_power_mode = PMU_POWER_MODE_NONE;

static enum PMU_VCORE_REQ_T BOOT_BSS_LOC pmu_vcore_req;

// Move all the data/bss invovled in pmu_open() to .sram_data/.sram_bss,
// so that pmu_open() can be called at the end of BootInit(),
// for data/bss is initialized after BootInit().
static enum PMU_VUSB_REQ_T SRAM_BSS_DEF(pmu_vusb_req);

static uint16_t SRAM_BSS_DEF(dcdc_ramp_map);

static PMU_CHARGER_IRQ_HANDLER_T charger_irq_handler;

static PMU_IRQ_UNIFIED_HANDLER_T pmu_irq_hdlrs[PMU_IRQ_TYPE_QTY];

static uint8_t SRAM_BSS_DEF(vio_risereq_map);
STATIC_ASSERT(sizeof(vio_risereq_map) * 8 >= PMU_VIORISE_REQ_USER_QTY, "vio_risereq_map size too small");

static uint8_t POSSIBLY_UNUSED SRAM_DATA_DEF(vio_act_normal) = IO_VOLT_ACTIVE_NORMAL;
static uint8_t POSSIBLY_UNUSED SRAM_DATA_DEF(vio_lp) = IO_VOLT_SLEEP;
#ifdef PMU_VIO_RISE
static uint8_t SRAM_DATA_DEF(vio_act_rise) = IO_VOLT_ACTIVE_RISE;
#endif

static const uint8_t ana_lp_dcdc = PMU_DCDC_ANA_SLEEP_1_3V;

// BT might have connection drop issues if dig_lp is lower than 0.8V
static const uint8_t dig_lp_ldo = PMU_VDIG_0_65V;
static const uint8_t dig_lp_dcdc = PMU_DCDC_DIG_0_65V;
static const uint8_t dig_lp_logic = PMU_ITN_VDIG_0_65V;
#if !defined(PROGRAMMER)
static bool vmic3_repl_vmic5 = false;
#endif
static uint16_t wdt_timer;

#if defined(MCU_HIGH_PERFORMANCE_MODE)
static const uint16_t high_perf_freq_mhz =
#if defined(MTEST_ENABLED) && defined(MTEST_CLK_MHZ)
    MTEST_CLK_MHZ;
#else
    300;
#endif
static bool high_perf_on;
#endif

#ifdef PMU_FORCE_LP_MODE
static enum PMU_BIG_BANDGAP_USER_T big_bandgap_user_map;
#endif

#if defined(PMU_INIT) || (!defined(FPGA) && !defined(PROGRAMMER))
static void pmu_hppa_dcdc_to_ldo(void);
#endif

#if defined(_AUTO_TEST_)
static bool at_skip_shutdown = false;

void pmu_at_skip_shutdown(bool enable)
{
    at_skip_shutdown = enable;
}
#endif

#ifdef RTC_ENABLE
struct PMU_RTC_CTX_T {
    bool enabled;
    bool alarm_set;
    uint32_t alarm_val;
};

static struct PMU_RTC_CTX_T BOOT_BSS_LOC rtc_ctx;

static PMU_RTC_IRQ_HANDLER_T rtc_irq_handler;

static void BOOT_TEXT_SRAM_LOC pmu_rtc_save_context(void)
{
    if (pmu_rtc_enabled()) {
        rtc_ctx.enabled = true;
        if (pmu_rtc_alarm_status_set()) {
            rtc_ctx.alarm_set = true;
            rtc_ctx.alarm_val = pmu_rtc_get_alarm();
        }
    } else {
        rtc_ctx.enabled = false;
    }
}

static void pmu_rtc_restore_context(void)
{
    uint32_t rtc_val;

    if (rtc_ctx.enabled) {
        pmu_rtc_enable();
        if (rtc_ctx.alarm_set) {
            rtc_val = pmu_rtc_get();
            if (rtc_val - rtc_ctx.alarm_val <= 1 || rtc_ctx.alarm_val - rtc_val < 5) {
                rtc_ctx.alarm_val = rtc_val + 5;
            }
            pmu_rtc_set_alarm(rtc_ctx.alarm_val);
        }
    }
}
#endif

#ifdef BT_RCOSC_CAL

static void pmu_bt_xtal_startup_config(int startup_en)
{
    if (hal_get_chip_metal_id() >= HAL_CHIP_METAL_ID_4) {
        uint16_t val = 0;
        uint16_t val_rf94 = 0;

        #if 0
        //bit13:reg_bt_xtal_manaac_en | bit14:reg_bt_xtal_manaac_kick | bit15:reg_bt_xtal_manaac_kick_en
        pmu_read(PMU_REG_RF_95, &val);
        val &= 0x1fff;
        val |= (0x4 << 13);
        pmu_write(PMU_REG_RF_95, val);
        #endif

        //bit3:reg_bt_xtal_startup_en
        pmu_read(PMU_REG_RF_91, &val);
        pmu_read(PMU_REG_RF_94, &val_rf94);
        val_rf94 &= 0xFE0F;
        if (1 == startup_en){
            val |= (1 << 3);
        } else {
            val &= ~(1 << 3);
        }
        val_rf94 |= (0x06 << 4);
        pmu_write(PMU_REG_RF_91, val);
        pmu_write(PMU_REG_RF_94, val_rf94);
    }
}

void pmu_bt_rcosc_cal(bool first_boot)
{
    if (hal_get_chip_metal_id() <= HAL_CHIP_METAL_ID_3) {
        ASSERT(0, "quick startup only supports metal id 4 and above!");
    } else {
        uint16_t val = 0;
        int16_t rf39f_14_0 = 0;
        int16_t rf_96_7_0 = 0;
        uint8_t delta_v = 0;
        uint16_t bt_rcosc_cal_done = 0;
        static const uint8_t delta_margin = 0x1;
        int cycle_cnt = 0;

        //reg_bt_xtal_startup_vldo[12:10]
        pmu_read(PMU_REG_RF_9B, &val);
        val &= ~0x1C00;
        val |= (0x7 << 10);
        pmu_write(PMU_REG_RF_9B, val);

        pmu_read(PMU_REG_RF_96, &val);
        val &= ~0x7800;
        val |= ((0x3 << 11) | (0x3 << 13));
        pmu_write(PMU_REG_RF_96,val);

        //reg_bt_xtal_bg_sel_temp[10:8]
        pmu_read(PMU_REG_RF_91, &val);
        val &= ~0x0700;
        val |= (0x7 << 8);
        pmu_write(PMU_REG_RF_91, val);

        //reg_bt_xtal_startup_ibias[14:11] | reg_bt_xtal_startup_cnt_rotate[10:8] | reg_bt_xtal_startup_cnt_next[7:0]
        pmu_read(PMU_REG_RF_98, &val);
        val &= ~0x7FFF;
        val |= ((0x14) | (0x0 << 8) | (0xF << 11));
        pmu_write(PMU_REG_RF_98, val);

        //reg_bt_xtal_rcosc_cal_en
        pmu_read(PMU_REG_RF_91, &val);
        val |= (1<<2);
        pmu_write(PMU_REG_RF_91,val);

        pmu_read(PMU_REG_RF_96, &val);
        val &= ~0x00FF;
        val |= 0x80;
        pmu_write(PMU_REG_RF_96, val);

        do {
            pmu_read(PMU_REG_RF_39E, &val);
            val &= ~(1 << 4);
            pmu_write(PMU_REG_RF_39E, val);
            pmu_read(PMU_REG_RF_39E, &val);
            val |= (1 << 4);
            pmu_write(PMU_REG_RF_39E, val);

            //bit3:reg_bt_rcosc_cal_start
            pmu_read(PMU_REG_RF_39E, &val);
            val &= ~(1 << 3);
            pmu_write(PMU_REG_RF_39E, val);
            pmu_read(PMU_REG_RF_39E, &val);
            val |= (1 << 3);
            pmu_write(PMU_REG_RF_39E, val);

            hal_sys_timer_delay(US_TO_TICKS(500));

            //bit15:dig_bt_rcosc_cal_done
            pmu_read(PMU_REG_RF_39F, &val);
            //TRACE(1, "PMU_REG_RF_39F=0x%x",val);
            bt_rcosc_cal_done = (val & 0x8000);
            if (!bt_rcosc_cal_done) {
                TRACE(0, "dig bt_rcosc_cal not done!");
                break;
            }

            //dig_bt_rcosc_cal_cnt[14:0]
            rf39f_14_0 = (val & 0x7fff);

            delta_v = ABS(rf39f_14_0 - 0x100);              //compare with 0x100(256)

            //reg_bt_xtal_rcosc_fre_trim[7:0]
            pmu_read(PMU_REG_RF_96, &val);
            rf_96_7_0 = (val & 0xff);
            TRACE(2, "original rf39f_14_0=0x%x, rf_96_7_0=0x%x", rf39f_14_0, rf_96_7_0);

            if (delta_v <= delta_margin) {
                break;      //out of while loop
            }

            //avoid endless loops
            if ( ((rf_96_7_0 == 0xff) || (rf_96_7_0 == 0x0)) && (delta_v > delta_margin) ) {
                TRACE(3, "rf_96_7_0=0x%x has reached the limit, but delta_v=0x%x still > delta_margin=0x%x", rf_96_7_0, delta_v,delta_margin);
                break;
            }
            //modify 0x96[7:0]
            if (delta_v > 4) {
                delta_v /= 4;
            } else {
                delta_v = 1;
            }
            if (rf39f_14_0 > 0x100) {
                rf_96_7_0 += delta_v;
            } else {
                rf_96_7_0 -= delta_v;
            }

            //prevent overflow
            if (rf_96_7_0 > 0xff) {
                rf_96_7_0 = 0xff;
            } else if (rf_96_7_0 < 0) {
                rf_96_7_0 = 0;
            }
            TRACE(1, "current rf_96_7_0=0x%x", rf_96_7_0);

            val &= ~0x00FF;                      //clear [7:0]
            val |= rf_96_7_0;
            pmu_write(PMU_REG_RF_96, val);

            cycle_cnt++;
        } while (cycle_cnt < 50);

        TRACE(1, "RCOSC_CAL: rf_96_7_0=0x%x delta_v=%d cycle_cnt=%d", rf_96_7_0, delta_v, cycle_cnt);

        if (first_boot) {
            //bit13:reg_bt_xtal_manaac_en | bit14:reg_bt_xtal_manaac_kick | bit15:reg_bt_xtal_manaac_kick_en
            pmu_read(PMU_REG_RF_95, &val);
            val &= 0x1fff;
            pmu_write(PMU_REG_RF_95, val);

            //reg_bt_xtal_startup_inj_period[5:0] | reg_bt_xtal_startup_sel_drv[7:6]
            pmu_read(PMU_REG_RF_99, &val);
            val &= ~0x00FF;
            val |= 0xD8;
            pmu_write(PMU_REG_RF_99, val);

            //reg_bt_xtal_startup_cnt_hold[13:8] | reg_bt_xtal_startup_cnt_1st[7:2]
            pmu_read(PMU_REG_RF_97, &val);
            val &= ~0x3FFC;
            val |= ((0x8 << 2) | (0x6 << 8));
            pmu_write(PMU_REG_RF_97, val);

            pmu_read(PMU_REG_RF_FF, &val);
            val |= (1<<4);
            pmu_write(PMU_REG_RF_FF, val);
        }
    }
}

#endif  //BT_RCOSC_CAL end

unsigned short pmu_reg_val_add(unsigned short val, int delta, unsigned short max)
{
    int result = val + delta;

    if (result > max) {
        result = max;
    } else if (result < 0) {
        result = 0;
    }

    return (unsigned short)result;
}

#ifdef PMU_DCDC_CALIB
union VOLT_COMP_T {
    struct VOLT_COMP_FIELD_T {
        uint16_t dcdc1_v: 5; //bit[4:0]: 0 ~ 31
        uint16_t dcdc1_f: 1; //bit[5]  : 1: negative, 0: positive;
        uint16_t dcdc2_v: 4; //bit[9:6]: 0 ~ 15
        uint16_t dcdc2_f: 1; //bit[10] :
        uint16_t dcdc3_v: 4; //bit[14:11]: 0 ~ 15
        uint16_t dcdc3_f: 1; //bit[15]
    } f;
    uint16_t v;
};

static int8_t pmu_dcdc_dig_comp = 0;
static int8_t pmu_dcdc_ana_comp = 0;
static int8_t pmu_dcdc_hppa_comp = 0;

static int8_t pmu_dig_lp_comp = 0;
static int8_t pmu_ana_lp_comp = 0;
static int8_t pmu_hppa_lp_comp = 0;

static POSSIBLY_UNUSED void pmu_get_dcdc_calib_value(void)
{
    union VOLT_COMP_T cv;

    pmu_get_efuse(PMU_EFUSE_PAGE_RESERVED_7, &cv.v);
    if (cv.f.dcdc1_f) { //digital
        pmu_dcdc_dig_comp = -(int8_t)(cv.f.dcdc1_v);
    } else {
        pmu_dcdc_dig_comp = (int8_t)(cv.f.dcdc1_v);
    }
    if (cv.f.dcdc2_f) { //ana
        pmu_dcdc_ana_comp = -(int8_t)(cv.f.dcdc2_v);
    } else {
        pmu_dcdc_ana_comp = (int8_t)(cv.f.dcdc2_v);
    }
    if (cv.f.dcdc3_f) { //hppa
        pmu_dcdc_hppa_comp = -(int8_t)(cv.f.dcdc3_v);
    } else {
        pmu_dcdc_hppa_comp = (int8_t)(cv.f.dcdc3_v);
    }

    if (pmu_type == PMU_CHIP_TYPE_1802) {
        uint16_t low8bit, high8bit;
        pmu_ext_get_efuse(PMU_EXT_EFUSE_PAGE_RESERVED_6, &low8bit);
        pmu_ext_get_efuse(PMU_EXT_EFUSE_PAGE_RESERVED_7, &high8bit);
        cv.v = ((high8bit & 0xFF) << 8) | ((low8bit & 0xFF00) >> 8);
    } else {
        pmu_ext_get_efuse(PMU_EXT_EFUSE_PAGE_RESERVED_7, &cv.v);
    }

    if (cv.f.dcdc1_f) { //digital
        pmu_dig_lp_comp = -(int8_t)(cv.f.dcdc1_v);
    } else {
        pmu_dig_lp_comp = (int8_t)(cv.f.dcdc1_v);
    }
    if (cv.f.dcdc2_f) { //ana
        pmu_ana_lp_comp = -(int8_t)(cv.f.dcdc2_v);
    } else {
        pmu_ana_lp_comp = (int8_t)(cv.f.dcdc2_v);
    }
    if (cv.f.dcdc3_f) { //hppa
        pmu_hppa_lp_comp = -(int8_t)(cv.f.dcdc3_v);
    } else {
        pmu_hppa_lp_comp = (int8_t)(cv.f.dcdc3_v);
    }

#ifdef FORCE_BIG_BANDGAP
    pmu_dig_lp_comp  = pmu_dcdc_dig_comp;
    pmu_ana_lp_comp  = pmu_dcdc_ana_comp;
    pmu_hppa_lp_comp = pmu_dcdc_hppa_comp;
#elif defined(PMU_FORCE_LP_MODE)
    pmu_dcdc_dig_comp  = pmu_dig_lp_comp;
    pmu_dcdc_ana_comp  = pmu_ana_lp_comp;
    pmu_dcdc_hppa_comp = pmu_hppa_lp_comp;
#endif
}
#endif

POSSIBLY_UNUSED
static int BOOT_TEXT_FLASH_LOC pmu_read_efuse_block(unsigned int block)
{
    int ret;
    unsigned short val;
    uint32_t start;
    uint32_t timeout = MS_TO_TICKS(50);

    val = TSMC_EFUSE_PGENB | TSMC_EFUSE_CLK_EN;
    ret = pmu_write(PMU_REG_EFUSE_CTRL, val);
    if (ret) {
        return ret;
    }

    val = TSMC_EFUSE_STROBE_WIDTH(20) | REG_READ_STROBE_WIDTH(4) | EFUSE_SEL(block);
    ret = pmu_write(PMU_REG_EFUSE_SEL, val);
    if (ret) {
        return ret;
    }

    ret = pmu_read(PMU_REG_POWER_OFF, &val);
    if (ret) {
        return ret;
    }
    val = SET_BITFIELD(val, EFUSE_READ_TRIG, EFUSE_READ_TRIG_WORD);
    ret = pmu_write(PMU_REG_POWER_OFF, val);
    if (ret) {
        return ret;
    }

    hal_sys_timer_delay(US_TO_TICKS(500));

    start = hal_sys_timer_get();
    do {
        ret = pmu_read(PMU_REG_POWER_OFF, &val);
        if (ret) {
            return ret;
        }
        if (hal_sys_timer_get() - start > timeout) {
            return -2;
        }
    } while ((val & (EFUSE_READ_DONE | EFUSE_READ_BUSY)) != EFUSE_READ_DONE);

    val = TSMC_EFUSE_PGENB;
    ret = pmu_write(PMU_REG_EFUSE_CTRL, val);
    if (ret) {
        return ret;
    }

    return 0;
}

uint32_t BOOT_TEXT_FLASH_LOC read_hw_metal_id(void)
{
    uint16_t val;
    uint32_t metal_id;

    pmu_read(PMU_REG_METAL_ID, &val);

    if (GET_BITFIELD(val, CHIPID) == 0x183) {
        pmu_type = PMU_CHIP_TYPE_1803;
    } else if (GET_BITFIELD(val, CHIPID) == 0x182) {
        pmu_type = PMU_CHIP_TYPE_1802;
    } else if (GET_BITFIELD(val, CHIPID) == 0x187) {
        pmu_type = PMU_CHIP_TYPE_1807;
    } else {
        pmu_type = PMU_CHIP_TYPE_1800;
    }

    pmu_metal_id = GET_BITFIELD(val, REVID);

#ifdef RTC_ENABLE
    // RTC will be restored in pmu_open()
    pmu_rtc_save_context();
#endif

#ifdef __WATCHER_DOG_RESET__
    pmu_wdt_save_context();
#endif

#if !defined(FPGA) && !defined(PROGRAMMER)
    pmu_charger_save_context();
#endif

    // Reset PMU (to recover from a possible insane state, e.g., ESD reset)
    pmu_write(PMU_REG_METAL_ID, 0xCAFE);
    pmu_write(PMU_REG_METAL_ID, 0x5FEE);
    hal_sys_timer_delay(US_TO_TICKS(500));

#ifdef __WATCHER_DOG_RESET__
    pmu_wdt_restore_context();
#else
    pmu_wdt_stop();
#endif

    if (pmu_type != PMU_CHIP_TYPE_1803) {
        // Set VIO to 1.8V
        pmu_read(PMU_REG_IO_CFG, &val);
        val = (val & ~(LDO_VIO_VBIT_NORMAL_MASK | LDO_VIO_VBIT_DSLEEP_MASK)) |
            LDO_VIO_VBIT_NORMAL(IO_VOLT_ACTIVE_NORMAL) | LDO_VIO_VBIT_DSLEEP(IO_VOLT_SLEEP);
        pmu_write(PMU_REG_IO_CFG, val);
    }

    // Init internal PMU
    pmu_itn_init();

    //To Do subsequent need to be compatible with 1802 ext pmu efuse
    if ((pmu_type != PMU_CHIP_TYPE_1802) && (pmu_type != PMU_CHIP_TYPE_1807)) {
        hal_cmu_boot_dcdc_clock_enable();

        int ret;
#if 1
        ret = pmu_read_efuse_block(2);
        if (ret) {
            SAFE_PROGRAM_STOP();
        }
#else
    #ifndef EFUSE_BLOCK_NUM
    #define EFUSE_BLOCK_NUM                 6
    #endif

        int i;
        for (i = 0; i < EFUSE_BLOCK_NUM; i++) {
            ret = pmu_read_efuse_block(i);
            if (ret) {
                SAFE_PROGRAM_STOP();
            }
        }
#endif
        hal_cmu_boot_dcdc_clock_disable();
    }

    pmu_rf_ana_init();

    // Enable SWD debug mode
    pmu_read(PMU_REG_SLEEP_CFG, &val);
    val = SET_BITFIELD(val, PMIC_TMODE_1300, 2);
    pmu_write(PMU_REG_SLEEP_CFG, val);

    metal_id = hal_cmu_get_aon_revision_id();

    return metal_id;
}

#endif // !NO_EXT_PMU

void BOOT_TEXT_FLASH_LOC bbpll_freq_pll_config(uint32_t freq)
{
#if 0
    int ret;
    uint64_t PLL_cfg_val;
    uint16_t v[3];
    uint16_t val;
    uint32_t crystal = hal_cmu_get_crystal_freq();

    PLL_cfg_val = ((uint64_t)(1 << 28) * (freq / 2) + crystal / 2) / crystal;
    v[0] = PLL_cfg_val & 0xFFFF;
    v[1] = (PLL_cfg_val >> 16) & 0xFFFF;
    v[2] = (PLL_cfg_val >> 32) & 0xFFFF;

    ret = pmu_write(PMU_REG_RF_BB, v[0]);
    if (ret) {
        return;
    }

    ret = pmu_write(PMU_REG_RF_BC, v[1]);
    if (ret) {
        return;
    }

    ret = pmu_read(PMU_REG_RF_BD, &val);
    if (ret) {
        return;
    }
    val = SET_BITFIELD(val, REG_BBPLL_FREQ_34_32, v[2]) | REG_BBPLL_FREQ_EN;
    ret = pmu_write(PMU_REG_RF_BD, val);
    if (ret) {
        return;
    }
#endif
    // Delay at least for 7us
    hal_sys_timer_delay(US_TO_TICKS(100));
}

void BOOT_TEXT_FLASH_LOC pmu_rf_ana_init(void)
{
    int ret;
    uint16_t val;
    union BOOT_SETTINGS_T boot;

    ret = pmu_get_efuse(PMU_EFUSE_PAGE_BOOT, &boot.reg);
    if (ret) {
        boot.reg = 0;
    }
    hal_cmu_set_crystal_freq_index(boot.crystal_freq);

#if !defined(FPGA) && !defined(PROGRAMMER) && !defined(MCU_HIGH_PERFORMANCE_MODE)
    if (hal_cmu_get_crystal_freq() != hal_cmu_get_default_crystal_freq()) {
        // Update bbpll freq after resetting RF and getting crystal freq
        bbpll_freq_pll_config(384000000);
    }
#endif

#if defined(PMU_FULL_INIT) || (!defined(FPGA) && !defined(PROGRAMMER))
    if (hal_cmu_get_aon_revision_id() >= HAL_CHIP_METAL_ID_1) {
        // Reset RF
        pmu_write(PMU_REG_RF_80, 0xCAFE);
        pmu_write(PMU_REG_RF_80, 0x5FEE);
        hal_sys_timer_delay(US_TO_TICKS(500));
    }

    // Reset ANA
    pmu_write(PMU_REG_ANA_60, 0xCAFE);
    pmu_write(PMU_REG_ANA_60, 0x5FEE);
    hal_sys_timer_delay(US_TO_TICKS(500));
#endif

    // Inc clock current
    pmu_write(PMU_REG_RF_99, 0xE59A);
    pmu_write(PMU_REG_RF_94, 0x4467);
    // X2/X4 settings
    pmu_write(PMU_REG_RF_39C, 0x009B);
    // PMU_REG_RF_99 has been set above
    pmu_write(PMU_REG_RF_9B, 0x1E58);
    pmu_write(PMU_REG_RF_9D, 0x2000);

    // Power up bbpll and audpll clk buf
    // Clk buf bits:
    // 0-codec 1-debouncing 2-audpll 3-rx_sar_pdt 4-usbpll 5-rf_itf 6-usb_itf 7-dpa_itf
    pmu_read(PMU_REG_RF_90, &val);
    val = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 5);
#if defined(USB_HIGH_SPEED) || defined(USB_USE_USBPLL)
    val |= (1 << 4);
#endif
    pmu_write(PMU_REG_RF_90, val);

    pmu_write(PMU_REG_RF_91, 0x0B40);
    pmu_write(PMU_REG_RF_92, 0x0004);
    pmu_write(PMU_REG_RF_93, 0x0F58);
    pmu_write(PMU_REG_RF_95, 0x1300);
    pmu_write(PMU_REG_RF_97, 0x0840);

    pmu_write(PMU_REG_RF_2A5, 0x0101);

    //bit5 : reg_bt_tstbuf_pu
    pmu_read(PMU_REG_RF_2CE, &val);
    val &= ~(1<<5);
    pmu_write(PMU_REG_RF_2CE, val);

    if (hal_cmu_get_aon_revision_id() <= HAL_CHIP_METAL_ID_2) {
        pmu_read(PMU_REG_RF_8E, &val);
        val |= (1 << 0) | (1 << 1);
        pmu_write(PMU_REG_RF_8E, val);

        pmu_read(PMU_REG_RF_388, &val);
        val &= ~(1 << 2);
        val |= (1 << 3);
        pmu_write(PMU_REG_RF_388, val);
    }

    // Init audpll_div_delay_cap to 0
    pmu_write(PMU_REG_ANA_36A, 0x90);
    // Disable audpll_clkgen_en_dig_dr and audpll_clkgen_en_ad_dr
    pmu_write(PMU_REG_ANA_380, 0);
}

int BOOT_TEXT_SRAM_LOC pmu_get_security_value(union SECURITY_VALUE_T *val)
{
    int ret;

    val->reg = 0;

    ret = pmu_get_efuse(PMU_EFUSE_PAGE_SECURITY, &val->reg);
    if (ret) {
        // Error
        return ret;
    }

    return 0;
}

enum HAL_FLASH_ID_T BOOT_TEXT_SRAM_LOC pmu_get_boot_flash_id(void)
{
    union SECURITY_VALUE_T security;

    pmu_get_security_value(&security);

    return (security.root.flash_id) ? HAL_FLASH_ID_1 : HAL_FLASH_ID_0;
}

#if defined(USE_CYBERON)
static uint32_t BOOT_BSS_LOC cyb_hack_efuse_check = 0;
void cyb_efuse_check_enable(int enabled)
{
    cyb_hack_efuse_check = enabled;
}

int cyb_efuse_check_status(void)
{
    return cyb_hack_efuse_check;
}

#if 1
int BOOT_TEXT_SRAM_LOC Cyb_pmu_get_efuse(enum PMU_EFUSE_PAGE_T page, unsigned short *efuse)
{
//    PMU_DEBUG_TRACE(2,"page %x, efuse %x", page, efuse);
      if(page==0XE){
          *efuse=0x1B9C;
      }
      if(page==0xF){
          *efuse = 0x098B;
      }

      return 0;
}
#endif
#endif

#ifndef NO_EXT_PMU

int BOOT_TEXT_SRAM_LOC pmu_ext_get_efuse(enum PMU_EXT_EFUSE_PAGE_T page, unsigned short *efuse)
{
    int ret;

    ret = pmu_read(PMU_REG_EFUSE_VAL_START + page, efuse);

    return ret;
}

static void pmu_sys_ctrl(bool shutdown)
{
    uint16_t val;
    uint32_t lock = int_lock();

    PMU_INFO_TRACE_IMM(0, "Start pmu %s", shutdown ? "shutdown" : "reboot");

#if defined(PMU_INIT) || (!defined(FPGA) && !defined(PROGRAMMER))
#if defined(MCU_HIGH_PERFORMANCE_MODE)
    // Default vcore might not be high enough to support high performance mode
    pmu_high_performance_mode_enable(false);
    hal_cmu_sys_set_freq(HAL_CMU_FREQ_26M);
#endif
    pmu_hppa_dcdc_to_ldo();
    pmu_mode_change(PMU_POWER_MODE_LDO);
    hal_sys_timer_delay(MS_TO_TICKS(1));
#endif

#ifdef RTC_ENABLE
    pmu_rtc_save_context();
#endif

    // Reset PMU
    pmu_write(PMU_REG_METAL_ID, 0xCAFE);
    pmu_write(PMU_REG_METAL_ID, 0x5FEE);
    hal_sys_timer_delay(4);

    if (pmu_type != PMU_CHIP_TYPE_1803) {
        // Set VIO to 1.8V
        pmu_read(PMU_REG_IO_CFG, &val);
        val = (val & ~(LDO_VIO_VBIT_NORMAL_MASK | LDO_VIO_VBIT_DSLEEP_MASK)) |
            LDO_VIO_VBIT_NORMAL(IO_VOLT_ACTIVE_NORMAL) | LDO_VIO_VBIT_DSLEEP(IO_VOLT_SLEEP);
        pmu_write(PMU_REG_IO_CFG, val);
    }

#ifdef RTC_ENABLE
    pmu_rtc_restore_context();
#endif

    if (shutdown) {
#if defined(_AUTO_TEST_)
        if (at_skip_shutdown) {
            hal_cmu_sys_reboot();
            return;
        }
#endif

#if defined(PMU_INIT) || (!defined(FPGA) && !defined(PROGRAMMER))
        pmu_wdt_config(3*1000,3*1000);
        pmu_wdt_start();
        pmu_charger_shutdown_config();
#endif

        // Power off
        pmu_read(PMU_REG_POWER_OFF,&val);
        val |= SOFT_POWER_OFF;
        for (int i = 0; i < 100; i++) {
            pmu_write(PMU_REG_POWER_OFF,val);
            hal_sys_timer_delay(MS_TO_TICKS(5));
        }

        hal_sys_timer_delay(MS_TO_TICKS(50));

        //can't reach here
        PMU_INFO_TRACE_IMM(0, "\nError: pmu shutdown failed!\n");
        hal_sys_timer_delay(MS_TO_TICKS(5));
    } else {
#if defined(PMU_FULL_INIT) || (!defined(FPGA) && !defined(PROGRAMMER))
        // CAUTION:
        // 1) Never reset RF because system or flash might be using X2/X4, which are off by default
        // 2) Never reset RF/ANA because system or flash might be using PLL, and the reset might cause clock glitch
        // TODO:
        // Restore BBPLL settings in RF
#endif
    }

    hal_cmu_sys_reboot();

    int_unlock(lock);
}

void pmu_shutdown(void)
{
    pmu_sys_ctrl(true);
}

void pmu_reboot(void)
{
    pmu_sys_ctrl(false);
}

static inline uint16_t pmu_get_module_addr(enum PMU_MODUAL_T module)
{
    if (module == PMU_HPPA2CODEC || module == PMU_CODEC) {
        return PMU_REG_MODULE_START + module + 1;
    } else if (module == PMU_BUCK2ANA) {
        return PMU_REG_BUCK2ANA_CFG;
    } else {
        return PMU_REG_MODULE_START + module;
    }
}

void pmu_module_config(enum PMU_MODUAL_T module,unsigned short is_manual,unsigned short ldo_on,unsigned short lp_mode,unsigned short dpmode)
{
    unsigned short val;
    unsigned short module_address;

    module_address = pmu_get_module_addr(module);

    if ((module == PMU_USB) && ((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807))) {
        module = PMU_USB1802;
    }
    const struct PMU_MODULE_CFG_T *module_cfg_p = &pmu_module_cfg[module];

    if (module == PMU_ANA) {
        pmu_read(PMU_REG_BIAS_CFG, &val);
        if(is_manual) {
            val |= module_cfg_p->pu_dr;
        } else {
            val &= ~module_cfg_p->pu_dr;
        }

        if(ldo_on) {
            val |= module_cfg_p->pu;
        } else {
            val &= ~module_cfg_p->pu;
        }
        pmu_write(PMU_REG_BIAS_CFG, val);
    } else if (module == PMU_DIG) {
        if((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) {
            pmu_read(PMU_REG_PU_LDO, &val);
            if(is_manual) {
                val |= REG_PU_LDO_DIG_M_DR;
            } else {
                val &= ~REG_PU_LDO_DIG_M_DR;
            }

            if(ldo_on) {
                val |= REG_PU_LDO_DIG_M_REG;
            } else {
                val &= ~REG_PU_LDO_DIG_M_REG;
            }

            if (dpmode) {
                val |= REG_PU_LDO_DIG_M_DSLEEP;
            } else {
                val &= ~REG_PU_LDO_DIG_M_DSLEEP;
            }
            pmu_write(PMU_REG_PU_LDO, val);
        } else {
            pmu_read(PMU_REG_DCDC_DIG_EN, &val);
            if(is_manual) {
                val |= module_cfg_p->pu_dr;
            } else {
                val &= ~module_cfg_p->pu_dr;
            }

            if(ldo_on) {
                val |= module_cfg_p->pu;
            } else {
                val &= ~module_cfg_p->pu;
            }
            pmu_write(PMU_REG_DCDC_DIG_EN, val);
        }
    } else if (module == PMU_HPPA2CODEC) {
        pmu_read(PMU_REG_SLEEP_CFG, &val);
        if(dpmode) {
            val |= module_cfg_p->pu_dsleep;
        } else {
            val &= ~module_cfg_p->pu_dsleep;
        }
        pmu_write(PMU_REG_SLEEP_CFG, val);
    } else if (module == PMU_CODEC) {
        pmu_read(PMU_REG_HPPA2CODEC_CFG, &val);
        if(dpmode) {
            val |= module_cfg_p->pu_dsleep;
        } else {
            val &= ~module_cfg_p->pu_dsleep;
        }
        pmu_write(PMU_REG_HPPA2CODEC_CFG, val);
    }

    pmu_read(module_address, &val);

    if (module != PMU_ANA && module != PMU_DIG) {
        if(is_manual) {
            val |= module_cfg_p->pu_dr;
        } else {
            val &= ~module_cfg_p->pu_dr;
        }

        if(ldo_on) {
            val |= module_cfg_p->pu;
        } else {
            val &= ~module_cfg_p->pu;
        }
    }

#ifdef PMU_FORCE_LP_MODE
    if (pmu_power_mode != PMU_POWER_MODE_NONE) {
        val |= module_cfg_p->lp_en_dr;
        if(ldo_on) {
            val |= module_cfg_p->lp_en;
        } else {
            val &= ~module_cfg_p->lp_en;
        }
    }
#else
    if (lp_mode) {
        val &= ~module_cfg_p->lp_en_dr;
    } else {
        val = (val & ~module_cfg_p->lp_en) | module_cfg_p->lp_en_dr;
    }
#endif

    if((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) {
        if (module != PMU_DIG && module != PMU_HPPA2CODEC && module != PMU_CODEC) {
            if (dpmode) {
                val |= module_cfg_p->pu_dsleep;
            } else {
                val &= ~module_cfg_p->pu_dsleep;
            }
        }
    } else {
        if (module != PMU_HPPA2CODEC && module != PMU_CODEC) {
            if (dpmode) {
                val |= module_cfg_p->pu_dsleep;
            } else {
                val &= ~module_cfg_p->pu_dsleep;
            }
        }
    }

    pmu_write(module_address, val);
}

#ifdef PMU_FORCE_LP_MODE
void pmu_module_force_lp_config(void)
{
    uint16_t val, tmp;
    uint16_t module_address;
    const struct PMU_MODULE_CFG_T *module_cfg_p;
    enum PMU_MODUAL_T module;
    bool ldo_on;

    for (module = 0; module < PMU_MODULE_QTY; module++) {
        module_cfg_p = &pmu_module_cfg[module];
        module_address = pmu_get_module_addr(module);

        pmu_read(module_address, &val);
        if (module == PMU_ANA) {
            pmu_read(PMU_REG_BIAS_CFG, &tmp);
            ldo_on = !!(tmp & module_cfg_p->pu);
        } else if (module == PMU_DIG) {
            pmu_read(PMU_REG_DCDC_DIG_EN, &tmp);
            ldo_on = !!(tmp & module_cfg_p->pu);
        } else {
            ldo_on = !!(val & module_cfg_p->pu);
        }
        val |= module_cfg_p->lp_en_dr;
        if (ldo_on) {
            val |= module_cfg_p->lp_en;
        } else {
            val &= ~module_cfg_p->lp_en;
        }
        pmu_write(module_address, val);
    }
}
#endif

void pmu_module_set_volt(unsigned char module, unsigned short sleep_v,unsigned short normal_v)
{
    unsigned short val;
    unsigned short module_address;

    module_address = pmu_get_module_addr(module);

    if ((module == PMU_USB) && ((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807))) {
        module = PMU_USB1802;
    }
    const struct PMU_MODULE_CFG_T *module_cfg_p = &pmu_module_cfg[module];

    pmu_read(module_address, &val);
    val &= ~module_cfg_p->vbit_normal_mask;
    val |= (normal_v << module_cfg_p->vbit_normal_shift) & module_cfg_p->vbit_normal_mask;
    val &= ~module_cfg_p->vbit_dsleep_mask;
    val |= (sleep_v << module_cfg_p->vbit_dsleep_shift) & module_cfg_p->vbit_dsleep_mask;
    pmu_write(module_address, val);
}

int pmu_module_get_volt(unsigned char module, unsigned short *sleep_vp,unsigned short *normal_vp)
{
    unsigned short val;
    unsigned short module_address;

    module_address = pmu_get_module_addr(module);

    if ((module == PMU_USB) && ((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807))) {
        module = PMU_USB1802;
    }
    const struct PMU_MODULE_CFG_T *module_cfg_p = &pmu_module_cfg[module];

    pmu_read(module_address, &val);
    if (normal_vp) {
        *normal_vp = (val & module_cfg_p->vbit_normal_mask) >> module_cfg_p->vbit_normal_shift;
    }
    if (sleep_vp) {
        *sleep_vp = (val & module_cfg_p->vbit_dsleep_mask) >> module_cfg_p->vbit_dsleep_shift;
    }

    return 0;
}

static void pmu_module_ramp_volt(unsigned char module, unsigned short sleep_v, unsigned short normal_v)
{
    uint16_t old_normal_v;
    uint16_t old_sleep_v;

    pmu_module_get_volt(module, &old_sleep_v, &old_normal_v);

    if (old_normal_v < normal_v) {
        while (old_normal_v++ < normal_v) {
            pmu_module_set_volt(module, sleep_v, old_normal_v);
        }
    } else if (old_normal_v != normal_v || old_sleep_v != sleep_v) {
        pmu_module_set_volt(module, sleep_v, normal_v);
    }
}

static void pmu_dcdc_ana_get_volt(unsigned short *normal_vp, unsigned short *dsleep_vp)
{
    unsigned short val;

    pmu_read(PMU_REG_DCDC_ANA_VOLT, &val);
    if (normal_vp) {
        *normal_vp = GET_BITFIELD(val, REG_DCDC2_VBIT_NORMAL);
    }
    if (dsleep_vp) {
        *dsleep_vp = GET_BITFIELD(val, REG_DCDC2_VBIT_DSLEEP);
    }

#ifdef PMU_DCDC_CALIB
    if (normal_vp) {
        *normal_vp = pmu_reg_val_add(*normal_vp, -pmu_dcdc_ana_comp, MAX_DCDC2_VBIT_VAL);
    }
    if (dsleep_vp) {
        *dsleep_vp = pmu_reg_val_add(*dsleep_vp, -pmu_ana_lp_comp, MAX_DCDC2_VBIT_VAL);
    }
#endif
}

static void pmu_dcdc_ana_set_volt(unsigned short normal_v,unsigned short dsleep_v)
{
    unsigned short val;

#ifdef PMU_DCDC_CALIB
    normal_v = pmu_reg_val_add(normal_v, pmu_dcdc_ana_comp, MAX_DCDC2_VBIT_VAL);
    dsleep_v = pmu_reg_val_add(dsleep_v, pmu_ana_lp_comp, MAX_DCDC2_VBIT_VAL);
#endif

    pmu_read(PMU_REG_DCDC_ANA_VOLT, &val);
    val &= ~REG_DCDC2_VBIT_DSLEEP_MASK;
    val &= ~REG_DCDC2_VBIT_NORMAL_MASK;
    val |= REG_DCDC2_VBIT_DSLEEP(dsleep_v);
    val |= REG_DCDC2_VBIT_NORMAL(normal_v);
    pmu_write(PMU_REG_DCDC_ANA_VOLT, val);

    if (normal_v > dsleep_v) {
        dcdc_ramp_map |= REG_DCDC2_RAMP_EN;
    } else {
        dcdc_ramp_map &= ~REG_DCDC2_RAMP_EN;
    }
}

static void pmu_ana_set_volt(int mode_change, enum PMU_POWER_MODE_T mode)
{
    uint16_t old_act_dcdc;
    uint16_t old_lp_dcdc;
    uint16_t new_act_dcdc;

    if (mode == PMU_POWER_MODE_ANA_DCDC || mode == PMU_POWER_MODE_DIG_DCDC) {
        pmu_dcdc_ana_get_volt(&old_act_dcdc, &old_lp_dcdc);
        new_act_dcdc = ana_act_dcdc;
        if (old_act_dcdc < new_act_dcdc) {
            while (old_act_dcdc++ < new_act_dcdc) {
                pmu_dcdc_ana_set_volt(old_act_dcdc, ana_lp_dcdc);
            }
            hal_sys_timer_delay_us(PMU_VANA_STABLE_TIME_US);
        } else if (old_act_dcdc != new_act_dcdc || old_lp_dcdc != ana_lp_dcdc) {
            pmu_dcdc_ana_set_volt(new_act_dcdc, ana_lp_dcdc);
        }
    }
}

static void pmu_dcdc_dig_get_volt(unsigned short *normal_vp, unsigned short *dsleep_vp)
{
    unsigned short val;

    pmu_read(PMU_REG_DCDC_DIG_VOLT, &val);
    if (normal_vp) {
        *normal_vp = GET_BITFIELD(val, REG_DCDC1_VBIT_NORMAL);
    }
    if (dsleep_vp) {
        *dsleep_vp = GET_BITFIELD(val, REG_DCDC1_VBIT_DSLEEP);
    }

#ifdef PMU_DCDC_CALIB
    if (normal_vp) {
        *normal_vp = pmu_reg_val_add(*normal_vp, -pmu_dcdc_dig_comp, MAX_DCDC1_VBIT_VAL);
    }
    if (dsleep_vp) {
        *dsleep_vp = pmu_reg_val_add(*dsleep_vp, -pmu_dig_lp_comp, MAX_DCDC1_VBIT_VAL);
    }
#endif
}

static void pmu_dcdc_dig_set_volt(unsigned short normal_v,unsigned short dsleep_v)
{
    unsigned short val;

#ifdef PMU_DCDC_CALIB
    normal_v = pmu_reg_val_add(normal_v, pmu_dcdc_dig_comp, MAX_DCDC1_VBIT_VAL);
    dsleep_v = pmu_reg_val_add(dsleep_v, pmu_dig_lp_comp, MAX_DCDC1_VBIT_VAL);
#endif

    pmu_read(PMU_REG_DCDC_DIG_VOLT, &val);
    val &= ~REG_DCDC1_VBIT_DSLEEP_MASK;
    val &= ~REG_DCDC1_VBIT_NORMAL_MASK;
    val |= REG_DCDC1_VBIT_DSLEEP(dsleep_v);
    val |= REG_DCDC1_VBIT_NORMAL(normal_v);
    pmu_write(PMU_REG_DCDC_DIG_VOLT, val);

    if (normal_v > dsleep_v) {
        dcdc_ramp_map |= REG_DCDC1_RAMP_EN;
    } else {
        dcdc_ramp_map &= ~REG_DCDC1_RAMP_EN;
    }
}

static void BOOT_TEXT_SRAM_LOC pmu_dig_get_target_volt(uint16_t *ldo, uint16_t *dcdc, uint16_t *logic)
{
    uint16_t ldo_volt;
    uint16_t dcdc_volt;
    uint16_t logic_volt;

    if (0) {
#if defined(MCU_HIGH_PERFORMANCE_MODE)
    } else if (pmu_vcore_req & (PMU_VCORE_SYS_FREQ_HIGH)) {
        if (high_perf_freq_mhz <= 260) {
            ldo_volt = PMU_VDIG_1_05V;
            dcdc_volt = PMU_DCDC_DIG_1_05V;
            logic_volt = PMU_ITN_VDIG_0_85V;
        } else if (high_perf_freq_mhz <= 300) {
            ldo_volt = PMU_VDIG_1_1V;
            dcdc_volt = PMU_DCDC_DIG_1_1V;
            logic_volt = PMU_ITN_VDIG_0_95V;
        } else {
            ldo_volt = PMU_VDIG_1_2V;
            dcdc_volt = PMU_DCDC_DIG_1_1V;
            logic_volt = PMU_ITN_VDIG_0_9V;
        }
#endif
    } else if (pmu_vcore_req & (PMU_VCORE_USB_HS_ENABLED | PMU_VCORE_RS_FREQ_HIGH | PMU_VCORE_SYS_FREQ_MEDIUM)) {
        ldo_volt = PMU_VDIG_0_9V;
        dcdc_volt = PMU_DCDC_DIG_0_9V;
        logic_volt = PMU_ITN_VDIG_0_85V;

#if defined(SKYWALKER_IMPLEMENTATION)
        uint32_t mhz = analog_aud_pll_get_freq_mhz();

        if (mhz == 122) {
            dcdc_volt = PMU_DCDC_DIG_0_8V;
            logic_volt = PMU_ITN_VDIG_0_8V;
        } else if ((mhz == 147) || (mhz == 172)) {
            dcdc_volt = PMU_DCDC_DIG_0_85V;
            logic_volt = PMU_ITN_VDIG_0_85V;
        } else {
            dcdc_volt = PMU_DCDC_DIG_0_9V;
            logic_volt = PMU_ITN_VDIG_0_85V;
        }
#endif // #if defined(SKYWALKER_IMPLEMENTATION)
    } else if (pmu_vcore_req & (PMU_VCORE_FLASH_FREQ_HIGH | PMU_VCORE_FLASH_WRITE_ENABLED)) {
        ldo_volt = PMU_VDIG_0_8V;
        dcdc_volt = PMU_DCDC_DIG_0_8V;
        logic_volt = PMU_ITN_VDIG_0_75V;
    } else {
        // Common cases
        ldo_volt = PMU_VDIG_0_8V;
        dcdc_volt = PMU_DCDC_DIG_0_8V;
        if (pmu_vcore_req & (PMU_VCORE_SYS_FREQ_MEDIUM_LOW |
                PMU_VCORE_FIR_FREQ_HIGH | PMU_VCORE_IIR_FREQ_HIGH | PMU_VCORE_IIR_EQ_FREQ_HIGH |
                PMU_VCORE_RS_FREQ_HIGH | PMU_VCORE_RS_ADC_FREQ_HIGH)) {
            logic_volt = PMU_ITN_VDIG_0_8V;
        } else {
            logic_volt = PMU_ITN_VDIG_0_75V; //PMU_ITN_VDIG_0_65V;
        }
    }

#if (defined(PROGRAMMER) || defined(__BES_OTA_MODE__)) && !defined(PMU_FULL_INIT)
    // Try to keep the same vcore voltage as ROM (hardware default)
    if (ldo_volt < PMU_VDIG_0_9V) {
        ldo_volt = PMU_VDIG_0_9V;
    }
    if (dcdc_volt < PMU_DCDC_DIG_0_9V) {
        dcdc_volt = PMU_DCDC_DIG_0_9V;
    }
    if (logic_volt < PMU_ITN_VDIG_0_7V) {
        logic_volt = PMU_ITN_VDIG_0_7V;
    }
#endif

#if defined(MTEST_ENABLED) && defined(MTEST_VOLT)
#ifdef DIG_DCDC_MODE
    dcdc_volt = MTEST_VOLT;
#else
    ldo_volt  = MTEST_VOLT;
#endif
#ifdef MTEST_LOGIC_VCORE
    logic_volt = MTEST_LOGIC_VCORE;
#endif
#endif

    if (ldo) {
        *ldo = ldo_volt;
    }
    if (dcdc) {
        *dcdc = dcdc_volt;
    }
    if (logic) {
        *logic = logic_volt;
    }
}

static void pmu_dig_set_volt(int mode_change, enum PMU_POWER_MODE_T mode)
{
    uint32_t lock;
    uint16_t dcdc_volt, old_act_dcdc, old_lp_dcdc;
    uint16_t ldo_volt, old_act_ldo, old_lp_ldo;
    uint16_t ldo_real, dcdc_real;
    uint16_t logic_volt;
    bool volt_inc = false;

    lock = int_lock();

    if (mode == PMU_POWER_MODE_NONE) {
        mode = pmu_power_mode;
    }

    pmu_dig_get_target_volt(&ldo_volt, &dcdc_volt, &logic_volt);

    pmu_module_get_volt(PMU_DIG, &old_lp_ldo, &old_act_ldo);
    pmu_dcdc_dig_get_volt(&old_act_dcdc, &old_lp_dcdc);

    if (mode_change) {
        // Both DCDC and LDO might have been enabled
        ldo_real = (old_act_ldo - PMU_VDIG_0_4V) / 0x2;
        dcdc_real = (old_act_dcdc - PMU_DCDC_DIG_0_4V) / 0x10;
        if (dcdc_real < ldo_real) {
            dcdc_real = ldo_real;
        }
        ldo_real = PMU_VDIG_0_4V + dcdc_real * 0x2;
        dcdc_real = PMU_DCDC_DIG_0_4V + dcdc_real * 0x10;

        // Set the voltage to current real voltage
        if (old_act_ldo < ldo_real) {
            old_act_ldo = ldo_real;
            old_lp_ldo = dig_lp_ldo;
            pmu_module_set_volt(PMU_DIG, old_lp_ldo, old_act_ldo);
        }
        if (old_act_dcdc < dcdc_real) {
            old_act_dcdc = dcdc_real;
            old_lp_dcdc = dig_lp_dcdc;
            pmu_dcdc_dig_set_volt(old_act_dcdc, old_lp_dcdc);
        }
    }

    if (mode_change || mode == PMU_POWER_MODE_DIG_DCDC) {
        if (old_act_dcdc < dcdc_volt) {
            volt_inc = true;
            while (old_act_dcdc++ < dcdc_volt) {
                pmu_dcdc_dig_set_volt(old_act_dcdc, dig_lp_dcdc);
            }
        } else if (old_act_dcdc != dcdc_volt || old_lp_dcdc != dig_lp_dcdc) {
            pmu_dcdc_dig_set_volt(dcdc_volt, dig_lp_dcdc);
        }
    }

    if (mode_change || mode != PMU_POWER_MODE_DIG_DCDC) {
        if (old_act_ldo < ldo_volt) {
            volt_inc = true;
        }
        pmu_module_ramp_volt(PMU_DIG, dig_lp_ldo, ldo_volt);
    }

    pmu_itn_logic_dig_set_volt(logic_volt, dig_lp_logic);

    if (mode_change == 0) {
        // Update the voltage of the other mode
        if (mode == PMU_POWER_MODE_DIG_DCDC) {
            pmu_module_set_volt(PMU_DIG, dig_lp_ldo, ldo_volt);
        } else {
            pmu_dcdc_dig_set_volt(dcdc_volt, dig_lp_dcdc);
        }
    }

    if (volt_inc) {
        hal_sys_timer_delay_us(PMU_VCORE_STABLE_TIME_US);
    }

    int_unlock(lock);
}

static void pmu_ldo_mode_en(void)
{
    unsigned short val;

    // Enable vana ldo
    pmu_module_config(PMU_ANA,PMU_AUTO_MODE,PMU_LDO_ON,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_ON);
    // Enable vcore ldo
    pmu_module_config(PMU_DIG,PMU_AUTO_MODE,PMU_LDO_ON,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_ON);

    pmu_ana_set_volt(1, PMU_POWER_MODE_LDO);
    pmu_dig_set_volt(1, PMU_POWER_MODE_LDO);

    hal_sys_timer_delay_us(PMU_LDO_PU_STABLE_TIME_US);

    // Disable vcore dcdc
    pmu_read(PMU_REG_DCDC_DIG_EN, &val);
    val = (val & ~REG_PU_DCDC1) | REG_PU_DCDC1_DR;
    pmu_write(PMU_REG_DCDC_DIG_EN, val);
    // Disable vana dcdc
    pmu_read(PMU_REG_DCDC_ANA_EN, &val);
    val = (val & ~REG_PU_DCDC2) | REG_PU_DCDC2_DR;
    pmu_write(PMU_REG_DCDC_ANA_EN, val);
}

static void pmu_dcdc_ana_mode_en(void)
{
    unsigned short val;

    if (pmu_power_mode == PMU_POWER_MODE_DIG_DCDC) {
        // Enable vcore ldo
        pmu_module_config(PMU_DIG,PMU_AUTO_MODE,PMU_LDO_ON,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_ON);

        pmu_ana_set_volt(1, PMU_POWER_MODE_ANA_DCDC);
        pmu_dig_set_volt(1, PMU_POWER_MODE_ANA_DCDC);

        hal_sys_timer_delay_us(PMU_LDO_PU_STABLE_TIME_US);

        // Disable vcore dcdc
        pmu_read(PMU_REG_DCDC_DIG_EN, &val);
        val = (val & ~REG_PU_DCDC1) | REG_PU_DCDC1_DR;
        pmu_write(PMU_REG_DCDC_DIG_EN, val);
    } else {
        pmu_read(PMU_REG_DCDC_ANA_CFG_19, &val);
        val |= DCDC2_REG_BYPASS;
        pmu_write(PMU_REG_DCDC_ANA_CFG_19, val);

        // Enable vana dcdc
        pmu_read(PMU_REG_DCDC_ANA_EN, &val);
#ifdef PMU_DCDC_ULP_MODE
        val |= REG_DCDC2_ULP_MODE_NORMAL | REG_DCDC2_ULP_MODE_DSLEEP | REG_PU_DCDC2_DR | REG_PU_DCDC2;
#elif defined(DCDC_ULP_LP_ON)
        val = (val & ~REG_DCDC2_ULP_MODE_NORMAL) | REG_DCDC2_ULP_MODE_DSLEEP | REG_PU_DCDC2_DR | REG_PU_DCDC2;
#else
        val = (val & ~REG_DCDC2_ULP_MODE_NORMAL) | REG_PU_DCDC2_DR | REG_PU_DCDC2;
#endif
        pmu_write(PMU_REG_DCDC_ANA_EN, val);

        pmu_ana_set_volt(1, PMU_POWER_MODE_ANA_DCDC);
        pmu_dig_set_volt(1, PMU_POWER_MODE_ANA_DCDC);

        hal_sys_timer_delay_us(PMU_DCDC_PU_STABLE_TIME_US);

        // Disable vana ldo
        pmu_module_config(PMU_ANA,PMU_MANUAL_MODE,PMU_LDO_OFF,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);

        pmu_read(PMU_REG_DCDC_ANA_CFG_19, &val);
        val &= ~DCDC2_REG_BYPASS;
        pmu_write(PMU_REG_DCDC_ANA_CFG_19, val);
    }
}

static void pmu_dcdc_dual_mode_en(void)
{
    unsigned short val;

    pmu_read(PMU_REG_DCDC_ANA_CFG_19, &val);
    val |= DCDC2_REG_BYPASS;
    pmu_write(PMU_REG_DCDC_ANA_CFG_19, val);
    pmu_read(PMU_REG_DCDC_DIG_CFG_36, &val);
    val |= DCDC1_REG_BYPASS;
    pmu_write(PMU_REG_DCDC_DIG_CFG_36, val);

    // Enable vana dcdc
    pmu_read(PMU_REG_DCDC_ANA_EN, &val);
#ifdef PMU_DCDC_ULP_MODE
    val |= REG_DCDC2_ULP_MODE_NORMAL | REG_DCDC2_ULP_MODE_DSLEEP | REG_PU_DCDC2_DR | REG_PU_DCDC2;
#elif defined(DCDC_ULP_LP_ON)
    val = (val & ~REG_DCDC2_ULP_MODE_NORMAL) | REG_DCDC2_ULP_MODE_DSLEEP | REG_PU_DCDC2_DR | REG_PU_DCDC2;
#else
    val = (val & ~(REG_DCDC2_ULP_MODE_NORMAL | REG_DCDC2_ULP_MODE_DSLEEP)) | REG_PU_DCDC2_DR | REG_PU_DCDC2;
#endif
    pmu_write(PMU_REG_DCDC_ANA_EN, val);
    // Enable vcore dcdc
    pmu_read(PMU_REG_DCDC_DIG_EN, &val);
    if((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) {
        val = (val & ~(REG_UVLO_SEL_MASK | REG_DCDC1_VREF_SEL_DSLEEP_MASK | REG_DCDC1_VREF_SEL_NORMAL_MASK)) |
              REG_UVLO_SEL(0) | REG_DCDC1_VREF_SEL_DSLEEP(0) | REG_DCDC1_VREF_SEL_NORMAL(0);
        val |= REG_DCDC1_BURST_MODE_SEL_DSLEEP | REG_DCDC1_BURST_MODE_SEL_NORMAL;
    }
#ifdef PMU_DCDC_ULP_MODE
    val |= REG_DCDC1_ULP_MODE_NORMAL | REG_DCDC1_ULP_MODE_DSLEEP | REG_PU_DCDC1_DR | REG_PU_DCDC1;
#elif defined(DCDC_ULP_LP_ON)
    val = (val & ~REG_DCDC1_ULP_MODE_NORMAL) | REG_DCDC1_ULP_MODE_DSLEEP | REG_PU_DCDC1_DR | REG_PU_DCDC1;
#else
    val = (val & ~(REG_DCDC1_ULP_MODE_NORMAL | REG_DCDC1_ULP_MODE_DSLEEP)) | REG_PU_DCDC1_DR | REG_PU_DCDC1;
#endif
    pmu_write(PMU_REG_DCDC_DIG_EN, val);

    pmu_ana_set_volt(1, PMU_POWER_MODE_DIG_DCDC);
    pmu_dig_set_volt(1, PMU_POWER_MODE_DIG_DCDC);

    hal_sys_timer_delay_us(PMU_DCDC_PU_STABLE_TIME_US);

    // Disable vana ldo
    pmu_module_config(PMU_ANA,PMU_MANUAL_MODE,PMU_LDO_OFF,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);
    // Disable vcore ldo
    pmu_module_config(PMU_DIG,PMU_MANUAL_MODE,PMU_LDO_OFF,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);

    pmu_read(PMU_REG_DCDC_ANA_CFG_19, &val);
    val &= ~DCDC2_REG_BYPASS;
    pmu_write(PMU_REG_DCDC_ANA_CFG_19, val);
    pmu_read(PMU_REG_DCDC_DIG_CFG_36, &val);
    val &= ~DCDC1_REG_BYPASS;
    pmu_write(PMU_REG_DCDC_DIG_CFG_36, val);
}

void pmu_mode_change(enum PMU_POWER_MODE_T mode)
{
    uint32_t lock;

    if (pmu_power_mode == mode || mode == PMU_POWER_MODE_NONE) {
        return;
    }

    lock = int_lock();

    if (mode == PMU_POWER_MODE_ANA_DCDC) {
        pmu_dcdc_ana_mode_en();
    } else if (mode == PMU_POWER_MODE_DIG_DCDC) {
        pmu_dcdc_dual_mode_en();
    } else if (mode == PMU_POWER_MODE_LDO) {
        pmu_ldo_mode_en();
    }

    pmu_power_mode = mode;

    int_unlock(lock);
}

void pmu_sleep_en(unsigned char sleep_en)
{
    unsigned short val;

    pmu_read(PMU_REG_SLEEP_CFG, &val);
    if (sleep_en) {
        val |= SLEEP_ALLOW;
    } else {
        val &= ~SLEEP_ALLOW;
    }
    pmu_write(PMU_REG_SLEEP_CFG, val);

    pmu_itn_sleep_en(sleep_en);
}

#if defined(PMU_INIT) || (!defined(FPGA) && !defined(PROGRAMMER))
static uint32_t pmu_vcodec_mv_to_val(uint16_t mv)
{
    uint32_t val;

#ifdef VHPPA2VCODEC
    if (mv == 1900) {
        val = PMU_HPPA2CODEC_1_9V;
    } else if (mv >= 1950) {
        val = PMU_HPPA2CODEC_1_95V;
    } else {
        val = PMU_HPPA2CODEC_1_7V;
    }
#else
    if (mv == 1900) {
        val = PMU_CODEC_1_9V;
    } else if (mv >= 1950) {
        val = PMU_CODEC_1_9V;
    } else {
        val = PMU_CODEC_1_7V;
    }
#endif

    return val;
}

static uint32_t pmu_ldo_hppa_mv_to_val(uint16_t mv)
{
    uint32_t val;

    if (mv == 1600) {
        val = PMU_VHPPA_1_6V;
    } else if (mv == 1700) {
        val = PMU_VHPPA_1_7V;
    } else if (mv >= 1900) {
        val = PMU_VHPPA_1_9V;
    } else {
        val = PMU_VHPPA_1_8V;
    }

    return val;
}

static void POSSIBLY_UNUSED pmu_ldo_hppa_get_volt(unsigned short *normal_vp, unsigned short *dsleep_vp)
{
    uint16_t val;

    pmu_read(PMU_REG_HPPA_LDO_EN, &val);
    val = GET_BITFIELD(val, REG_RES_SEL_VHPPA);
    if (normal_vp) {
        *normal_vp = val;
    }
    if (dsleep_vp) {
        *dsleep_vp = val;
    }
}

static void pmu_ldo_hppa_set_volt(unsigned short normal_v,unsigned short dsleep_v)
{
    uint16_t val;

    pmu_read(PMU_REG_HPPA_LDO_EN, &val);
    val = SET_BITFIELD(val, REG_RES_SEL_VHPPA, normal_v);
    pmu_write(PMU_REG_HPPA_LDO_EN, val);
}

static void pmu_ldo_hppa_en(int enable)
{
    uint16_t val;

    pmu_read(PMU_REG_HPPA_LDO_EN, &val);
    if (enable) {
        val |= (REG_PU_LDO_VHPPA_DSLEEP | REG_PU_LDO_VHPPA_EN);
    } else {
        val &= ~(REG_PU_LDO_VHPPA_DSLEEP | REG_PU_LDO_VHPPA_EN);
    }
    pmu_write(PMU_REG_HPPA_LDO_EN, val);
}

#ifndef HPPA_LDO_ON
static uint32_t pmu_dcdc_hppa_mv_to_val(uint16_t mv)
{
    uint32_t val;

    if (mv == 1600) {
        val = PMU_DCDC_HPPA_1_6V;
    } else if (mv == 1700) {
        val = PMU_DCDC_HPPA_1_7V;
    } else if (mv == 1900) {
        val = PMU_DCDC_HPPA_1_9V;
    } else if (mv >= 1950) {
        val = PMU_DCDC_HPPA_1_95V;
    } else {
        val = PMU_DCDC_HPPA_1_8V;
    }

    return val;
}

static void pmu_dcdc_hppa_get_volt(unsigned short *normal_vp, unsigned short *dsleep_vp)
{
    unsigned short val;

    pmu_read(PMU_REG_DCDC_HPPA_VOLT, &val);
    if (normal_vp) {
        *normal_vp = GET_BITFIELD(val, REG_DCDC3_VBIT_NORMAL);
    }
    if (dsleep_vp) {
        *dsleep_vp = GET_BITFIELD(val, REG_DCDC3_VBIT_DSLEEP);
    }

#ifdef PMU_DCDC_CALIB
    if (normal_vp) {
        *normal_vp = pmu_reg_val_add(*normal_vp, -pmu_dcdc_hppa_comp, MAX_DCDC3_VBIT_VAL);
    }
    if (dsleep_vp) {
        *dsleep_vp = pmu_reg_val_add(*dsleep_vp, -pmu_hppa_lp_comp, MAX_DCDC3_VBIT_VAL);
    }
#endif
}

static void pmu_dcdc_hppa_set_volt(unsigned short normal_v,unsigned short dsleep_v)
{
    unsigned short val;

#ifdef PMU_DCDC_CALIB
    normal_v = pmu_reg_val_add(normal_v, pmu_dcdc_hppa_comp, MAX_DCDC3_VBIT_VAL);
    dsleep_v = pmu_reg_val_add(dsleep_v, pmu_hppa_lp_comp, MAX_DCDC3_VBIT_VAL);
#endif

    pmu_read(PMU_REG_DCDC_HPPA_VOLT, &val);
    val &= ~REG_DCDC3_VBIT_DSLEEP_MASK;
    val &= ~REG_DCDC3_VBIT_NORMAL_MASK;
    val |= REG_DCDC3_VBIT_DSLEEP(dsleep_v);
    val |= REG_DCDC3_VBIT_NORMAL(normal_v);
    pmu_write(PMU_REG_DCDC_HPPA_VOLT, val);

    if (normal_v > dsleep_v) {
        dcdc_ramp_map |= REG_DCDC3_RAMP_EN;
    } else {
        dcdc_ramp_map &= ~REG_DCDC3_RAMP_EN;
    }
}

static void pmu_dcdc_hppa_en(int enable)
{
    uint16_t val;

    if (enable) {
        pmu_read(PMU_REG_DCDC_HPPA_CFG_1D, &val);
        val |= DCDC3_REG_BYPASS;
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1D, val);
    }

    pmu_read(PMU_REG_DCDC_HPPA_EN, &val);
    if (enable) {
#ifdef PMU_DCDC_ULP_MODE
    val |= REG_DCDC3_ULP_MODE_NORMAL | REG_DCDC3_ULP_MODE_DSLEEP | REG_PU_DCDC3_DR | REG_PU_DCDC3;
#elif defined(DCDC_ULP_LP_ON)
        val = (val & ~REG_DCDC3_ULP_MODE_NORMAL) | REG_DCDC3_ULP_MODE_DSLEEP | REG_PU_DCDC3_DR | REG_PU_DCDC3;
#else
        val = (val & ~(REG_DCDC3_ULP_MODE_NORMAL | REG_DCDC3_ULP_MODE_DSLEEP)) | REG_PU_DCDC3_DR | REG_PU_DCDC3;
#endif
    } else {
        val = (val & ~REG_PU_DCDC3) | REG_PU_DCDC3_DR;
    }
    pmu_write(PMU_REG_DCDC_HPPA_EN, val);

    if (enable) {
        hal_sys_timer_delay_us(PMU_DCDC_PU_STABLE_TIME_US);
        pmu_read(PMU_REG_DCDC_HPPA_CFG_1D, &val);
        val &= ~DCDC3_REG_BYPASS;
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1D, val);
    }
}
#endif

static uint32_t pmu_vhppa_mv_to_val(uint16_t mv)
{
#ifdef HPPA_LDO_ON
    return pmu_ldo_hppa_mv_to_val(mv);
#else
    return pmu_dcdc_hppa_mv_to_val(mv);
#endif
}

static void POSSIBLY_UNUSED pmu_hppa_get_volt(unsigned short *normal_vp, unsigned short *dsleep_vp)
{
#ifdef HPPA_LDO_ON
    pmu_ldo_hppa_get_volt(normal_vp, dsleep_vp);
#else
    pmu_dcdc_hppa_get_volt(normal_vp, dsleep_vp);
#endif
}

static void pmu_hppa_set_volt(unsigned short normal_v,unsigned short dsleep_v)
{
#ifdef HPPA_LDO_ON
    pmu_ldo_hppa_set_volt(normal_v, dsleep_v);
#else
    pmu_dcdc_hppa_set_volt(normal_v, dsleep_v);
#endif
}

static void pmu_hppa_en(int enable)
{
#ifdef HPPA_LDO_ON
    pmu_ldo_hppa_en(enable);
#else
    pmu_dcdc_hppa_en(enable);
#endif
}

static void pmu_hppa_dcdc_to_ldo(void)
{
#ifndef HPPA_LDO_ON
    uint16_t val;
    uint16_t act_volt;

    pmu_ldo_hppa_en(true);

    val = pmu_dcdc_hppa_mv_to_val(vhppa_mv);
    pmu_dcdc_hppa_get_volt(&act_volt, NULL);
    if (val < act_volt) {
        val = pmu_ldo_hppa_mv_to_val(HPPA_RAMP_UP_VOLT_MV);
    } else {
        val = pmu_ldo_hppa_mv_to_val(vhppa_mv);
    }
    pmu_ldo_hppa_set_volt(val, val);

    hal_sys_timer_delay_us(PMU_LDO_PU_STABLE_TIME_US);
#endif
}

static void BOOT_TEXT_FLASH_LOC pmu_dig_init_volt(void)
{
    uint16_t ldo_volt;
    uint16_t logic_volt;
    uint16_t val;

    pmu_dig_get_target_volt(&ldo_volt, NULL, &logic_volt);

    pmu_read(PMU_REG_DIG_CFG, &val);
    if (GET_BITFIELD(val, LDO_DIG_VBIT_NORMAL) < ldo_volt) {
        val = SET_BITFIELD(val, LDO_DIG_VBIT_NORMAL, ldo_volt);
        pmu_write(PMU_REG_DIG_CFG, val);
    }

    pmu_itn_logic_dig_init_volt(logic_volt, dig_lp_logic);
}

int pmu_codec_volt_ramp_up(void)
{
#ifndef VMEM_ON
    unsigned short normal, dsleep;
    unsigned short target;
    const unsigned short step =
#ifdef HPPA_LDO_ON
        1;
#else
        8;
#endif

    if (vcodec_off) {
        target = pmu_vhppa_mv_to_val(HPPA_RAMP_UP_VOLT_MV);

        pmu_hppa_get_volt(&normal, &dsleep);

        if (normal < target) {
            if (normal + step < target) {
                normal += step;
            } else {
                normal = target;
            }

            pmu_hppa_set_volt(normal, dsleep);

            return 1;
        }
    }

    return 0;
#endif
}

int pmu_codec_volt_ramp_down(void)
{
#ifndef VMEM_ON
    unsigned short normal, dsleep;
    unsigned short target;
    const unsigned short step =
#ifdef HPPA_LDO_ON
        1;
#else
        8;
#endif

    if (vcodec_off) {
        target = pmu_vhppa_mv_to_val(vhppa_mv);

        pmu_hppa_get_volt(&normal, &dsleep);

        if (normal > target) {
            if (normal - step > target) {
                normal -= step;
            } else {
                normal = target;
            }

            pmu_hppa_set_volt(normal, dsleep);

            return 1;
        }
    }

    return 0;
#endif
}
#endif

#ifdef DCDC_HIGH_EFFICIENCY_CTRL
int pmu_high_dcdc_efficiency_mode_enable(bool enable)
{
    uint16_t val;
    int edge;

    if(pmu_type == PMU_CHIP_TYPE_1802) 
    {
        if (enable)
        {
            edge = 3;
        } 
        else 
        {
            edge = 0;
        }

        pmu_read(PMU_REG_DCDC3_EDGE, &val);
        val = SET_BITFIELD(val, REG_DCDC3_EDGE_CON_N, edge);
        val = SET_BITFIELD(val, REG_DCDC3_EDGE_CON_P, edge);
        pmu_write(PMU_REG_DCDC3_EDGE, val);

        pmu_read(PMU_REG_DCDC2_EDGE, &val);
        val = SET_BITFIELD(val, REG_DCDC2_EDGE_CON_N, edge);
        val = SET_BITFIELD(val, REG_DCDC2_EDGE_CON_P, edge);
        pmu_write(PMU_REG_DCDC2_EDGE, val);

        if (enable)
        {
            edge = 2;
        }
        else
        {
            edge = 0;
        }


        pmu_read(PMU_REG_DCDC1_EDGE, &val);
        val = SET_BITFIELD(val, REG_DCDC1_EDGE_CON_N, edge);
        val = SET_BITFIELD(val, REG_DCDC1_EDGE_CON_P, edge);
        pmu_write(PMU_REG_DCDC1_EDGE, val);

        
    }
    else if((pmu_type == PMU_CHIP_TYPE_1803) || pmu_metal_id >= HAL_CHIP_METAL_ID_1)
    {
        if(enable==1)
            val = 0x0217;
        else
            val = 0x0211;
        pmu_write(PMU_REG_DCDC_ANA_CFG_17, val);
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1B, val);
        if(enable==1)
            val = 0x0215;
        else
            val = 0x0211;
        pmu_write( PMU_REG_DCDC_DIG_CFG_34, val);
    }
    else
    {
        //nothing
    }
    return 0;
}
#endif

int BOOT_TEXT_FLASH_LOC pmu_open(void)
{
#if defined(PMU_INIT) || (!defined(FPGA) && !defined(PROGRAMMER))

    uint16_t val;
    enum PMU_POWER_MODE_T mode;
    enum PMU_MODUAL_T codec;

    ASSERT(!vcodec_off || vcodec_mv == vhppa_mv,
        "Invalid vcodec/vhppa cfg: vcodec_off=%d vcodec_mv=%u vhppa_mv=%u", vcodec_off, vcodec_mv, vhppa_mv);
    ASSERT(vcodec_mv == 1600 || vcodec_mv == 1700 || vcodec_mv == 1800 || vcodec_mv == 1900 || vcodec_mv == 1950,
        "Invalid vcodec cfg: vcodec_mv=%u", vcodec_mv);
    ASSERT(vhppa_mv == 1600 || vhppa_mv == 1700 || vhppa_mv == 1800 || vhppa_mv == 1900 || vhppa_mv == 1950,
        "Invalid vhppa cfg: vhppa_mv=%u", vhppa_mv);

    // Disable and clear all PMU irqs by default
    pmu_write(PMU_REG_INT_MASK, 0);
    pmu_write(PMU_REG_INT_EN, 0);
    // PMU irqs cannot be cleared by PMU soft reset
    pmu_read(PMU_REG_CHARGER_STATUS, &val);
    pmu_write(PMU_REG_CHARGER_STATUS, val);
    pmu_read(PMU_REG_INT_STATUS, &val);
    pmu_write(PMU_REG_INT_CLR, val);

    pmu_read(PMU_REG_POWER_KEY_CFG, &val);
    // Allow PMU to sleep when power key is pressed
    val &= ~POWERKEY_WAKEUP_OSC_EN;
    // Increase big bandgap startup time (stable time)
    val = SET_BITFIELD(val, REG_VCORE_SSTIME_MODE, 2);
    pmu_write(PMU_REG_POWER_KEY_CFG, val);

    if (pmu_type == PMU_CHIP_TYPE_1803) {
        pmu_get_efuse(PMU_EFUSE_PAGE_SW_CFG, &val);
        if (val & (1 << 3)) {
            vmic3_repl_vmic5 = true;
        }
    }

#ifndef PMU_FORCE_LP_MODE
#ifdef FORCE_BIG_BANDGAP
    // Force big bandgap
    pmu_read(PMU_REG_INT_EN, &val);
    val |= REG_BG_SLEEP_MSK;
    pmu_write(PMU_REG_INT_EN, val);
    pmu_read(PMU_REG_BIAS_CFG, &val);
    val |= BG_CONSTANT_GM_BIAS_DR | BG_CONSTANT_GM_BIAS_REG;
    val |= BG_CORE_EN_DR | BG_CORE_EN_REG;
    pmu_write(PMU_REG_BIAS_CFG, val);
    hal_sys_timer_delay_us(20);
    val |= BG_VBG_SEL_DR | BG_VBG_SEL_REG;
    pmu_write(PMU_REG_BIAS_CFG, val);
#else
    // Allow low power bandgap
    pmu_read(PMU_REG_BIAS_CFG, &val);
    val &= ~BG_VBG_SEL_DR;
    pmu_write(PMU_REG_BIAS_CFG, val);
#endif
#endif

    // Init DCDC settings part 1
    if((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) {
        pmu_write(PMU_REG_DCDC_ANA_CFG_16, 0x8E1F);
        pmu_write(PMU_REG_DCDC_ANA_CFG_17, 0x6011);
        pmu_write(PMU_REG_DCDC_ANA_CFG_18, 0xA444);
        pmu_write(PMU_REG_DCDC_ANA_CFG_19, 0x4218);

        pmu_write(PMU_REG_DCDC_HPPA_CFG_1A, 0x8E1F);
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1B, 0x6011);
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1C, 0xA444);
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1D, 0x4218);

        pmu_write(PMU_REG_DCDC_DIG_CFG_33, 0x8E1F);
        pmu_write(PMU_REG_DCDC_DIG_CFG_34, 0x6011);
        pmu_write(PMU_REG_DCDC_DIG_CFG_35, 0xA444);
        pmu_write(PMU_REG_DCDC_DIG_CFG_36, 0x4218);


        // Fix working current too high, wrong default val.
        pmu_read(PMU_REG_DCDC_HPPA_CFG_1B, &val);
        val = SET_BITFIELD(val, PMU1802_1807_DCDC3_DELTAV, 0x20);
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1B, val);

        pmu_read(PMU_REG_DCDC_ANA_CFG_17, &val);
        val = SET_BITFIELD(val, PMU1802_1807_DCDC2_DELTAV, 0x20);
        pmu_write(PMU_REG_DCDC_ANA_CFG_17, val);

        pmu_read(PMU_REG_DCDC_DIG_CFG_34, &val);
        val = SET_BITFIELD(val, PMU1802_1807_DCDC1_DELTAV, 0x20);
        pmu_write(PMU_REG_DCDC_DIG_CFG_34, val);


        pmu_read(PMU_REG_MIC_LDO_EN, &val);
        val |= DCDC1_OFFSET_CURRENT_EN;
        pmu_write(PMU_REG_MIC_LDO_EN, val);

        pmu_read(PMU_REG_DCDC1_OFFSET, &val);
        val = SET_BITFIELD(val, DCDC1_OFFSET_BIT, 0xA0);
        pmu_write(PMU_REG_DCDC1_OFFSET, val);

        pmu_read(PMU_REG_LDO_VCORE_L, &val);
        val |= REG_LDO_VCORE_L_BYPASS_DR;
        pmu_write(PMU_REG_LDO_VCORE_L, val);

        pmu_read(PMU_REG_PU_LDO, &val);
        val &= ~(REG_PU_LDO_DIG_L_REG | REG_PU_VMEM_RET_LDO |
            REG_PU_VDCDC_RET_LDO | REG_PU_VBAT2VCORE_RET_LDO);
        val |= REG_PU_LDO_DIG_L_DSLEEP | REG_PU_LDO_DIG_L_DR | REG_PU_VMEM_RET_LDO_DR |
            REG_PU_VDCDC_RET_LDO_DR | REG_PU_VBAT2VCORE_RET_LDO_DR;
        if (pmu_metal_id == HAL_CHIP_METAL_ID_0) {
            val = (val & ~REG_PU_VIO_RET_LDO) | REG_PU_VIO_RET_LDO_DR;
            val &= ~(REG_PU_VCORE_RET_LDO | REG_PU_VCORE_RET_LDO_DR);
        } else {
            val = (val & ~REG_PU_VIO_RET_LDO_DR) | REG_PU_VIO_RET_LDO;
            val = (val & ~REG_PU_VCORE_RET_LDO) | REG_PU_VCORE_RET_LDO_DR;
        }
        pmu_write(PMU_REG_PU_LDO, val);
    } else if ((pmu_type == PMU_CHIP_TYPE_1803) || pmu_metal_id >= HAL_CHIP_METAL_ID_1) {
#ifdef DCDC_HIGH_EFFICIENCY_CTRL
        val = 0x0217;
#else
        val = 0x0211;
#endif
        pmu_write(PMU_REG_DCDC_ANA_CFG_16, 0x8E0F);
        pmu_write(PMU_REG_DCDC_ANA_CFG_17, val);
        pmu_write(PMU_REG_DCDC_ANA_CFG_18, 0xA484);
        pmu_write(PMU_REG_DCDC_ANA_CFG_19, 0xE210);

        pmu_write(PMU_REG_DCDC_HPPA_CFG_1A, 0x8E0F);
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1B, val);
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1C, 0xA484);
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1D, 0xE210);

        pmu_write(PMU_REG_DCDC_DIG_CFG_33, 0x8E0F);
#ifdef DCDC_HIGH_EFFICIENCY_CTRL
        val = 0x0215;
#else
        val = 0x0211;
#endif
        pmu_write(PMU_REG_DCDC_DIG_CFG_34, val);
        pmu_write(PMU_REG_DCDC_DIG_CFG_35, 0xA484);
        pmu_write(PMU_REG_DCDC_DIG_CFG_36, 0xE210);

        // Fix working current too high, wrong default val.
        pmu_read(PMU_REG_DCDC_HPPA_CFG_1B, &val);
        val = SET_BITFIELD(val, PMU1800_1803_DCDC3_DELTAV, 0x8);
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1B, val);

        pmu_read(PMU_REG_DCDC_ANA_CFG_17, &val);
        val = SET_BITFIELD(val, PMU1800_1803_DCDC2_DELTAV, 0x8);
        pmu_write(PMU_REG_DCDC_ANA_CFG_17, val);

        pmu_read(PMU_REG_DCDC_DIG_CFG_34, &val);
        val = SET_BITFIELD(val, PMU1800_1803_DCDC1_DELTAV, 0x8);
        pmu_write(PMU_REG_DCDC_DIG_CFG_34, val);

        pmu_read(PMU_REG_MIC_LDO_EN, &val);
        val |= DCDC1_OFFSET_CURRENT_EN;
        pmu_write(PMU_REG_MIC_LDO_EN, val);

        pmu_read(PMU_REG_DCDC1_OFFSET, &val);
        val = SET_BITFIELD(val, DCDC1_OFFSET_BIT, 0xE0);
        pmu_write(PMU_REG_DCDC1_OFFSET, val);
    }

#ifdef PMU_DCDC_CALIB
    pmu_get_dcdc_calib_value();
#endif

#ifdef PMU_LDO_VCORE_CALIB
    pmu_get_ldo_vcore_calib_value();
#endif

#ifndef NO_SLEEP
    pmu_sleep_en(true);
#endif

    // Disable vbat2vcore
    pmu_module_config(PMU_BAT2DIG,PMU_MANUAL_MODE,PMU_LDO_OFF,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);
#ifdef LBRT
    // Enable vbuck2ana
    pmu_module_config(PMU_BUCK2ANA,PMU_MANUAL_MODE,PMU_LDO_ON,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_ON);
    pmu_module_set_volt(PMU_BUCK2ANA, PMU_VBUCK2ANA_1_3V, PMU_VBUCK2ANA_1_3V);
    // Disable vana dcdc
    pmu_read(PMU_REG_HPPA_LDO_EN, &val);
    val &= ~REG_BYPASS_VBUCK2ANA;
    pmu_write(PMU_REG_HPPA_LDO_EN, val);
#else
    // Disable vbuck2ana
    pmu_module_config(PMU_BUCK2ANA,PMU_MANUAL_MODE,PMU_LDO_OFF,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);
#endif

    // Disable vhppa ldo
    pmu_ldo_hppa_en(false);

#ifdef VMEM_ON
    pmu_module_config(PMU_MEM,PMU_MANUAL_MODE,PMU_LDO_ON,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_ON);
    pmu_module_ramp_volt(PMU_MEM, PMU_VMEM_1_8V, PMU_VMEM_1_8V);
    pmu_hppa_en(false);
    // VHPPA will be enabled/disabled dynamically. Enable soft start.
    pmu_read(PMU_REG_DCDC_HPPA_CFG_1D, &val);
    val |= DCDC3_SOFT_START_EN;
    pmu_write(PMU_REG_DCDC_HPPA_CFG_1D, val);
#else
    // vmem and vhppa is shorted -- enable vhppa dcdc and disable vmem ldo
    val = pmu_vhppa_mv_to_val(1800);
    pmu_hppa_set_volt(val, val);
    pmu_hppa_en(true);
    pmu_module_config(PMU_MEM,PMU_MANUAL_MODE,PMU_LDO_OFF,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);
#endif

    pmu_module_config(PMU_GP,PMU_MANUAL_MODE,PMU_LDO_OFF,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);

#ifndef PROGRAMMER
#ifdef VUSB_ON
    pmu_vusb_req |= PMU_VUSB_REQ_INIT;
#endif
    // Disable usbphy power, and vusb if possible
    pmu_usb_config(PMU_USB_CONFIG_TYPE_NONE);
#ifdef VUSB_ON
    pmu_vusb_req &= ~PMU_VUSB_REQ_INIT;
#endif
#endif

#ifdef __BEST_FLASH_VIA_ANA__
    pmu_module_config(PMU_IO,PMU_AUTO_MODE,PMU_LDO_ON,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);
#else
    pmu_module_config(PMU_IO,PMU_AUTO_MODE,PMU_LDO_ON,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_ON);
#endif
    if (pmu_type == PMU_CHIP_TYPE_1803) {
        vio_act_normal = IO_VOLT_ACTIVE_NORMAL_1803;
        vio_lp = IO_VOLT_SLEEP_1803;
    }
    pmu_module_ramp_volt(PMU_IO, vio_lp, vio_act_normal);

    // Disable vcodec
    pmu_module_config(PMU_CODEC,PMU_MANUAL_MODE,PMU_LDO_OFF,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);
    // Disable vhppa2vcodec
    pmu_module_config(PMU_HPPA2CODEC,PMU_MANUAL_MODE,PMU_LDO_OFF,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);
#ifdef VHPPA2VCODEC
    codec = PMU_HPPA2CODEC;
#else
    codec = PMU_CODEC;
#endif
    val = pmu_vcodec_mv_to_val(vcodec_mv);
    // No need to ramp because the LDO is disabled (and its soft start is enabled)
    pmu_module_set_volt(codec, val, val);

    val = pmu_vhppa_mv_to_val(vhppa_mv);
#ifdef VMEM_ON
    // No need to ramp because VHPPA is disabled (and its soft start is enabled)
    pmu_hppa_set_volt(val, val);
#else
    uint16_t old_act_volt;
    uint16_t old_lp_volt;

    // VHPPA is on. Ramp is needed.
    pmu_hppa_get_volt(&old_act_volt, &old_lp_volt);
    if (old_act_volt < val) {
        while (old_act_volt++ < val) {
            pmu_hppa_set_volt(old_act_volt, val);
        }
    } else if (old_act_volt != val || old_lp_volt != val) {
        pmu_hppa_set_volt(val, val);
    }
#endif

    uint16_t mic_volt_mv;
#ifdef DIGMIC_HIGH_VOLT
    mic_volt_mv = 3300;
#else
    mic_volt_mv = 2200;
#endif
    pmu_codec_mic_bias_set_volt((AUD_VMIC_MAP_VMIC1 << MAX_VMIC_CH_NUM) - 1, mic_volt_mv);

#ifdef DIG_DCDC_MODE
    mode = PMU_POWER_MODE_DIG_DCDC;
#elif defined(ANA_DCDC_MODE)
    mode = PMU_POWER_MODE_ANA_DCDC;
#else // LDO_MODE
    mode = PMU_POWER_MODE_LDO;
#endif

    pmu_mode_change(mode);

#ifdef PMU_FORCE_LP_MODE
#ifndef DIG_DCDC_MODE
#error "VCORE LDO cannot be forced to run in LP mode"
#endif
    // Force LP mode
    pmu_module_force_lp_config();
    hal_sys_timer_delay(MS_TO_TICKS(1));

    // Reduce LDO voltage ripple
    pmu_read(PMU_REG_HPPA_LDO_EN, &val);
    val = SET_BITFIELD(val, REG_LP_BIAS_SEL_LDO, 0);
    pmu_write(PMU_REG_HPPA_LDO_EN, val);

    // Switch to little bandgap
    pmu_read(PMU_REG_BIAS_CFG, &val);
    val = (val & ~BG_VBG_SEL_REG) | BG_VBG_SEL_DR;
    pmu_write(PMU_REG_BIAS_CFG, val);
    hal_sys_timer_delay(MS_TO_TICKS(1));

    if (big_bandgap_user_map == 0) {
        // Disable big bandgap
        val = (val & ~(PU_BIAS_LDO_REG | BG_CONSTANT_GM_BIAS_REG | BG_CORE_EN_REG | BG_NOTCH_EN_REG)) |
            PU_BIAS_LDO_DR | BG_CONSTANT_GM_BIAS_DR | BG_CORE_EN_DR | BG_NOTCH_EN_DR;
        pmu_write(PMU_REG_BIAS_CFG, val);
    }
#endif

#ifdef RTC_ENABLE
    pmu_rtc_restore_context();
#endif

#if defined(MCU_HIGH_PERFORMANCE_MODE)
    pmu_high_performance_mode_enable(true);
#endif

    // Init DCDC settings part 2
    if(pmu_type != PMU_CHIP_TYPE_1802) {
        if ((pmu_type == PMU_CHIP_TYPE_1803) || pmu_metal_id >= HAL_CHIP_METAL_ID_1) {
            hal_sys_timer_delay_us(5000);
            pmu_write(PMU_REG_DCDC_ANA_CFG_16, 0x8E1F);
            pmu_write(PMU_REG_DCDC_HPPA_CFG_1A, 0x8E1F);
            pmu_write(PMU_REG_DCDC_DIG_CFG_33, 0x8E1F);
        }
    }

#ifdef BT_RCOSC_CAL
    pmu_bt_rcosc_cal(true);
#endif

#endif // PMU_INIT || (!FPGA && !PROGRAMMER)

    return 0;
}

enum PMU_CHIP_TYPE_T pmu_get_chip_type(void)
{
    return pmu_type;
}

void pmu_big_bandgap_enable(enum PMU_BIG_BANDGAP_USER_T user, int enable)
{
#ifdef PMU_FORCE_LP_MODE
    uint16_t val;
    bool update = false;

    if (enable) {
        if (big_bandgap_user_map == 0) {
            update = true;
        }
        big_bandgap_user_map |= user;
    } else {
        if (big_bandgap_user_map & user) {
            big_bandgap_user_map &= ~user;
            if (big_bandgap_user_map == 0) {
                update = true;
            }
        }
    }

    if (!update) {
        return;
    }

    pmu_read(PMU_REG_BIAS_CFG, &val);
    val |= PU_BIAS_LDO_DR | BG_CONSTANT_GM_BIAS_DR | BG_CORE_EN_DR | BG_NOTCH_EN_DR;
    if (enable) {
        val |= PU_BIAS_LDO_REG | BG_CONSTANT_GM_BIAS_REG | BG_CORE_EN_REG | BG_NOTCH_EN_REG;
    } else {
        val &= ~(PU_BIAS_LDO_REG | BG_CONSTANT_GM_BIAS_REG | BG_CORE_EN_REG | BG_NOTCH_EN_REG);
    }
    pmu_write(PMU_REG_BIAS_CFG, val);
    if (enable) {
        hal_sys_timer_delay(MS_TO_TICKS(1));
    }
#endif
}

void pmu_codec_vad_save_power(void)
{
    uint16_t val;

    // reduce ldo power supply
    pmu_read(PMU_REG_DCDC_RAMP_EN, &val);
    val &= ~REG_MIC_BIASA_IX2;
    pmu_write(PMU_REG_DCDC_RAMP_EN, val);

    // enable dcdc1 low power mode
    pmu_read(PMU_REG_DCDC_DIG_EN, &val);
    val |= REG_DCDC1_ULP_MODE_NORMAL;
    pmu_write(PMU_REG_DCDC_DIG_EN, val);

    // enable dcdc2 low power mode
    pmu_read(PMU_REG_DCDC_ANA_EN, &val);
    val |= REG_DCDC2_ULP_MODE_NORMAL;
    pmu_write(PMU_REG_DCDC_ANA_EN, val);

    // enable dcdc3 low power mode
    pmu_read(PMU_REG_DCDC_HPPA_EN, &val);
    val |= REG_DCDC3_ULP_MODE_NORMAL;
    pmu_write(PMU_REG_DCDC_HPPA_EN, val);
}

void pmu_codec_vad_restore_power(void)
{
    uint16_t val;

    // restore ldo power supply
    pmu_read(PMU_REG_DCDC_RAMP_EN, &val);
    val |= REG_MIC_BIASA_IX2;
    pmu_write(PMU_REG_DCDC_RAMP_EN, val);

    // disable dcdc1 low power mode
    pmu_read(PMU_REG_DCDC_DIG_EN, &val);
    val &= ~REG_DCDC1_ULP_MODE_NORMAL;
    pmu_write(PMU_REG_DCDC_DIG_EN, val);

    // disable dcdc2 low power mode
    pmu_read(PMU_REG_DCDC_ANA_EN, &val);
    val &= ~REG_DCDC2_ULP_MODE_NORMAL;
    pmu_write(PMU_REG_DCDC_ANA_EN, val);

    // disable dcdc3 low power mode
    pmu_read(PMU_REG_DCDC_HPPA_EN, &val);
    val &= ~REG_DCDC3_ULP_MODE_NORMAL;
    pmu_write(PMU_REG_DCDC_HPPA_EN, val);
}

void pmu_sleep(void)
{
    uint16_t val;

    if (hal_get_chip_metal_id() <= HAL_CHIP_METAL_ID_2) {
        enum HAL_ANALOGIF_RF_ACCESS_T type = hal_analogif_rf_access_config(HAL_ANALOGIF_RF_ACCESS_ALL);
        pmu_write(PMU_REG_RF_384, 0x00FF);
        pmu_write(PMU_REG_RF_385, 0x0);
        pmu_write(PMU_REG_RF_386, 0x0);
        hal_analogif_rf_access_config(type);
    }

#ifdef BT_RCOSC_CAL
    pmu_bt_xtal_startup_config(1);
#endif

    if((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) {
        pmu_read(PMU_REG_DCDC_ANA_CFG_19, &val);
        val |= DCDC2_REG_BYPASS;
        pmu_write(PMU_REG_DCDC_ANA_CFG_19, val);

        pmu_read(PMU_REG_DCDC_HPPA_CFG_1D, &val);
        val |= DCDC3_REG_BYPASS;
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1D, val);

        pmu_read(PMU_REG_DCDC_DIG_CFG_36, &val);
        val |= DCDC1_REG_BYPASS;
        pmu_write(PMU_REG_DCDC_DIG_CFG_36, val);
    }

    if (dcdc_ramp_map) {
        // Enable DCDC ramp
        pmu_read(PMU_REG_DCDC_RAMP_EN, &val);
        val |= dcdc_ramp_map;
        pmu_write(PMU_REG_DCDC_RAMP_EN, val);
    }

    return;
}

void pmu_wakeup(void)
{
    uint16_t val;

    if (hal_get_chip_metal_id() <= HAL_CHIP_METAL_ID_2) {
        enum HAL_ANALOGIF_RF_ACCESS_T type = hal_analogif_rf_access_config(HAL_ANALOGIF_RF_ACCESS_ALL);
        pmu_write(PMU_REG_RF_384, 0xFFFF);
        pmu_write(PMU_REG_RF_385, 0xFFFF);
        pmu_write(PMU_REG_RF_386, 0xFFFF);
        hal_analogif_rf_access_config(type);
    }

#ifdef BT_RCOSC_CAL
    pmu_bt_xtal_startup_config(0);
#endif

    if((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) {
        pmu_read(PMU_REG_DCDC_ANA_CFG_19, &val);
        val &= ~DCDC2_REG_BYPASS;
        pmu_write(PMU_REG_DCDC_ANA_CFG_19, val);

        pmu_read(PMU_REG_DCDC_HPPA_CFG_1D, &val);
        val &= ~DCDC3_REG_BYPASS;
        pmu_write(PMU_REG_DCDC_HPPA_CFG_1D, val);

        pmu_read(PMU_REG_DCDC_DIG_CFG_36, &val);
        val &= ~DCDC1_REG_BYPASS;
        pmu_write(PMU_REG_DCDC_DIG_CFG_36, val);
    }

    if (dcdc_ramp_map) {
        // Disable DCDC ramp so that s/w can control the voltages freely
        pmu_read(PMU_REG_DCDC_RAMP_EN, &val);
        val &= ~dcdc_ramp_map;
        pmu_write(PMU_REG_DCDC_RAMP_EN, val);
    }

    return;
}

void pmu_codec_config(int enable)
{
    enum PMU_MODUAL_T codec;

#ifdef VHPPA2VCODEC
    codec = PMU_HPPA2CODEC;
#else
    codec = PMU_CODEC;
#endif

    if (vcodec_off) {
#ifdef VMEM_ON
        pmu_hppa_en(enable);
#endif
    } else {
        if (enable) {
            pmu_module_config(codec,PMU_MANUAL_MODE,PMU_LDO_ON,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);
        } else {
            pmu_module_config(codec,PMU_MANUAL_MODE,PMU_LDO_OFF,PMU_LP_MODE_ON,PMU_DSLEEP_MODE_OFF);
        }
    }
}

void pmu_codec_hppa_enable(int enable)
{
    if (!vcodec_off) {
#ifdef VMEM_ON
        pmu_hppa_en(enable);
#endif
    }
}

uint32_t pmu_codec_mic_bias_remap(uint32_t map)
{
    if (pmu_type == PMU_CHIP_TYPE_1803) {
        // Compatible with 1800 settings
        if (map & AUD_VMIC_MAP_VMIC3) {
            map = (map & ~AUD_VMIC_MAP_VMIC3) | AUD_VMIC_MAP_VMIC1;
        }
        if (1) {
            if (map & AUD_VMIC_MAP_VMIC2) {
                map = (map & ~AUD_VMIC_MAP_VMIC2) | AUD_VMIC_MAP_VMIC3;
            }
            if (map & AUD_VMIC_MAP_VMIC5) {
                map = (map & ~AUD_VMIC_MAP_VMIC5) | AUD_VMIC_MAP_VMIC3;
            }
        } else {
            if (map & AUD_VMIC_MAP_VMIC5) {
                map = (map & ~AUD_VMIC_MAP_VMIC5) | AUD_VMIC_MAP_VMIC2;
            }
        }
    } else if ((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) {
        if (map & AUD_VMIC_MAP_VMIC3) {
            map = (map & ~AUD_VMIC_MAP_VMIC3) | AUD_VMIC_MAP_VMIC1;
        }
        if (map & AUD_VMIC_MAP_VMIC5) {
            map = (map & ~AUD_VMIC_MAP_VMIC5) | AUD_VMIC_MAP_VMIC2;
        }
    } else {
        // The vmic1 pin has been replaced by the vmic3 pin in package bonding
        if (map & AUD_VMIC_MAP_VMIC1) {
            map = (map & ~AUD_VMIC_MAP_VMIC1) | AUD_VMIC_MAP_VMIC3;
        }
        // The vmic2 pin has been replaced by the vmic5 pin in package bonding
        if (map & AUD_VMIC_MAP_VMIC2) {
            map = (map & ~AUD_VMIC_MAP_VMIC2) | AUD_VMIC_MAP_VMIC5;
        }
    }

    return map;
}

void pmu_codec_mic_bias_noremap_enable(uint32_t map, int enable)
{
    uint16_t val;
    int i;
    enum PMU_REG_T bias_reg;
    uint16_t ldo_en;
    uint16_t ldo_mask;
    uint8_t vmic_num;

#ifdef PMU_FORCE_LP_MODE
    if (pmu_type == PMU_CHIP_TYPE_1803) {
        if (map & AUD_VMIC_MAP_VMIC1) {
            pmu_big_bandgap_enable(PMU_BIG_BANDGAP_USER_VMIC1, enable);
        }
#ifndef NO_VMIC2_BG_CHECK
        if (map & AUD_VMIC_MAP_VMIC2) {
            pmu_big_bandgap_enable(PMU_BIG_BANDGAP_USER_VMIC2, enable);
        }
#endif
    }
#endif

    if (pmu_type == PMU_CHIP_TYPE_1803) {
        vmic_num = MAX_EXT_VMIC_CH_NUM_1803;
    } else if ((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) {
        vmic_num = MAX_EXT_VMIC_CH_NUM_1802;
    } else {
        vmic_num = MAX_VMIC_CH_NUM;
    }

    ldo_en = 0;
    ldo_mask = 0;
    for (i = 0; i < vmic_num; i++) {
        if ((map & (AUD_VMIC_MAP_VMIC1 << i)) == 0) {
            continue;
        }
        if (i == MAX_VMIC_CH_NUM - 1) {
            bias_reg = PMU_REG_MIC_BIAS_E;
        } else {
            bias_reg = PMU_REG_MIC_BIAS_A + i;
        }
        pmu_read(bias_reg, &val);
        if (enable) {
            ldo_en |= (REG_MIC_LDOA_EN >> i);
            val |= REG_MIC_BIASA_EN;
        } else {
            ldo_mask |= (REG_MIC_LDOA_EN >> i);
            val &= ~REG_MIC_BIASA_EN;
        }
        pmu_write(bias_reg, val);
    }
    if (ldo_en || ldo_mask) {
        pmu_read(PMU_REG_MIC_LDO_EN, &val);
        val = (val & ~ldo_mask) | ldo_en;
        pmu_write(PMU_REG_MIC_LDO_EN, val);
    }
}

void pmu_codec_mic_bias_enable(uint32_t map, int enable)
{
    map = pmu_codec_mic_bias_remap(map);
    pmu_codec_mic_bias_noremap_enable(map, enable);
}

void pmu_codec_mic_bias_set_volt(uint32_t map, uint32_t mv)
{
    uint16_t val;
    int i;
    enum PMU_REG_T bias_reg;
    uint8_t volt;
    uint8_t res;
    uint8_t vmic_num;

    map = pmu_codec_mic_bias_remap(map);

    if (pmu_type == PMU_CHIP_TYPE_1803) {
        vmic_num = MAX_EXT_VMIC_CH_NUM_1803;
    } else if ((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) {
        vmic_num = MAX_EXT_VMIC_CH_NUM_1802;
    } else {
        vmic_num = MAX_VMIC_CH_NUM;
    }

    if (mv <= 1600) {
        volt = PMU_VMIC_1_6V;
    } else if (mv < 1900 + 50) {
        volt = (mv - 1600 + 50) / 100 + PMU_VMIC_1_6V;
    } else if (mv < 2500 + 50) {
        volt = (mv - 2000 + 50) / 100 + PMU_VMIC_2_0V;
    } else {
        volt = (mv - 2600 + 50) / 100 + PMU_VMIC_2_6V;
        if (volt > 0x1F) {
            volt = 0x1F;
        }
    }

    if (vcodec_mv < 1800) {
        if (volt >= 0x1F) {
            volt = 0x1F;
        } else {
            volt += 1;
        }
    }

    res = PMU_VMIC_RES_3_3V;

    for (i = 0; i < vmic_num; i++) {
        if ((map & (AUD_VMIC_MAP_VMIC1 << i)) == 0) {
            continue;
        }
        if (i == MAX_VMIC_CH_NUM - 1) {
            bias_reg = PMU_REG_MIC_BIAS_E;
        } else {
            bias_reg = PMU_REG_MIC_BIAS_A + i;
        }
        pmu_read(bias_reg, &val);
        val = (val & ~(REG_MIC_BIASA_VSEL_MASK | REG_MIC_LDOA_RES_MASK)) |
            REG_MIC_BIASA_VSEL(volt) | REG_MIC_LDOA_RES(res);
        pmu_write(bias_reg, val);
    }
}

void pmu_codec_mic_bias_noremap_lowpower_mode(uint32_t map, int enable)
{
    uint16_t val;
    uint16_t sel;

    if (map & AUD_VMIC_MAP_VMIC3) {
        pmu_read(PMU_REG_DCDC_RAMP_EN, &val);
        if (enable) {
            val &= ~REG_MIC_BIASC_IX2;
        } else {
            val |= REG_MIC_BIASC_IX2;
        }
        pmu_write(PMU_REG_DCDC_RAMP_EN, val);

        pmu_read(PMU_REG_MIC_BIAS_C, &val);
        if (enable) {
            sel = 0x3;
        } else {
            sel = 0x0;
        }
        val = SET_BITFIELD(val, REG_MIC_BIASC_CHANSEL, sel);
        pmu_write(PMU_REG_MIC_BIAS_C, val);
    }
}

void pmu_codec_mic_bias_lowpower_mode(uint32_t map, int enable)
{
    map = pmu_codec_mic_bias_remap(map);
    pmu_codec_mic_bias_noremap_lowpower_mode(map, enable);
}

void pmu_codec_adc_pre_start(void)
{
}

void pmu_codec_adc_post_start(void (*delay_ms)(uint32_t))
{
}

void pmu_codec_dac_pre_start(void)
{
}

void pmu_codec_dac_post_start(void (*delay_ms)(uint32_t))
{
}

SRAM_TEXT_LOC void pmu_flash_write_config(void)
{
#ifdef FLASH_WRITE_AT_HIGH_VCORE
    uint32_t lock;

    if (pmu_vcore_req & PMU_VCORE_FLASH_WRITE_ENABLED) {
        return;
    }

    lock = int_lock();
    pmu_vcore_req |= PMU_VCORE_FLASH_WRITE_ENABLED;
    int_unlock(lock);

    pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
#endif
}

SRAM_TEXT_LOC void pmu_flash_read_config(void)
{
#ifdef FLASH_WRITE_AT_HIGH_VCORE
    uint32_t lock;

    if ((pmu_vcore_req & PMU_VCORE_FLASH_WRITE_ENABLED) == 0) {
        return;
    }

    lock = int_lock();
    pmu_vcore_req &= ~PMU_VCORE_FLASH_WRITE_ENABLED;
    int_unlock(lock);

    pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
#endif
}

void BOOT_TEXT_FLASH_LOC pmu_flash_freq_config(uint32_t freq)
{
#if defined(PMU_INIT) || (!defined(FPGA) && !defined(PROGRAMMER))
    uint32_t lock;

    lock = int_lock();
    if (freq > 52000000) {
        // The real max freq is 120M
        //pmu_vcore_req |= PMU_VCORE_FLASH_FREQ_HIGH;
    } else {
        pmu_vcore_req &= ~PMU_VCORE_FLASH_FREQ_HIGH;
    }
    int_unlock(lock);

    if (pmu_power_mode == PMU_POWER_MODE_NONE) {
        // PMU not init yet
        pmu_dig_init_volt();
        return;
    }

    pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
#endif
}

void BOOT_TEXT_FLASH_LOC pmu_psram_freq_config(uint32_t freq)
{
}

void pmu_anc_config(int enable)
{
}

void pmu_fir_high_speed_config(int enable)
{
}

void pmu_fir_freq_config(uint32_t freq)
{
    uint32_t lock;

    lock = int_lock();
    if (freq >= 96000000) {
        pmu_vcore_req |= PMU_VCORE_FIR_FREQ_HIGH;
    } else {
        pmu_vcore_req &= ~PMU_VCORE_FIR_FREQ_HIGH;
    }
    int_unlock(lock);

    pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
}

void pmu_iir_freq_config(uint32_t freq)
{
    uint32_t lock;

    lock = int_lock();
    if (freq >= 96000000) {
        pmu_vcore_req |= PMU_VCORE_IIR_FREQ_HIGH;
    } else {
        pmu_vcore_req &= ~PMU_VCORE_IIR_FREQ_HIGH;
    }
    int_unlock(lock);

    pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
}

void pmu_iir_eq_freq_config(uint32_t freq)
{
    uint32_t lock;

    lock = int_lock();
    if (freq >= 96000000) {
        pmu_vcore_req |= PMU_VCORE_IIR_EQ_FREQ_HIGH;
    } else {
        pmu_vcore_req &= ~PMU_VCORE_IIR_EQ_FREQ_HIGH;
    }
    int_unlock(lock);

    pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
}

void pmu_rs_freq_config(uint32_t freq)
{
    uint32_t lock;

    lock = int_lock();
    if (freq >= 96000000) {
        pmu_vcore_req |= PMU_VCORE_RS_FREQ_HIGH;
    } else {
        pmu_vcore_req &= ~PMU_VCORE_RS_FREQ_HIGH;
    }
    int_unlock(lock);

    pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
}

void pmu_rs_adc_freq_config(uint32_t freq)
{
    uint32_t lock;

    lock = int_lock();
    if (freq >= 96000000) {
        pmu_vcore_req |= PMU_VCORE_RS_ADC_FREQ_HIGH;
    } else {
        pmu_vcore_req &= ~PMU_VCORE_RS_ADC_FREQ_HIGH;
    }
    int_unlock(lock);

    pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
}

void BOOT_TEXT_SRAM_LOC pmu_sys_freq_config(enum HAL_CMU_FREQ_T freq)
{
#if defined(PMU_INIT) || (!defined(FPGA) && !defined(PROGRAMMER))
    uint32_t lock;
    enum PMU_VCORE_REQ_T old_req;
    bool update = false;

    lock = int_lock();
    old_req = pmu_vcore_req;
    pmu_vcore_req &= ~(PMU_VCORE_SYS_FREQ_HIGH | PMU_VCORE_SYS_FREQ_MEDIUM | PMU_VCORE_SYS_FREQ_MEDIUM_LOW);
#if defined(MCU_HIGH_PERFORMANCE_MODE)
    if (freq > HAL_CMU_FREQ_104M) {
        if (high_perf_on) {
            // The real freq is 300M
            pmu_vcore_req |= PMU_VCORE_SYS_FREQ_HIGH;
        } else {
            pmu_vcore_req |= PMU_VCORE_SYS_FREQ_MEDIUM;
        }
    } else {
#ifdef OSC_26M_X4_AUD2BB
        if (freq >= HAL_CMU_FREQ_78M) {
            // The real freq is 104M
            pmu_vcore_req |= PMU_VCORE_SYS_FREQ_MEDIUM_LOW;
        }
#else
        if (freq == HAL_CMU_FREQ_104M) {
            pmu_vcore_req |= PMU_VCORE_SYS_FREQ_MEDIUM_LOW;
        }
#endif
    }
#else // !MCU_HIGH_PERFORMANCE_MODE
    if (freq > HAL_CMU_FREQ_104M) {
        pmu_vcore_req |= PMU_VCORE_SYS_FREQ_MEDIUM;
#ifdef OSC_26M_X4_AUD2BB
    } else if (freq >= HAL_CMU_FREQ_78M) {
        // The real freq is 104M
        pmu_vcore_req |= PMU_VCORE_SYS_FREQ_MEDIUM_LOW;
    }
#else
    } else if (freq == HAL_CMU_FREQ_104M) {
        pmu_vcore_req |= PMU_VCORE_SYS_FREQ_MEDIUM_LOW;
    }
#endif
#endif // !MCU_HIGH_PERFORMANCE_MODE
    if (old_req != pmu_vcore_req) {
        update = true;
    }
    int_unlock(lock);

    if (!update) {
        // Nothing changes
        return;
    }

    if (pmu_power_mode == PMU_POWER_MODE_NONE) {
        // PMU not init yet
        pmu_dig_init_volt();
        return;
    }

    pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
#endif
}

#if defined(SKYWALKER_IMPLEMENTATION)
void pmu_sys_update_dig_volt(void)
{
    pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
}
#endif

void pmu_high_performance_mode_enable(bool enable)
{
#if defined(MCU_HIGH_PERFORMANCE_MODE)
    if (high_perf_on == enable) {
        return;
    }
    high_perf_on = enable;

    if (!enable) {
        if (high_perf_freq_mhz > 300) {
            // Switch to 52M to avoid using PLL
            hal_cmu_sys_set_freq(HAL_CMU_FREQ_52M);
            // Restore the default div
            analog_aud_pll_set_dig_div(2);
            // Restore the sys freq
            hal_cmu_sys_set_freq(hal_sysfreq_get_hw_freq());
        }
        // Restore the default PLL freq
        analog_aud_freq_pll_config(CODEC_FREQ_24P576M, CODEC_PLL_DIV);
    }

    pmu_sys_freq_config(hal_sysfreq_get_hw_freq());

    if (enable) {
        uint32_t pll_freq;

        // Change freq first, and then change divider.
        // Otherwise there will be an instant very high freq sent to digital domain.

        if (high_perf_freq_mhz <= 300) {
            pll_freq = high_perf_freq_mhz * 1000000 * 2;
        } else {
            pll_freq = high_perf_freq_mhz * 1000000;
        }
        analog_aud_freq_pll_config(pll_freq / CODEC_PLL_DIV, CODEC_PLL_DIV);

        if (high_perf_freq_mhz > 300) {
            // Switch to 52M to avoid using PLL
            hal_cmu_sys_set_freq(HAL_CMU_FREQ_52M);
            analog_aud_pll_set_dig_div(1);
            // Restore the sys freq
            hal_cmu_sys_set_freq(hal_sysfreq_get_hw_freq());
        }
    }
#endif
}

void pmu_usb_config(enum PMU_USB_CONFIG_TYPE_T type)
{
    uint32_t lock;
    bool update;
    unsigned short ldo_on, deep_sleep_on;

    update = false;

    if (type == PMU_USB_CONFIG_TYPE_NONE) {
        usbphy_ldo_config(false);

        ldo_on = PMU_LDO_OFF;
        deep_sleep_on = PMU_DSLEEP_MODE_OFF;
    } else {
        ldo_on = PMU_LDO_ON;
        deep_sleep_on = PMU_DSLEEP_MODE_ON;
    }

    lock = int_lock();

    if (type == PMU_USB_CONFIG_TYPE_NONE) {
        pmu_vusb_req &= ~PMU_VUSB_REQ_USB;
        if (pmu_vusb_req == 0) {
            update = true;
        }
    } else {
        if (pmu_vusb_req == 0) {
            update = true;
        }
        pmu_vusb_req |= PMU_VUSB_REQ_USB;
    }

    if (update) {
        pmu_module_config(PMU_USB, PMU_MANUAL_MODE, ldo_on, PMU_LP_MODE_ON, deep_sleep_on);
    }

#ifdef USB_HIGH_SPEED
    if (type == PMU_USB_CONFIG_TYPE_NONE) {
        pmu_vcore_req &= ~PMU_VCORE_USB_HS_ENABLED;
    } else {
        pmu_vcore_req |= PMU_VCORE_USB_HS_ENABLED;
    }
#endif

    int_unlock(lock);

    if (pmu_power_mode != PMU_POWER_MODE_NONE) {
        // PMU has been inited
        pmu_dig_set_volt(0, PMU_POWER_MODE_NONE);
    }

    if (type != PMU_USB_CONFIG_TYPE_NONE) {
        usbphy_ldo_config(true);
    }
}

void pmu_lbrt_config(int enable)
{
    uint32_t lock;
    bool update;
    unsigned short ldo_on, deep_sleep_on;

    update = false;

    if (enable) {
        ldo_on = PMU_LDO_ON;
        deep_sleep_on = PMU_DSLEEP_MODE_ON;
    } else {
        ldo_on = PMU_LDO_OFF;
        deep_sleep_on = PMU_DSLEEP_MODE_OFF;
    }

    lock = int_lock();

    if (enable) {
        if (pmu_vusb_req == 0) {
            update = true;
        }
        pmu_vusb_req |= PMU_VUSB_REQ_LBRT;
    } else {
        pmu_vusb_req &= ~PMU_VUSB_REQ_LBRT;
        if (pmu_vusb_req == 0) {
            update = true;
        }
    }

    if (update) {
        pmu_module_config(PMU_USB, PMU_MANUAL_MODE, ldo_on, PMU_LP_MODE_ON, deep_sleep_on);
    }

    int_unlock(lock);
}

#if !defined(FPGA) && !defined(PROGRAMMER)
struct PMU_CHG_CTX_T {
    uint16_t pmu_chg_status;
};

struct PMU_CHG_CTX_T BOOT_BSS_LOC pmu_chg_ctx;

void BOOT_TEXT_SRAM_LOC pmu_charger_save_context(void)
{
    pmu_read(PMU_REG_CHARGER_STATUS, &pmu_chg_ctx.pmu_chg_status);
}

enum PMU_POWER_ON_CAUSE_T pmu_charger_poweron_status(void)
{
    enum PMU_POWER_ON_CAUSE_T pmu_power_on_cause = PMU_POWER_ON_CAUSE_NONE;

    if (pmu_chg_ctx.pmu_chg_status & AC_ON_DET_OUT){
        pmu_power_on_cause = PMU_POWER_ON_CAUSE_CHARGER_ACOFF;
    }else if (pmu_chg_ctx.pmu_chg_status & AC_ON){
        pmu_power_on_cause = PMU_POWER_ON_CAUSE_CHARGER_ACON;
    }
    return pmu_power_on_cause;
}

enum PMU_POWER_ON_CAUSE_T pmu_get_power_on_cause(void)
{
    enum PMU_POWER_ON_CAUSE_T pmu_power_on_cause = PMU_POWER_ON_CAUSE_NONE;

    pmu_power_on_cause = pmu_charger_poweron_status();
    if (pmu_power_on_cause == PMU_POWER_ON_CAUSE_NONE){
        pmu_power_on_cause = PMU_POWER_ON_CAUSE_POWER_KEY;
    }

    return pmu_power_on_cause;
}
#endif

void pmu_charger_init(void)
{
    unsigned short readval_cfg;
    uint32_t lock;

    lock = int_lock();
    pmu_read(PMU_REG_CHARGER_CFG, &readval_cfg);
    readval_cfg &= ~(REG_CHARGE_IN_INTR_MSK | REG_CHARGE_OUT_INTR_MSK |
        REG_AC_ON_OUT_EN | REG_AC_ON_IN_EN | REG_CHARGE_INTR_EN);
    pmu_write(PMU_REG_CHARGER_CFG ,readval_cfg);
    int_unlock(lock);

    // PMU irqs cannot be cleared by PMU soft reset
    pmu_read(PMU_REG_CHARGER_STATUS, &readval_cfg);
    pmu_write(PMU_REG_CHARGER_STATUS, readval_cfg);
    pmu_read(PMU_REG_INT_STATUS, &readval_cfg);
    pmu_write(PMU_REG_INT_CLR, readval_cfg);

    hal_sys_timer_delay(MS_TO_TICKS(1));

    lock = int_lock();
    pmu_read(PMU_REG_CHARGER_CFG, &readval_cfg);
    readval_cfg |= REG_AC_ON_OUT_EN | REG_AC_ON_IN_EN | REG_CHARGE_INTR_EN;
    readval_cfg = SET_BITFIELD(readval_cfg, REG_AC_ON_DB_VALUE, 8);
    pmu_write(PMU_REG_CHARGER_CFG ,readval_cfg);
    int_unlock(lock);

#ifdef PMU_ACON_CAUSE_POWER_UP
    pmu_read(0x102 ,&readval_cfg);
    readval_cfg |= 1;
    pmu_write(0x102,readval_cfg);
#endif
}

void pmu_charger_shutdown_config(void)
{
#ifdef PMU_ACON_CAUSE_POWER_UP
    pmu_wdt_stop();
    pmu_charger_init();
#endif
}

static void pmu_charger_irq_handler(uint16_t irq_status)
{
    enum PMU_CHARGER_STATUS_T status = PMU_CHARGER_UNKNOWN;

    PMU_DEBUG_TRACE(3,"%s REG_%02X=0x%04X", __func__, PMU_REG_CHARGER_STATUS, irq_status);

    if ((irq_status & (AC_ON_DET_IN_MASKED | AC_ON_DET_OUT_MASKED)) == 0){
        PMU_DEBUG_TRACE(1,"%s SKIP", __func__);
        return;
    } else if ((irq_status & (AC_ON_DET_IN_MASKED | AC_ON_DET_OUT_MASKED)) ==
            (AC_ON_DET_IN_MASKED | AC_ON_DET_OUT_MASKED)) {
        PMU_DEBUG_TRACE(1,"%s DITHERING", __func__);
        hal_sys_timer_delay(2);
    } else {
        PMU_DEBUG_TRACE(1,"%s NORMAL", __func__);
    }

    status = pmu_charger_get_status();

    if (charger_irq_handler) {
        charger_irq_handler(status);
    }
}

void pmu_charger_set_irq_handler(PMU_CHARGER_IRQ_HANDLER_T handler)
{
    uint32_t lock;
    uint16_t val;

    charger_irq_handler = handler;

    lock = int_lock();
    pmu_read(PMU_REG_CHARGER_CFG, &val);
    if (handler) {
        val |= REG_CHARGE_IN_INTR_MSK | REG_CHARGE_OUT_INTR_MSK;
    } else {
        val &= ~(REG_CHARGE_IN_INTR_MSK | REG_CHARGE_OUT_INTR_MSK);
    }
    pmu_write(PMU_REG_CHARGER_CFG, val);
    pmu_set_irq_unified_handler(PMU_IRQ_TYPE_CHARGER, handler ? pmu_charger_irq_handler : NULL);
    int_unlock(lock);
}

void pmu_charger_plugin_config(void)
{
#ifdef PMU_VIO_RISE
    if (IO_VOLT_ACTIVE_RISE < PMU_IO_3_1V) {
        vio_act_rise = PMU_IO_3_1V;
    }
    if (IO_VOLT_SLEEP < PMU_IO_3_2V) {
        vio_lp = PMU_IO_3_2V;
    }
    pmu_viorise_req(PMU_VIORISE_REQ_USER_CHARGER, true);
#endif
}

void pmu_charger_plugout_config(void)
{
#ifdef PMU_VIO_RISE
    vio_act_rise = IO_VOLT_ACTIVE_RISE;
    vio_lp = IO_VOLT_SLEEP;
    pmu_viorise_req(PMU_VIORISE_REQ_USER_CHARGER, false);
#endif
}

enum PMU_CHARGER_STATUS_T pmu_charger_get_status(void)
{
    unsigned short readval;
    enum PMU_CHARGER_STATUS_T status;

    pmu_read(PMU_REG_CHARGER_STATUS, &readval);
    if (readval & AC_ON)
        status = PMU_CHARGER_PLUGIN;
    else
        status = PMU_CHARGER_PLUGOUT;

    return status;
}

#ifdef RTC_ENABLE
void pmu_rtc_enable(void)
{
    uint16_t readval;
    uint32_t lock;

#ifdef SIMU
    // Set RTC counter to 1KHz
    pmu_write(PMU_REG_RTC_DIV_1HZ, 32 - 2);
#else
    // Set RTC counter to 1Hz
    pmu_write(PMU_REG_RTC_DIV_1HZ, CONFIG_SYSTICK_HZ * 2 - 2);
#endif

    lock = int_lock();
    pmu_read(PMU_REG_POWER_KEY_CFG, &readval);
    readval |= RTC_POWER_ON_EN | PU_LPO_DR | PU_LPO_REG;
    pmu_write(PMU_REG_POWER_KEY_CFG, readval);
    int_unlock(lock);
}

void pmu_rtc_disable(void)
{
    uint16_t readval;
    uint32_t lock;

    pmu_rtc_clear_alarm();

    lock = int_lock();
    pmu_read(PMU_REG_POWER_KEY_CFG, &readval);
    readval &= ~(RTC_POWER_ON_EN | PU_LPO_DR);
    pmu_write(PMU_REG_POWER_KEY_CFG, readval);
    int_unlock(lock);
}

int BOOT_TEXT_SRAM_LOC pmu_rtc_enabled(void)
{
    uint16_t readval;

    pmu_read(PMU_REG_POWER_KEY_CFG, &readval);

    return !!(readval & RTC_POWER_ON_EN);
}

void pmu_rtc_set(uint32_t seconds)
{
    uint16_t high, low;

    // Need 3 seconds to load a new value
    seconds += 3;

    high = seconds >> 16;
    low = seconds & 0xFFFF;

    pmu_write(PMU_REG_RTC_LOAD_LOW, low);
    pmu_write(PMU_REG_RTC_LOAD_HIGH, high);
}

uint32_t pmu_rtc_get(void)
{
    uint16_t high, low, high2;

    pmu_read(PMU_REG_RTC_VAL_HIGH, &high);
    pmu_read(PMU_REG_RTC_VAL_LOW, &low);
    // Handle counter wrap
    pmu_read(PMU_REG_RTC_VAL_HIGH, &high2);
    if (high != high2) {
        high = high2;
        pmu_read(PMU_REG_RTC_VAL_LOW, &low);
    }

    return (high << 16) | low;
}

void pmu_rtc_set_alarm(uint32_t seconds)
{
    uint16_t readval;
    uint16_t high, low;
    uint32_t lock;

    // Need 1 second to raise the interrupt
    if (seconds > 0) {
        seconds -= 1;
    }

    high = seconds >> 16;
    low = seconds & 0xFFFF;

    pmu_write(PMU_REG_INT_CLR, RTC_INT1_MSKED);

    pmu_write(PMU_REG_RTC_MATCH1_LOW, low);
    pmu_write(PMU_REG_RTC_MATCH1_HIGH, high);

    lock = int_lock();
    pmu_read(PMU_REG_INT_EN, &readval);
    readval |= RTC_INT_EN_1;
    pmu_write(PMU_REG_INT_EN, readval);
    int_unlock(lock);
}

uint32_t BOOT_TEXT_SRAM_LOC pmu_rtc_get_alarm(void)
{
    uint16_t high, low;

    pmu_read(PMU_REG_RTC_MATCH1_LOW, &low);
    pmu_read(PMU_REG_RTC_MATCH1_HIGH, &high);

    // Compensate the alarm offset
    return (uint32_t)((high << 16) | low) + 1;
}

void pmu_rtc_clear_alarm(void)
{
    uint16_t readval;
    uint32_t lock;

    lock = int_lock();
    pmu_read(PMU_REG_INT_EN, &readval);
    readval &= ~RTC_INT_EN_1;
    pmu_write(PMU_REG_INT_EN, readval);
    int_unlock(lock);

    pmu_write(PMU_REG_INT_CLR, RTC_INT1_MSKED);
}

int BOOT_TEXT_SRAM_LOC pmu_rtc_alarm_status_set(void)
{
    uint16_t readval;

    pmu_read(PMU_REG_INT_EN, &readval);

    return !!(readval & RTC_INT_EN_1);
}

int pmu_rtc_alarm_alerted()
{
    uint16_t readval;

    pmu_read(PMU_REG_INT_STATUS, &readval);

    return !!(readval & RTC_INT_1);
}

static void pmu_rtc_irq_handler(uint16_t irq_status)
{
    uint32_t seconds;

    if (irq_status & RTC_INT1_MSKED) {
        pmu_rtc_clear_alarm();

        if (rtc_irq_handler) {
            seconds = pmu_rtc_get();
            rtc_irq_handler(seconds);
        }
    }
}

void pmu_rtc_set_irq_handler(PMU_RTC_IRQ_HANDLER_T handler)
{
    uint16_t readval;
    uint32_t lock;

    rtc_irq_handler = handler;

    lock = int_lock();
    pmu_read(PMU_REG_INT_MASK, &readval);
    if (handler) {
        readval |= RTC_INT1_MSK;
    } else {
        readval &= ~RTC_INT1_MSK;
    }
    pmu_write(PMU_REG_INT_MASK, readval);
    pmu_set_irq_unified_handler(PMU_IRQ_TYPE_RTC, handler ? pmu_rtc_irq_handler : NULL);
    int_unlock(lock);
}
#endif

int pmu_pwrkey_set_irq(enum HAL_PWRKEY_IRQ_T type)
{
    uint16_t val;
    uint16_t int_en;

    if (type == HAL_PWRKEY_IRQ_NONE) {
        // Clear existing IRQs
        pmu_write(PMU_REG_CHARGER_STATUS, (POWER_ON_PRESS | POWER_ON_RELEASE));
        // Disable future IRQs
        int_en = 0;
    } else if (type == HAL_PWRKEY_IRQ_FALLING_EDGE) {
        int_en = POWERON_PRESS_EN;
    } else if (type == HAL_PWRKEY_IRQ_RISING_EDGE) {
        int_en = POWERON_RELEASE_EN;
    } else if (type == HAL_PWRKEY_IRQ_BOTH_EDGE) {
        int_en = POWERON_PRESS_EN | POWERON_RELEASE_EN;
    } else {
        return 1;
    }

    pmu_read(PMU_REG_SLEEP_CFG, &val);
    val = (val & ~(POWERON_PRESS_EN | POWERON_RELEASE_EN)) | int_en;
    pmu_write(PMU_REG_SLEEP_CFG, val);

    return 0;
}
int hal_pwrkey_set_irq(enum HAL_PWRKEY_IRQ_T type) __attribute__((alias("pmu_pwrkey_set_irq")));

bool pmu_pwrkey_pressed(void)
{
    uint16_t val;

    pmu_read(PMU_REG_CHARGER_STATUS, &val);

    return !!(val & POWER_ON);
}
bool hal_pwrkey_pressed(void) __attribute__((alias("pmu_pwrkey_pressed")));

enum HAL_PWRKEY_IRQ_T pmu_pwrkey_irq_value_to_state(uint16_t irq_status)
{
    enum HAL_PWRKEY_IRQ_T state = HAL_PWRKEY_IRQ_NONE;

    if (irq_status & POWER_ON_PRESS) {
        state |= HAL_PWRKEY_IRQ_FALLING_EDGE;
    }

    if (irq_status & POWER_ON_RELEASE) {
        state |= HAL_PWRKEY_IRQ_RISING_EDGE;
    }

    return state;
}

static void pmu_general_irq_handler(void)
{
    uint32_t lock;
    uint16_t val;
    bool pwrkey, charger, gpadc, rtc;

    pwrkey = false;
    charger = false;
    gpadc = false;
    rtc = false;

    lock = int_lock();
    pmu_read(PMU_REG_CHARGER_STATUS, &val);
    if (val & (POWER_ON_PRESS | POWER_ON_RELEASE)) {
        pwrkey = true;
    }
    if (val & (AC_ON_DET_OUT_MASKED | AC_ON_DET_IN_MASKED)) {
        charger = true;
    }
    if (pwrkey || charger) {
        pmu_write(PMU_REG_CHARGER_STATUS, val);
    }
    int_unlock(lock);

    if (pwrkey) {
        if (pmu_irq_hdlrs[PMU_IRQ_TYPE_PWRKEY]) {
            pmu_irq_hdlrs[PMU_IRQ_TYPE_PWRKEY](val);
        }
    }
    if (charger) {
        if (pmu_irq_hdlrs[PMU_IRQ_TYPE_CHARGER]) {
            pmu_irq_hdlrs[PMU_IRQ_TYPE_CHARGER](val);
        }
    }

    lock = int_lock();
    pmu_read(PMU_REG_INT_MSKED_STATUS, &val);
    if (val & (KEY_ERR1_INTR_MSKED | KEY_ERR0_INTR_MSKED | KEY_PRESS_INTR_MSKED | KEY_RELEASE_INTR_MSKED |
            SAMPLE_DONE_INTR_MSKED | CHAN_DATA_INTR_MSKED_MASK)) {
        gpadc = true;
    }
    if (val & (RTC_INT1_MSKED | RTC_INT0_MSKED)) {
        rtc = true;
    }
    if (gpadc || rtc) {
        pmu_write(PMU_REG_INT_CLR, val);
    }
    int_unlock(lock);

    if (gpadc) {
        if (pmu_irq_hdlrs[PMU_IRQ_TYPE_GPADC]) {
            pmu_irq_hdlrs[PMU_IRQ_TYPE_GPADC](val);
        }
    }
    if (rtc) {
        if (pmu_irq_hdlrs[PMU_IRQ_TYPE_RTC]) {
            pmu_irq_hdlrs[PMU_IRQ_TYPE_RTC](val);
        }
    }
}

int pmu_set_irq_unified_handler(enum PMU_IRQ_TYPE_T type, PMU_IRQ_UNIFIED_HANDLER_T hdlr)
{
    bool update;
    uint32_t lock;
    int i;

    if (type >= PMU_IRQ_TYPE_QTY) {
        return 1;
    }

    update = false;

    lock = int_lock();

    for (i = 0; i < PMU_IRQ_TYPE_QTY; i++) {
        if (pmu_irq_hdlrs[i]) {
            break;
        }
    }

    pmu_irq_hdlrs[type] = hdlr;

    if (hdlr) {
        update = (i >= PMU_IRQ_TYPE_QTY);
    } else {
        if (i == type) {
            for (; i < PMU_IRQ_TYPE_QTY; i++) {
                if (pmu_irq_hdlrs[i]) {
                    break;
                }
            }
            update = (i >= PMU_IRQ_TYPE_QTY);
        }
    }

    if (update) {
        if (hdlr) {
            NVIC_SetVector(PMU_IRQn, (uint32_t)pmu_general_irq_handler);
            NVIC_SetPriority(PMU_IRQn, IRQ_PRIORITY_NORMAL);
            NVIC_ClearPendingIRQ(PMU_IRQn);
            NVIC_EnableIRQ(PMU_IRQn);
        } else {
            NVIC_DisableIRQ(PMU_IRQn);
        }
    }

    int_unlock(lock);

    return 0;
}

void pmu_viorise_req(enum PMU_VIORISE_REQ_USER_T user, bool rise)
{
#ifdef PMU_VIO_RISE
    uint32_t lock;

    lock = int_lock();
    if (rise) {
        if (vio_risereq_map == 0) {
            pmu_module_ramp_volt(PMU_IO,vio_lp,vio_act_rise);
        }
        vio_risereq_map |= (1 << user);
    } else {
        vio_risereq_map &= ~(1 << user);
        if (vio_risereq_map == 0) {
            pmu_module_ramp_volt(PMU_IO,vio_lp,vio_act_normal);
        }
    }
    int_unlock(lock);
#endif
}

int pmu_debug_config_ana(uint16_t volt)
{
    return 0;
}

int pmu_debug_config_codec(uint16_t volt)
{
#ifdef ANC_PROD_TEST
    if (volt == 1600 || volt == 1700 || volt == 1800 || volt == 1900 || volt == 1950) {
        vhppa_mv = vcodec_mv = volt;
        vcodec_off = true;
    } else {
        vcodec_off = false;
        return 1;
    }
#endif
    return 0;
}

int pmu_debug_config_vcrystal(bool on)
{
    return 0;
}

int pmu_debug_config_audio_output(bool diff)
{
    return 0;
}

void pmu_debug_reliability_test(int stage)
{
    uint16_t volt;

    if (stage == 0) {
        volt = PMU_DCDC_ANA_1_2V;
    } else {
        volt = PMU_DCDC_ANA_1_3V;
    }
    pmu_dcdc_ana_set_volt(volt, ana_lp_dcdc);
}

void pmu_led_set_hiz(enum HAL_GPIO_PIN_T pin)
{
    uint16_t val1, val2;
    uint32_t lock;

    if (pin == HAL_GPIO_PIN_LED1 || pin == HAL_GPIO_PIN_LED2) {
        lock = int_lock();
        pmu_read(PMU_REG_MIC_LDO_EN, &val1);
        pmu_read(PMU_REG_LED_CFG, &val2);
        if (pin == HAL_GPIO_PIN_LED1) {
            val1 &= ~REG_LED_IO1_RX_EN;
            val2 = (val2 | REG_LED_IO1_PU | REG_LED_IO1_OENB) & ~(REG_LED_IO1_PUEN | REG_LED_IO1_PDEN);
        } else {
            val1 &= ~REG_LED_IO2_RX_EN;
            val2 = (val2 | REG_LED_IO2_PU | REG_LED_IO2_OENB) & ~(REG_LED_IO2_PUEN | REG_LED_IO2_PDEN);
        }
        pmu_write(PMU_REG_MIC_LDO_EN, val1);
        pmu_write(PMU_REG_LED_CFG, val2);
        int_unlock(lock);
    }
}

void pmu_led_set_direction(enum HAL_GPIO_PIN_T pin, enum HAL_GPIO_DIR_T dir)
{
    uint16_t val;
    uint16_t val_rx;
    uint32_t lock;

    if (pin == HAL_GPIO_PIN_LED1 || pin == HAL_GPIO_PIN_LED2) {
        lock = int_lock();
        pmu_read(PMU_REG_LED_CFG, &val);
        pmu_read(PMU_REG_MIC_LDO_EN, &val_rx);
        if (pin == HAL_GPIO_PIN_LED1) {
            val |= REG_LED_IO1_PU;
            if (dir == HAL_GPIO_DIR_IN) {
                val |= REG_LED_IO1_OENB;
                val_rx |= REG_LED_IO1_RX_EN;
            } else {
                val &= ~REG_LED_IO1_OENB;
                val_rx &= ~REG_LED_IO1_RX_EN;
            }
        } else {
            val |= REG_LED_IO2_PU;
            if (dir == HAL_GPIO_DIR_IN) {
                val |= REG_LED_IO2_OENB;
                val_rx |= REG_LED_IO2_RX_EN;
            } else {
                val &= ~REG_LED_IO2_OENB;
                val_rx &= ~REG_LED_IO2_RX_EN;
            }
        }
        pmu_write(PMU_REG_LED_CFG, val);
        pmu_write(PMU_REG_MIC_LDO_EN, val_rx);;
        int_unlock(lock);
    }
}

enum HAL_GPIO_DIR_T pmu_led_get_direction(enum HAL_GPIO_PIN_T pin)
{
    uint16_t val;

    if (pin == HAL_GPIO_PIN_LED1 || pin == HAL_GPIO_PIN_LED2) {
        pmu_read(PMU_REG_LED_CFG, &val);
        if (pin == HAL_GPIO_PIN_LED1) {
            return (val & REG_LED_IO1_OENB) ? HAL_GPIO_DIR_IN : HAL_GPIO_DIR_OUT;
        } else {
            return (val & REG_LED_IO2_OENB) ? HAL_GPIO_DIR_IN : HAL_GPIO_DIR_OUT;
        }
    } else {
        return HAL_GPIO_DIR_IN;
    }
}

void pmu_led_set_voltage_domains(enum HAL_IOMUX_PIN_T pin, enum HAL_IOMUX_PIN_VOLTAGE_DOMAINS_T volt)
{
    enum PMU_LED_VOLT_T {
        PMU_LED_VOLT_VBAT,
        PMU_LED_VOLT_VMEM,
        PMU_LED_VOLT_VIO,
    };
    enum PMU_LED_VOLT_T sel;
    uint16_t val;
    uint32_t lock;

    if (pin == HAL_IOMUX_PIN_LED1 || pin == HAL_IOMUX_PIN_LED2) {
        if (volt == HAL_IOMUX_PIN_VOLTAGE_VIO) {
            sel = PMU_LED_VOLT_VIO;
        } else if (volt == HAL_IOMUX_PIN_VOLTAGE_MEM) {
            sel = PMU_LED_VOLT_VMEM;
        } else {
            sel = PMU_LED_VOLT_VBAT;
        }

        lock = int_lock();
        pmu_read(PMU_REG_LED_CFG, &val);
        if (pin == HAL_IOMUX_PIN_LED1) {
            val = SET_BITFIELD(val, REG_LED_IO1_SEL, sel);
        } else {
            val = SET_BITFIELD(val, REG_LED_IO2_SEL, sel);
        }
        pmu_write(PMU_REG_LED_CFG, val);
        int_unlock(lock);
    }
}

void pmu_led_set_pull_select(enum HAL_IOMUX_PIN_T pin, enum HAL_IOMUX_PIN_PULL_SELECT_T pull_sel)
{
    uint16_t val;
    uint32_t lock;

    if (pin == HAL_IOMUX_PIN_LED1 || pin == HAL_IOMUX_PIN_LED2) {
        lock = int_lock();
        pmu_read(PMU_REG_LED_CFG, &val);
        if (pin == HAL_IOMUX_PIN_LED1) {
            val &= ~(REG_LED_IO1_PDEN | REG_LED_IO1_PUEN);
            if (pull_sel == HAL_IOMUX_PIN_PULLUP_ENABLE) {
                val |= REG_LED_IO1_PUEN;
            } else if (pull_sel == HAL_IOMUX_PIN_PULLDOWN_ENABLE) {
                val |= REG_LED_IO1_PDEN;
            }
        } else {
            val &= ~(REG_LED_IO2_PDEN | REG_LED_IO2_PUEN);
            if (pull_sel == HAL_IOMUX_PIN_PULLUP_ENABLE) {
                val |= REG_LED_IO2_PUEN;
            } else if (pull_sel == HAL_IOMUX_PIN_PULLDOWN_ENABLE) {
                val |= REG_LED_IO2_PDEN;
            }
        }
        pmu_write(PMU_REG_LED_CFG, val);
        int_unlock(lock);
    }
}

void pmu_led_set_value(enum HAL_GPIO_PIN_T pin, int data)
{
    uint16_t val;
    uint32_t lock;
#ifdef PMU_LED_VIA_PWM
    uint16_t br_val;
#endif
    if (pin == HAL_GPIO_PIN_LED1 || pin == HAL_GPIO_PIN_LED2) {
        lock = int_lock();
#ifdef PMU_LED_VIA_PWM
        pmu_read(PMU_REG_PWM_BR_EN, &br_val);
#endif
        pmu_read(PMU_REG_PWM_EN, &val);
        if (pin == HAL_GPIO_PIN_LED1) {
#ifdef PMU_LED_VIA_PWM
            pmu_write(PMU_REG_PWM2_TOGGLE, 0xFFFF);
            br_val &= ~REG_PWM2_BR_EN;
            val |= PWM_SELECT_EN(1 << 0);
            if (data) {
                val &= ~PWM_SELECT_INV(1 << 0);
            } else {
                val |= PWM_SELECT_INV(1 << 0);
            }
#else
            if (data) {
                val |= REG_LED0_OUT;
            } else {
                val &= ~REG_LED0_OUT;
            }
#endif
        } else {
#ifdef PMU_LED_VIA_PWM
            pmu_write(PMU_REG_PWM3_TOGGLE, 0xFFFF);
            br_val &= ~REG_PWM3_BR_EN;
            val |= PWM_SELECT_EN(1 << 1);
            if (data) {
                val &= ~PWM_SELECT_INV(1 << 1);
            } else {
                val |= PWM_SELECT_INV(1 << 1);
            }
#else
            if (data) {
                val |= REG_LED1_OUT;
            } else {
                val &= ~REG_LED1_OUT;
            }
#endif
        }
#ifdef PMU_LED_VIA_PWM
        pmu_write(PMU_REG_PWM_BR_EN, br_val);
#endif
        pmu_write(PMU_REG_PWM_EN, val);
        int_unlock(lock);
    }
}

int pmu_led_get_value(enum HAL_GPIO_PIN_T pin)
{
    uint16_t val;
    int data = 0;

    if (pin == HAL_GPIO_PIN_LED1 || pin == HAL_GPIO_PIN_LED2) {
        pmu_read(PMU_REG_POWER_OFF, &val);
        if (pin == HAL_GPIO_PIN_LED1) {
            data = REG_LED_IO1_DATA_IN;
        } else {
            data = REG_LED_IO2_DATA_IN;
        }
        data &= val;
    }

    return !!data;
}

void pmu_led_breathing_enable(enum HAL_IOMUX_PIN_T pin, const struct PMU_LED_BR_CFG_T *cfg)
{
    uint32_t st1;
    uint32_t st2;
    uint32_t subcnt_data;
    uint8_t tg;
    uint16_t val;
    uint32_t lock;
    if (pin == HAL_IOMUX_PIN_LED1 || pin == HAL_IOMUX_PIN_LED2) {
        st1 = MS_TO_TICKS(cfg->off_time_ms);
        if (st1 > 0xFFFF) {
            st1 = 0xFFFF;
        }
        st2 = MS_TO_TICKS(cfg->on_time_ms);
        if (st2 > 0xFFFF) {
            st2 = 0xFFFF;
        }
        subcnt_data = MS_TO_TICKS(cfg->fade_time_ms);
        subcnt_data = integer_sqrt_nearest(subcnt_data);
        if (subcnt_data > (SUBCNT_DATA2_MASK >> SUBCNT_DATA2_SHIFT)) {
            subcnt_data = (SUBCNT_DATA2_MASK >> SUBCNT_DATA2_SHIFT);
        }
        // TODO: HW bug
        if (subcnt_data > 0xFE) {
            subcnt_data = 0xFE;
        }
        tg = 1;

#ifdef PMU1800_LED_PWM_VIA_BREATH
    if (pmu_type != PMU_CHIP_TYPE_1800) {
        TRACE(0,"use breath to realize the PWM mode is only suitable for PMU1800!");
    } else {
        st1 = cfg->off_time_ms;
        st2 = cfg->on_time_ms;
        if (st1 > 0xFFFF) {
            st1 = 0xFFFF;
        }
        if (st2 > 0xFFFF) {
            st2 = 0xFFFF;
        }

        subcnt_data = 0;
        tg = 0;
    }
#endif

        pmu_led_set_direction((enum HAL_GPIO_PIN_T)pin, HAL_GPIO_DIR_OUT);
        pmu_led_set_pull_select(pin, HAL_IOMUX_PIN_NOPULL);
        pmu_led_set_voltage_domains(pin, HAL_IOMUX_PIN_VOLTAGE_VBAT);

        lock = int_lock();
        if (pin == HAL_IOMUX_PIN_LED1) {
            pmu_write(PMU_REG_PWM2_TOGGLE, st2);
            pmu_write(PMU_REG_PWM2_ST1, st1);
            pmu_read(PMU_REG_SUBCNT_DATA, &val);
            val = SET_BITFIELD(val, SUBCNT_DATA2, subcnt_data);
            pmu_write(PMU_REG_SUBCNT_DATA, val);
            pmu_read(PMU_REG_PWM_BR_EN, &val);
            val = (val & ~TG_SUBCNT_D2_ST_MASK) | TG_SUBCNT_D2_ST(tg) | REG_PWM2_BR_EN;
            pmu_write(PMU_REG_PWM_BR_EN, val);
            pmu_read(PMU_REG_PWM_EN, &val);
            val = (val & ~REG_CLK_PWM_DIV_MASK) | REG_CLK_PWM_DIV(0) | PWM_SELECT_EN(1 << 0);
            pmu_write(PMU_REG_PWM_EN, val);
        } else {
            pmu_write(PMU_REG_PWM3_TOGGLE, st2);
            pmu_write(PMU_REG_PWM3_ST1, st1);
            pmu_read(PMU_REG_SUBCNT_DATA, &val);
            val = SET_BITFIELD(val, SUBCNT_DATA3, subcnt_data);
            pmu_write(PMU_REG_SUBCNT_DATA, val);
            pmu_read(PMU_REG_PWM_BR_EN, &val);
            val = (val & ~TG_SUBCNT_D3_ST_MASK) | TG_SUBCNT_D3_ST(tg) | REG_PWM3_BR_EN;
            pmu_write(PMU_REG_PWM_BR_EN, val);
            pmu_read(PMU_REG_PWM_EN, &val);
            val = (val & ~REG_CLK_PWM_DIV_MASK) | REG_CLK_PWM_DIV(0) | PWM_SELECT_EN(1 << 1);
            pmu_write(PMU_REG_PWM_EN, val);
        }
        int_unlock(lock);
    }
}

void pmu_led_breathing_disable(enum HAL_IOMUX_PIN_T pin)
{
    uint16_t val;
    uint32_t lock;
    if (pin == HAL_IOMUX_PIN_LED1 || pin == HAL_IOMUX_PIN_LED2) {
        lock = int_lock();
        if (pin == HAL_IOMUX_PIN_LED1) {
            pmu_read(PMU_REG_PWM_BR_EN, &val);
            val &= ~REG_PWM2_BR_EN;
            pmu_write(PMU_REG_PWM_BR_EN, val);
            pmu_read(PMU_REG_PWM_EN, &val);
            val &= ~PWM_SELECT_EN(1 << 0);
            pmu_write(PMU_REG_PWM_EN, val);
        } else {
            pmu_read(PMU_REG_PWM_BR_EN, &val);
            val &= ~REG_PWM3_BR_EN;
            pmu_write(PMU_REG_PWM_BR_EN, val);
            pmu_read(PMU_REG_PWM_EN, &val);
            val &= ~PWM_SELECT_EN(1 << 1);
            pmu_write(PMU_REG_PWM_EN, val);
        }
        int_unlock(lock);
        pmu_led_set_direction((enum HAL_GPIO_PIN_T)pin, HAL_GPIO_DIR_IN);
        pmu_led_set_pull_select(pin, HAL_IOMUX_PIN_PULLUP_ENABLE);
    }
}

__STATIC_FORCEINLINE void pmu_wdt_load_value(void)
{
    uint32_t lock;
    uint16_t val;

    if (pmu_type == PMU_CHIP_TYPE_1803) {
        // For 1803 chips
        lock = int_lock();
        pmu_read(PMU_REG_GP_CFG, &val);
        pmu_write(PMU_REG_GP_CFG, val);
        int_unlock(lock);
    }
}

#ifdef __WATCHER_DOG_RESET__
struct PMU_WDT_CTX_T {
    bool enabled;
    uint16_t wdt_timer;
    uint16_t wdt_cfg;
};

static struct PMU_WDT_CTX_T BOOT_BSS_LOC wdt_ctx;

void BOOT_TEXT_SRAM_LOC pmu_wdt_save_context(void)
{
    uint16_t wdt_cfg = 0, wdt_timer = 0;
    pmu_read(PMU_REG_WDT_CFG, &wdt_cfg);
    if (wdt_cfg & (REG_WDT_RESET_EN | REG_WDT_EN)){
        wdt_ctx.enabled = true;
        wdt_ctx.wdt_cfg = wdt_cfg;
        pmu_read(PMU_REG_WDT_TIMER, &wdt_timer);
        wdt_ctx.wdt_timer = wdt_timer;
    }
}

void BOOT_TEXT_SRAM_LOC pmu_wdt_restore_context(void)
{
    if (wdt_ctx.enabled) {
        pmu_write(PMU_REG_WDT_TIMER, wdt_ctx.wdt_timer);
        pmu_write(PMU_REG_WDT_CFG, wdt_ctx.wdt_cfg);
        pmu_wdt_load_value();
    }
}
#endif

void pmu_wdt_set_irq_handler(PMU_WDT_IRQ_HANDLER_T handler)
{
}

int pmu_wdt_config(uint32_t irq_ms, uint32_t reset_ms)
{
    // No wdt irq on best2300
    if (irq_ms + reset_ms > 0xFFFF) {
        return 1;
    }
    wdt_timer = irq_ms + reset_ms;

    pmu_write(PMU_REG_WDT_TIMER, wdt_timer);
    pmu_wdt_load_value();

    return 0;
}

void pmu_wdt_start(void)
{
    uint16_t val;

    if (wdt_timer == 0) {
        return;
    }

    pmu_read(PMU_REG_WDT_CFG, &val);
    val |= (REG_WDT_RESET_EN | REG_WDT_EN);
    pmu_write(PMU_REG_WDT_CFG, val);
}

#ifndef __WATCHER_DOG_RESET__
BOOT_TEXT_SRAM_LOC
#endif
void pmu_wdt_stop(void)
{
    uint16_t val;

    pmu_read(PMU_REG_WDT_CFG, &val);
    val &= ~(REG_WDT_RESET_EN | REG_WDT_EN);
    pmu_write(PMU_REG_WDT_CFG, val);
}

void pmu_wdt_feed(void)
{
    if (wdt_timer == 0) {
        return;
    }

    pmu_write(PMU_REG_WDT_TIMER, wdt_timer);
    pmu_wdt_load_value();
}

void pmu_ntc_capture_enable(void)
{
}

void pmu_ntc_capture_disable(void)
{
}

#endif // !NO_EXT_PMU

void pmu_1803_uart_enable(void)
{
    uint16_t val = 0;

    if(pmu_type == PMU_CHIP_TYPE_1803) {
        pmu_read(PMU1803_REG_PMIC_UART_CFG, &val);
        val &= ~(REG_GPIO_I_SEL | REG_UART_LED2_SEL | REG_PMIC_UART_DR2);
        val |= (REG_PMIC_UART_DR1| REG_PMIC_UART_OENB_SEL);
        pmu_write(PMU1803_REG_PMIC_UART_CFG, val);
    }
}
void pmu_1802_uart_enable(void)
{
    uint16_t val;

    if((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) {
        pmu_read(PMU_REG_RESERVED_PMU, &val);
        val &= ~(1 << 12);
        pmu_write(PMU_REG_RESERVED_PMU, val);
    }
}

int pmu_get_gpadc_data_bits(void)
{
    return ((pmu_type == PMU_CHIP_TYPE_1802) || (pmu_type == PMU_CHIP_TYPE_1807)) ? 12 : 10;
}

void pmu_reset_charger(void)
{
    uint16_t val;
    if(pmu_type == PMU_CHIP_TYPE_1802) {
        pmu_read(PMU_REG_RESERVED_PMU, &val);
        val &= ~REG_ENABLE_CHARGER;
        pmu_write(PMU_REG_RESERVED_PMU, val);

        hal_sys_timer_delay(MS_TO_TICKS(1));

        pmu_read(PMU_REG_RESERVED_PMU, &val);
        val |= REG_ENABLE_CHARGER;
        pmu_write(PMU_REG_RESERVED_PMU, val);
    }
}
