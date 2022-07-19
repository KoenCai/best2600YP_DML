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
#ifndef APP_UI_EVENT_AHNDLER_H__
#define APP_UI_EVENT_AHNDLER_H__

void app_ui_event_handler_init();

ibrt_mgr_evt_t app_ibrt_mgr_map_conn_event(ibrt_conn_evt_header* ev_pkt);

void ibrt_mgr_evt_set_next_event(ibrt_mgr_evt_t evt,ibrt_mgr_evt_t extra_evt = IBRT_MGR_EV_NONE,uint8_t link_id = MOBILE_LINK_ID_INVALID);

#endif
