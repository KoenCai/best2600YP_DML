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
#ifndef __IBRT_MGR_ENTRY_H__
#define __IBRT_MGR_ENTRY_H__


void app_ui_mobile_push_evt_to_cache(ibrt_mgr_mobile_sm_t *p_mobile_link_sm,ibrt_mgr_evt_t mgr_evt,uint32_t param);
void app_ui_start_run_evt_on_mobile_link(ibrt_mgr_mobile_sm_t* mobile_link_sm,ibrt_mgr_evt_t trigger_evt, uint32_t param0 = 0,uint32_t param1 = 0);

void app_ui_super_state_machine_on_event(ibrt_mgr_evt_t evt,ibrt_mgr_evt_t extra_evt = IBRT_MGR_EV_NONE,uint8_t link_id = MOBILE_LINK_ID_INVALID);;
void app_ui_handle_event(ibrt_conn_evt_header *pkt);



#endif
