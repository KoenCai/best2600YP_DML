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

#include "a2dp_codec_aac.h"
#include "avdtp_api.h"

#if defined(A2DP_AAC_ON)

btif_avdtp_codec_t a2dp_aac_avdtpcodec;

const unsigned char a2dp_codec_aac_elements[A2DP_AAC_OCTET_NUMBER] = {
    A2DP_AAC_OCTET0_MPEG2_AAC_LC,
    A2DP_AAC_OCTET1_SAMPLING_FREQUENCY_44100,
    A2DP_AAC_OCTET2_CHANNELS_1 | A2DP_AAC_OCTET2_CHANNELS_2 | A2DP_AAC_OCTET2_SAMPLING_FREQUENCY_48000,
    A2DP_AAC_OCTET3_VBR_SUPPORTED | ((MAX_AAC_BITRATE >> 16) & 0x7f),
    /* left bit rate 0 for unkown */
    (MAX_AAC_BITRATE >> 8) & 0xff,
    (MAX_AAC_BITRATE) & 0xff
};


btif_avdtp_codec_t *app_a2dp_codec_get_aac_avdtp_codec()
{
    return (btif_avdtp_codec_t *)&a2dp_aac_avdtpcodec;
}

void a2dp_codec_aac_common_init(void)
{
    a2dp_aac_avdtpcodec.codecType = BTIF_AVDTP_CODEC_TYPE_MPEG2_4_AAC;
    a2dp_aac_avdtpcodec.discoverable = 1;
    a2dp_aac_avdtpcodec.elements = (U8 *)&a2dp_codec_aac_elements;
    a2dp_aac_avdtpcodec.elemLen  = sizeof(a2dp_codec_aac_elements);
}

bt_status_t a2dp_codec_aac_init(int index)
{
    struct BT_DEVICE_T *bt_dev = app_bt_get_device(index);
    btif_avdtp_content_prot_t *cp = NULL;

#ifdef __A2DP_AVDTP_CP__
    bt_dev->a2dp_avdtp_cp.cpType = BTIF_AVDTP_CP_TYPE_SCMS_T;
    bt_dev->a2dp_avdtp_cp.data = bt_dev->a2dp_avdtp_cp_security_data;
    bt_dev->a2dp_avdtp_cp.dataLen = 0;
    cp = &bt_dev->a2dp_avdtp_cp;
#endif

    a2dp_codec_aac_common_init();

    return btif_a2dp_register(bt_dev->btif_a2dp_stream, BTIF_A2DP_STREAM_TYPE_SINK, &a2dp_aac_avdtpcodec, cp, 1, a2dp_callback);
}

#endif /* A2DP_AAC_ON */
