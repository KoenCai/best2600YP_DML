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
#ifndef __APP_UI_MOBILE_EVT_CACHE_H__
#define __APP_UI_MOBILE_EVT_CACHE_H__

void app_ui_mobile_cache_event_init();
void app_ui_mobile_push_evt_to_cache(ibrt_mgr_mobile_sm_t *mobile_link_sm, ibrt_mgr_evt_t event, uint32_t param0 = 0, uint32_t param1 = 0);
void app_ui_mobile_handle_cache_event();

#endif /* __APP_UI_MOBILE_EVT_CACHE_H__ */
