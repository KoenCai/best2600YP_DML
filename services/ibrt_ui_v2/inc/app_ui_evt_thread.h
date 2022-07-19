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
#ifndef __APP_UI_EVT_THREAD_H__
#define __APP_UI_EVT_THREAD_H__


typedef struct
{
    /* Distinguish IBRT UI/IBRT CORE/LE_AUDIO_CORE/LE_AUDIO_UI Event */
    uint32_t evt_module_type;

    void *p;
}app_ux_event_pkt;

typedef void (*APP_UI_BT_EVENT_HANDLER)(ibrt_conn_evt_header *pkt);

void app_ui_recv_event_thread_init();
void app_ui_register_bt_event_handler(APP_UI_BT_EVENT_HANDLER handler);
int app_ui_notify_ibrt_core_event(void *event);
int app_ui_notify_ibrt_ui_event(void *event);
int app_ui_notify_ble_core_event(void *event);




#endif /* __APP_UI_EVT_THREAD_H__ */

