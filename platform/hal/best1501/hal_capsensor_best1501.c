
#ifdef CHIP_SUBSYS_SENS

#include "cmsis.h"

#include "hal_cmu.h"
#include "hal_timer.h"
#include "hal_capsensor_best1501.h"
#include "reg_capsensor_best1501.h"

#define CAPSENSOR_BASE_ADDR		0x50010000

struct CAPSENSOR_T* capsensor_base = (struct CAPSENSOR_T*)CAPSENSOR_BASE_ADDR;

void hal_capsensor_setup(void)
{
    capsensor_base->REG_00 = SET_BITFIELD(capsensor_base->REG_00, CAP_SENS_REG_STG_NUM, 3);
    capsensor_base->REG_00 = SET_BITFIELD(capsensor_base->REG_00, CAP_SENS_REG_INTVL1_CONV_NUM, 1);
    capsensor_base->REG_00 = SET_BITFIELD(capsensor_base->REG_00, CAP_SENS_REG_INTVL1, 0x20);
    capsensor_base->REG_00 = SET_BITFIELD(capsensor_base->REG_00, CAP_SENS_REG_INTVL2, 0x1900);//5Hz 0x140);//100Hz  0xA0); // 200Hz 0xC80);//10Hz

    capsensor_base->REG_04 = SET_BITFIELD(capsensor_base->REG_04, CAP_SENS_REG_CFG_STG3, 0x0D);
    capsensor_base->REG_04 = SET_BITFIELD(capsensor_base->REG_04, CAP_SENS_REG_CFG_STG2, 0x0C);
    capsensor_base->REG_04 = SET_BITFIELD(capsensor_base->REG_04, CAP_SENS_REG_CFG_STG1, 0x0B);
    capsensor_base->REG_04 = SET_BITFIELD(capsensor_base->REG_04, CAP_SENS_REG_CFG_STG0, 0x0A);

    capsensor_base->REG_08 = SET_BITFIELD(capsensor_base->REG_08, CAP_SENS_REG_CFG_STG7, 0x17);
    capsensor_base->REG_08 = SET_BITFIELD(capsensor_base->REG_08, CAP_SENS_REG_CFG_STG6, 0x16);
    capsensor_base->REG_08 = SET_BITFIELD(capsensor_base->REG_08, CAP_SENS_REG_CFG_STG5, 0x15);
    capsensor_base->REG_08 = SET_BITFIELD(capsensor_base->REG_08, CAP_SENS_REG_CFG_STG4, 0x14);

    capsensor_base->REG_1C = SET_BITFIELD(capsensor_base->REG_1C, CAP_SENS_REG_TCH_ACTIVE_NOISE_NUM, 0x3);
    capsensor_base->REG_1C = SET_BITFIELD(capsensor_base->REG_1C, CAP_SENS_REG_TCH_ACTIVE_CAP_NUM,   0x3);

    capsensor_base->REG_20 = SET_BITFIELD(capsensor_base->REG_20, CAP_SENS_REG_CAP_THR_CHAN1, 0x550);
    capsensor_base->REG_20 = SET_BITFIELD(capsensor_base->REG_20, CAP_SENS_REG_CAP_THR_CHAN0, 0x550);

    capsensor_base->REG_24 = SET_BITFIELD(capsensor_base->REG_24, CAP_SENS_REG_CAP_THR_CHAN3, 0x400);
    capsensor_base->REG_24 = SET_BITFIELD(capsensor_base->REG_24, CAP_SENS_REG_CAP_THR_CHAN2, 0x3B0);

    capsensor_base->REG_30 = SET_BITFIELD(capsensor_base->REG_30, CAP_SENS_REG_CAP_THR_DIFF1, 0x1000);
    capsensor_base->REG_30 = SET_BITFIELD(capsensor_base->REG_30, CAP_SENS_REG_CAP_THR_DIFF0, 0x2000);

    capsensor_base->REG_48 = SET_BITFIELD(capsensor_base->REG_48, CAP_SENS_REG_CHANN_SEL_DIFF3, 0);
    capsensor_base->REG_48 = SET_BITFIELD(capsensor_base->REG_48, CAP_SENS_REG_CHANN_SEL_DIFF2, 0);
    capsensor_base->REG_48 = SET_BITFIELD(capsensor_base->REG_48, CAP_SENS_REG_CHANN_SEL_DIFF1, 0x1A);
    capsensor_base->REG_48 = SET_BITFIELD(capsensor_base->REG_48, CAP_SENS_REG_CHANN_SEL_DIFF0, 0x8);

    capsensor_base->REG_4C = SET_BITFIELD(capsensor_base->REG_4C, CAP_SENS_REG_CHANN_SEL_DBL_CLICK3, 0x3);
    capsensor_base->REG_4C = SET_BITFIELD(capsensor_base->REG_4C, CAP_SENS_REG_CHANN_SEL_DBL_CLICK2, 0x2);
    capsensor_base->REG_4C = SET_BITFIELD(capsensor_base->REG_4C, CAP_SENS_REG_CHANN_SEL_DBL_CLICK1, 0x1);
    capsensor_base->REG_4C = SET_BITFIELD(capsensor_base->REG_4C, CAP_SENS_REG_CHANN_SEL_DBL_CLICK0, 0);

    capsensor_base->REG_50 = SET_BITFIELD(capsensor_base->REG_50, CAP_SENS_REG_CHANN_DBL_CLICK_INTVL_MIN, 0x8);
    capsensor_base->REG_50 = SET_BITFIELD(capsensor_base->REG_50, CAP_SENS_REG_CHANN_DBL_CLICK_INTVL_MAX, 0x2000);

    capsensor_base->REG_58 = SET_BITFIELD(capsensor_base->REG_58, CAP_SENS_REG_CONV_EN_INITIAL_DLY, 0x10);

    capsensor_base->REG_5C = SET_BITFIELD(capsensor_base->REG_5C, CAP_SENS_REG_CONV_WAIT, 0xF);
    capsensor_base->REG_5C = SET_BITFIELD(capsensor_base->REG_5C, CAP_SENS_REG_CDC_CORE_RST_CNT, 0x5);
    capsensor_base->REG_5C = SET_BITFIELD(capsensor_base->REG_5C, CAP_SENS_REG_CDC_CORE_PWUP_WAIT, 0x5);
    capsensor_base->REG_5C = SET_BITFIELD(capsensor_base->REG_5C, CAP_SENS_REG_VREF_PRECHG_CNT, 0x80);

    capsensor_base->REG_60 = SET_BITFIELD(capsensor_base->REG_60, CAP_SENS_REG_BIAS_ISEL_0, 4);
    capsensor_base->REG_60 = SET_BITFIELD(capsensor_base->REG_60, CAP_SENS_REG_BIAS_ISEL_1, 4);
    capsensor_base->REG_60 = SET_BITFIELD(capsensor_base->REG_60, CAP_SENS_REG_LDO_VSEL,    4);
    capsensor_base->REG_60 = SET_BITFIELD(capsensor_base->REG_60, CAP_SENS_REG_CREF_SEL,    3);

    capsensor_base->REG_68 |= CAP_SENS_REG_SOFT_MODE;
    capsensor_base->REG_68 &= ~CAP_SENS_REG_CAP_SENSOR_EN;
    hal_sys_timer_delay(MS_TO_TICKS(10));
    capsensor_base->REG_68 |= CAP_SENS_REG_CAP_SENSOR_EN;
    hal_sys_timer_delay(MS_TO_TICKS(10));
    capsensor_base->REG_68 |= CAP_SENS_REG_DMA_EN;

    capsensor_base->REG_80 &= ~CAP_SENS_REG_SAR_TEST_LOCK;
    capsensor_base->REG_80 |= CAP_SENS_REG_SAR_TEST_LOCK;
}

void hal_capsensor_irq_enable(void)
{
    capsensor_base->REG_74 |= (1 << 24);
}

void hal_capsensor_irq_disable(void)
{
    capsensor_base->REG_78 |= (1 << 24);
}


#endif // CHIP_SUBSYS_SENS
