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
#include <stdio.h>
#include "cmsis_os.h"
#include "hal_uart.h"
#include "hal_timer.h"
#include "audioflinger.h"
#include "lockcqueue.h"
#include "hal_trace.h"
#include "hal_cmu.h"
#include "analog.h"
#include "bt_drv.h"
#include "app_audio.h"
#include "bluetooth.h"
#include "nvrecord.h"
#include "nvrecord_env.h"
#include "nvrecord_dev.h"
#include "hal_location.h"
#include "a2dp_api.h"
#if defined(NEW_NV_RECORD_ENABLED)
#include "nvrecord_bt.h"
#endif

#include "a2dp_api.h"
#include "avrcp_api.h"
#include "besbt.h"

#include "cqueue.h"
#include "btapp.h"
#include "app_bt.h"
#include "apps.h"
#include "resources.h"
#include "app_bt_media_manager.h"
#include "tgt_hardware.h"
#include "bt_drv_interface.h"
#include "hci_api.h"

#include "a2dp_codec_lhdc.h"
#include "avdtp_api.h"

#if defined(A2DP_LHDC_ON)
btif_avdtp_codec_t a2dp_lhdc_avdtpcodec;

#if 0
const unsigned char a2dp_codec_lhdc_elements[A2DP_LHDC_OCTET_NUMBER] = {
    0x3A, 0x05, 0x00, 0x00, //Vendor ID
    0x4C, 0x48,         //Codec ID
    (A2DP_LHDC_SR_96000 | A2DP_LHDC_SR_48000 | A2DP_LHDC_SR_44100) | (A2DP_LHDC_FMT_16 | A2DP_LHDC_FMT_24),
};
#else
//V2

const unsigned char a2dp_codec_lhdc_elements[A2DP_LHDC_OCTET_NUMBER] = {
    0x3A, 0x05, 0x00, 0x00, //Vendor ID
#if defined(A2DP_LHDC_V3)
    0x33, 0x4c,         //Codec ID
#else
    0x32, 0x4c,         //Codec ID
#endif
    (A2DP_LHDC_SR_96000 | A2DP_LHDC_SR_48000 | A2DP_LHDC_SR_44100) | (A2DP_LHDC_FMT_16 | A2DP_LHDC_FMT_24),
    (A2DP_LHDC_LLC_ENABLE | A2DP_LHDC_LLAC | A2DP_LHDC_MAX_SR_900 | A2DP_LHDC_VERSION_NUM),
    (A2DP_LHDC_COF_CSC_DISABLE | A2DP_LHDC_V4)
};
#endif /* if 0 */

btif_avdtp_codec_t * app_a2dp_codec_get_lhdc_avdtp_codec()
{
    return (btif_avdtp_codec_t *)&a2dp_lhdc_avdtpcodec;
}

void a2dp_codec_lhdc_common_init(void)
{
    a2dp_lhdc_avdtpcodec.codecType = BTIF_AVDTP_CODEC_TYPE_LHDC;
    a2dp_lhdc_avdtpcodec.discoverable = 1;
    a2dp_lhdc_avdtpcodec.elements = (U8 *)&a2dp_codec_lhdc_elements;
    a2dp_lhdc_avdtpcodec.elemLen  = sizeof(a2dp_codec_lhdc_elements);
    {
        btif_avdtp_codec_t *p = &a2dp_lhdc_avdtpcodec;
        TRACE(1,"a2dp_lhdc_avdtpcodec.elemLen = %d \n", p->elemLen);

        TRACE(5,"a2dp_lhdc_avdtpcodec.elements->[0]=0x%02x,[1]=0x%02x,[2]=0x%02x,[3]=0x%02x,[4]=0x%02x\n",
                                    p->elements[0], p->elements[1], p->elements[2],
                                    p->elements[3], p->elements[4]);

        TRACE(4,"a2dp_lhdc_avdtpcodec.elements->[5]=0x%02x,[6]=0x%02x,[7]=0x%02x,[8]=0x%02x\n",
                                    p->elements[5], p->elements[6],
                                    p->elements[7], p->elements[8]);
    }
}

bt_status_t a2dp_codec_lhdc_init(int index)
{
    struct BT_DEVICE_T *bt_dev = app_bt_get_device(index);

    a2dp_codec_lhdc_common_init();

    return btif_a2dp_register(bt_dev->btif_a2dp_stream, BTIF_A2DP_STREAM_TYPE_SINK, &a2dp_lhdc_avdtpcodec, NULL, 2, a2dp_callback);
}
#endif /* A2DP_LHDC_ON */
