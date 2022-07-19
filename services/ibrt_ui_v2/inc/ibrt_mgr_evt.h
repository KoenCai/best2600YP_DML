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
#ifndef __IBRT_MGR_EVT_H__
#define __IBRT_MGR_EVT_H__

#define BD_ADDR_LEN     6

typedef  uint16_t   ibrt_box_state;
/*
 * BOX state
 */
#define IBRT_BOX_UNKNOWN            (0)
#define IBRT_IN_BOX_CLOSED          (1)
#define IBRT_IN_BOX_OPEN            (2)
#define IBRT_OUT_BOX                (3)
#define IBRT_OUT_BOX_WEARED         (4)

#define IBRT_MGR_BOX_EVT_MASK       0x100
#define IBRT_MGR_PEER_BOX_EVT_MASK  0x200
#define IBRT_MGR_CONN_EVT_MASK      0x300
#define IBRT_MGR_IBRT_ACTION        0x800

/* sync info status */
#define SYNC_INFO_SUCCESS           (0)
#define SYNC_INFO_TIMEOUT           (1)
#define SYNC_INFO_FAIL              (2)
#define SYNC_INFO_DISALLOW          (3)

/* Exchange info status */
#define EXCHANGE_INFO_SUCCESS       (0)
#define EXCHANGE_INFO_FAILED        (1)
#define EXCHANGE_INFO_TIMEOUT       (2)

#define EXCHANGE_INFO_RESYNC        (3)
#define EXCHANGE_INFO_ONE_SIDESYNC  (4)
#define EXCHANGE_INFO_RESTART       (5)

#define MOBILE_LINK_ID_INVALID      0xFF

typedef enum {
    IBRT_MGR_EV_NONE     = 0x100,  //Box event need to keep the same as the ibrt connectoion box event define
    IBRT_MGR_EV_CASE_OPEN,
    IBRT_MGR_EV_CASE_CLOSE,
    IBRT_MGR_EV_DOCK,
    IBRT_MGR_EV_UNDOCK,
    IBRT_MGR_EV_WEAR_UP,
    IBRT_MGR_EV_WEAR_DOWN,

    IBRT_MGR_EV_PEER_EV_NONE = 0x200,
    IBRT_MGR_EV_PEER_CASE_OPEN,
    IBRT_MGR_EV_PEER_CASE_CLOSE,
    IBRT_MGR_EV_PEER_DOCK,
    IBRT_MGR_EV_PEER_UNDOCK,
    IBRT_MGR_EV_PEER_WEAR_UP,
    IBRT_MGR_EV_PEER_WEAR_DOWN,

    IBRT_MGR_EV_TW_CONNECTED     = 0x400,
    IBRT_MGR_EV_TW_DISCONNECTED,
    IBRT_MGR_EV_TW_CONNECTING,
    IBRT_MGR_EV_TW_CONNECTING_FAILURE,
    IBRT_MGR_EV_TWS_PAIRING_IN_PROCESS,
    IBRT_MGR_EV_TWS_PAIRING_COMPLETE,
    IBRT_MGR_EV_TWS_PAIRING_TIMEOUT,
    IBRT_MGR_EV_BESAUD_CONNECTED,
    IBRT_MGR_EV_MOBILE_CONNECTED,
    IBRT_MGR_EV_MOBILE_DISCONNECTED,
    IBRT_MGR_EV_MOBILE_CONNECTING,                  //0x40a
    IBRT_MGR_EV_MOBILE_CONNECTING_CANCELED,
    IBRT_MGR_EV_MOBILE_CONNECTING_CANCELE_FAILED,
    IBRT_MGR_EV_MOBILE_CONNECTING_FAILURE,
    IBRT_MGR_EV_MOBILE_AUTH_COMPLETE,
    IBRT_MGR_EV_IBRT_CONNECTING_FAILURE,
    IBRT_MGR_EV_IBRT_CONNECTED,                    //0x40F
    IBRT_MGR_EV_IBRT_DISCONNECTED,                 //0x410
    IBRT_MGR_EV_IBRT_ACL_CONNECTED,
    IBRT_MGR_EV_HOST_PAIRING_COMPLETE,
    IBRT_MGR_EV_DISCONNECT_HOST_TIMEOUT,
    IBRT_MGR_EV_DISCONNECT_HOST_RETRY_TIMEOUT,
    IBRT_MGT_EV_PROFILE_STATE_CHANGED,
    IBRT_MGR_EV_ROLE_SWITCH_COMPLETE,
    IBRT_MGR_EV_ROLE_STATUS_CHANGED,
    IBRT_MGR_EV_ROLE_SWITCH_FAILURE,
    IBRT_MGR_EV_SYNC_NV_INFO,
    IBRT_MGR_EV_SYNC_INFO_FAIL,
    IBRT_MGR_EV_SYNC_INFO_TIMEOUT,
    IBRT_MGR_EV_RE_SYNC_NV_INFO,  //0x41a
    IBRT_MGR_EV_ONE_SIDE_SYNC_NV_INFO,
    IBRT_MGR_EV_EXCHANG_INFO_COMPLETE,
    IBRT_MGR_EV_EXCHANG_INFO_FAIL,           //0x41d
    IBRT_MGR_EV_EXCHANG_INFO_TIMEOUT,
    IBRT_MGR_EV_EXCHANGE_INFO_RESTART,
    IBRT_MGR_EV_MOBILE_RUN_COMPLETE,
    IBRT_MGR_EV_EXIT_PAIRING_MODE,
    IBRT_MGR_EV_GLOBAL_STATE_CHANGED,
    IBRT_MGR_EV_ACCESS_MODE_CHANGED,
    IBRT_MGR_EV_MOBILE_NOTIFY_STATUS,
    IBRT_MGR_EV_SCO_STATE_CHANGED,
    IBRT_MGR_EV_DEL_MOBILE_ADDR_SUCCESS,
    IBRT_MGR_EV_DISC_MOBILE_LINK_SUCCESS,        //specail event

    IBRT_MGR_EV_TWS_PAIRING = 0x800,
    IBRT_MGR_EV_FREE_MAN_MODE,
    IBRT_MGR_EV_MOBILE_RECONNECT,
    IBRT_MGR_EV_PHONE_CONNECT,
    IBRT_MGR_EV_TWS_RECONNECT,
    IBRT_MGR_EV_TRY_TWS_RECONNECT,
    IBRT_MGR_EV_CHOICE_MOBILE_CONNECT,
    IBRT_MGR_EV_PASSIVE_ENTRY_PARING,
    IBRT_MGR_EV_PEER_TWS_RECONNECT,
    IBRT_MGR_EV_PEER_MOBILE_RECONNECT,
    IBRT_MGR_EV_ENTRY_PAIRING_MODE,
    IBRT_MGR_EV_MOBILE_DISCONNECT,

    IBRT_MGR_EV_TWS_FSM_IGNORE    = 0xF00,  /*It means TWS state machine don't need to handle current event,ignore it */
    IBRT_MGR_EV_IGNORE,
} ibrt_mgr_evt_t;

typedef enum {
    EXTRA_INFO_NONE                = 0x1000,
    IBRT_CONNECTED_IND,
    DEVICE_DESTROY,
} ibrt_mgr_extra_info_type_t;


typedef enum
{
    IBRT_MOBILE_CONNECT_EVT,
    IBRT_MOBILE_DISCONNECT_EVT,
    IBRT_TWS_CONNECT_EVT,
    IBRT_TWS_DISCONNECT_EVT,
    IBRT_OPEN_BOX_EVT,
    IBRT_CLOSE_BOX_EVT,
    IBRT_ENTER_PAIR_MODE_EVT,
    IBRT_EXIT_PAIR_MODE_EVT,
    IBRT_ROLE_CHANGE_EVT,
    IBRT_A2DP_PLAYING_EVT,
    IBRT_A2DP_SUSPEND_EVT,
    IBRT_SCO_PLAYING_EVT,
    IBRT_SCO_SUSPEND_EVT,
    IBRT_ENTER_FASTPAIR_EVT,
    IBRT_SEARCH_SLAVE_EVT,
    IBRT_TYPE_ERROR,
} ibrt_update_scan_evt_type;

typedef union {
    ibrt_conn_event_packet bt_conn_event;
    // add more event types as needed
} ibrt_mgr_event_packet;

typedef enum {
    IBRT_MGR_EVENT_FIRST                  = IBRT_CONN_EVENT_LAST,

    // Extend for ibrt manager
    IBRT_MGR_EVENT_BOX_STATE,
    IBRT_MGR_EVENT_OPERATION_STATE,
    IBRT_MGR_EVENT_EXCHANGE_LINK_INFO,
    IBRT_MGR_EVENT_SYNC_LINK_INFO,
    IBRT_MGR_MOBILE_SM_RUN_COMPLETE,
    IBRT_MGR_SYNC_PAIRING_MODE,    
    APP_UI_RECV_NOTIFY_LINK_EVT,                 /* Recv notify event,need switch to ui thread */
    APP_UI_RECV_PEER_EVT,                        /* Recv help run peer event,need switch to ui thread */

    IBRT_MGR_EVENT_LAST                  = 0x2000
} ibrt_ui_event_type;

typedef struct {
    ibrt_ui_event_type     type;
    uint16_t               length;
} ibrt_ui_evt_header;

typedef struct {
    ibrt_ui_evt_header     header;
    ibrt_mgr_evt_t         box_evt;
} ibrt_ui_box_state_evt;

typedef enum
{
    // Request/response information types
    REQ_NONE,
    REQ_LOCAL_RUN,        /* Request running of initiator*/
    REQ_WAIT_DECISION,    /* Request running is decision by responder*/
    REQ_LOCAL_RUN_PREFER, /* if peer mobile connected,peer run,otherwise local run */

    // Response exchange infor types from deciesion
    RSP_LOCAL_RUN,         /* Running of initiator*/
    RSP_LOCAL_RUN_DEFER,   /* Running of initiator,but defered until respondor idle*/
    RSP_PEER_RUN,          /* Running of responder */
    RSP_RESTART,

    // Sync NV address information between earbuds when NV mismatch
    RSP_FORCE_SYNC_NV,     /*Sync nv of initiator*/

    // Notify information types
    NOTIFY_LINK_IDLE,              /* Notify peer side current link is idle */
    NOTIFY_PEER_EVENT_COMPLETE,    /* Notify peer side peer event run complete */
}ibrt_ui_req_info_type_t;

typedef  uint16_t   ibrt_ui_box_state;


typedef enum
{
    SYNC_PAIRING_NONE,
    SYNC_FOR_DISCONNECTED,
    SYNC_FOR_CONNECTED_FAIL,
    SYNC_BOTH_SIDE_PAIRING_MODE,
}sync_pairing_reason_t;

typedef struct
{
    ibrt_ui_req_info_type_t   req_type;
    ibrt_ui_req_info_type_t   rsp_type;
    uint8_t                   req_link_id;
}ibrt_ui_req_hdr_t;

typedef struct {
    ibrt_ui_req_hdr_t req_hdr;
    bt_bdaddr_t         addr;
    uint8_t             link_id;
    ibrt_mgr_evt_t      box_event;
    ibrt_ui_box_state   box_state;
    bool                mobile_link_status;
    bool                ibrt_link_status;
    bool                is_busy;
    bool                exchangeinfo_ongoing;
    bool                peer_busy_status;
    bool                is_sco_active;
} ibrt_ui_share_link_info; //aligment. 18 bytes

typedef struct {
    nvrec_btdevicerecord  dev_record;
    bool                  mobile_link_status;
} ibrt_ui_nv_info_rec; //26 bytes

typedef struct {
    ibrt_ui_nv_info_rec  info_record[BT_DEVICE_NUM];
    uint16_t             mobile_connected_count;
    uint16_t             sync_count;
} ibrt_ui_sync_nv_info;

typedef struct {
    ibrt_ui_evt_header       header;
    uint32_t                 status;     /* 0:SUCCESS,1:FAIL */
    uint16_t                 restart_cnt;

    union
    {
        ibrt_ui_share_link_info share_link_info[BT_DEVICE_NUM];
        ibrt_ui_sync_nv_info    sync_link_info;
    }link_info;
} ibrt_ui_link_info_pkt;

typedef struct {
    ibrt_ui_evt_header      header;
    ibrt_mgr_evt_t          ibrt_evt;
    uint16_t                link_id;
    bt_bdaddr_t             addr;
} ibrt_ui_ibrt_pkt;

typedef struct {
    ibrt_ui_evt_header       header;
    uint8_t                  action;       /* 0:None,1:Entry,2:Exit */
    uint8_t                  action_rsp;   /* 0:Agree,1:Reject */
    sync_pairing_reason_t    reason;
} ibrt_ui_sync_paring_mode_pkt;

typedef struct {
    ibrt_ui_evt_header      header;
    uint16_t                link_id;
    uint16_t                complete_evt;
} ibrt_ui_mobile_link_pkt;

typedef struct {
    ibrt_ui_evt_header      header;
    uint8_t                 type;
} ibrt_ui_link_status_pkt;

typedef struct {
    ibrt_ui_evt_header      header;
    ibrt_mgr_evt_t          ui_evt;
    uint16_t                param_0;
    uint16_t                param_1;
} app_ui_event_pkt;


//Add more packet
typedef union {
    ibrt_ui_box_state_evt         box_state;
    ibrt_ui_link_info_pkt         ibrt_link_info;
    ibrt_ui_ibrt_pkt              ibrt_event;
    ibrt_ui_mobile_link_pkt       mobile_complete_evt;
    ibrt_ui_link_status_pkt       link_status;
    app_ui_event_pkt              peer_side_event;
}ibrt_ui_event_pkt;

const char* ibrt_mgr_event_to_string(ibrt_mgr_evt_t type);
const char* ibrt_mgr_box_state_to_string(ibrt_box_state box_state);
bool ibrt_mgr_is_user_action_event(ibrt_mgr_evt_t evt);
bool ibrt_mgr_is_peer_box_event(ibrt_mgr_evt_t evt);
bool ibrt_mgr_is_peer_event(ibrt_mgr_evt_t evt);
bool ibrt_mgr_is_box_event(ibrt_mgr_evt_t evt);
bool ibrt_mgr_is_high_priority_event(ibrt_mgr_evt_t event);
const char* ibrt_mgr_req_type_to_string(ibrt_ui_req_info_type_t type);
const char* ibrt_mgr_get_scan_trigger_str(ibrt_update_scan_evt_type trigger_evt);
ibrt_mgr_evt_t ibrt_mgr_local_evt_convert_to_peer_evt(ibrt_mgr_evt_t evt);
ibrt_mgr_evt_t ibrt_mgr_peer_evt_convert_to_local_evt(ibrt_mgr_evt_t evt);


#endif

