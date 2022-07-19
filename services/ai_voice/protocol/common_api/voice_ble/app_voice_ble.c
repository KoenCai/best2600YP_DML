/**
 ****************************************************************************************
 *
 * @file app_sv.c
 *
 * @brief Smart Voice Application entry point
 *
 * Copyright (C) BES
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */
#ifdef __AI_VOICE_BLE_ENABLE__



/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "itf_rwip_config.h"     // SW configuration
#include "itf_prf_types.h"
#include "itf_prf_utils.h"
#include "itf_prf.h"
#include "string.h"


#include "ai_transport.h"
#include "ai_manager.h"
#include "ai_thread.h"
#include "ai_control.h"
#include "ai_gatt_server.h"
#include "app_ai_ble.h"
#include "app_voice_ble.h"                  // ama Voice Application Definitions
#include "voice_gatt_server.h"
#include "app_voice_handle.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */


bool app_voice_send_cmd_via_notification(uint8_t* ptrData, uint32_t length)
{
    struct ble_ai_send_data_req_t * req = KE_MSG_ALLOC_DYN(VOICE_SEND_CMD_VIA_NOTIFICATION,
                                                prf_get_task_from_id(TASK_ID_AI),
                                                TASK_APP,
                                                ble_ai_send_data_req_t,
                                                length);
    req->connecionIndex = app_ai_env.connectionIndex;
    req->length = length;
    memcpy(req->value, ptrData, length);

    ke_msg_send(req);
    return true;
}

void app_voice_send_cmd_via_indication(uint8_t* ptrData, uint32_t length)
{
    struct ble_ai_send_data_req_t * req = KE_MSG_ALLOC_DYN(VOICE_SEND_CMD_VIA_INDICATION,
                                                prf_get_task_from_id(TASK_ID_AI),
                                                TASK_APP,
                                                ble_ai_send_data_req_t,
                                                length);
    req->connecionIndex = app_ai_env.connectionIndex;
    req->length = length;
    memcpy(req->value, ptrData, length);

    ke_msg_send(req);
}

bool app_voice_send_data_via_notification(uint8_t* ptrData, uint32_t length)
{
    struct ble_ai_send_data_req_t * req = KE_MSG_ALLOC_DYN(VOICE_SEND_DATA_VIA_NOTIFICATION,
                                                prf_get_task_from_id(TASK_ID_AI),
                                                TASK_APP,
                                                ble_ai_send_data_req_t,
                                                length);
    TRACE(2,"%s length %d", __func__, length);
    req->connecionIndex = app_ai_env.connectionIndex;
    req->length = length;
    memcpy(req->value, ptrData, length);

    ke_msg_send(req);
    return TRUE;
}

void app_voice_send_data_via_indication(uint8_t* ptrData, uint32_t length)
{
    struct ble_ai_send_data_req_t * req = KE_MSG_ALLOC_DYN(VOICE_SEND_DATA_VIA_INDICATION,
                                                prf_get_task_from_id(TASK_ID_AI),
                                                TASK_APP,
                                                ble_ai_send_data_req_t,
                                                length);
    req->connecionIndex = app_ai_env.connectionIndex;
    req->length = length;
    memcpy(req->value, ptrData, length);

    ke_msg_send(req);
}

static int app_voice_msg_handler(ke_msg_id_t const msgid,
                              void const *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id)
{
    // Do nothing
    return (KE_MSG_CONSUMED);
}

static int app_voice_cmd_ccc_changed_handler(ke_msg_id_t const msgid,
                              struct ble_ai_tx_notif_config_t *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id)
{
    TRACE(2,"%s %d", __func__, param->isNotificationEnabled);
    app_ai_env.isCmdNotificationEnabled = param->isNotificationEnabled;
    if (app_ai_env.isCmdNotificationEnabled)
    {
        if (BLE_INVALID_CONNECTION_INDEX == app_ai_env.connectionIndex)
        {
            uint8_t conidx = KE_IDX_GET(src_id);
            app_ai_connected_evt_handler(conidx);

            if (app_ai_env.mtu[conidx] > 0)
            {
                ai_function_handle(CALLBACK_UPDATE_MTU, NULL, app_ai_env.mtu[conidx]);
            }
        }
    }

    ai_function_handle(CALLBACK_AI_CONNECT, NULL, AI_TRANSPORT_BLE);

    return (KE_MSG_CONSUMED);
}

static int app_voice_data_ccc_changed_handler(ke_msg_id_t const msgid,
                              struct ble_ai_tx_notif_config_t *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id)
{
    TRACE(2,"%s %d", __func__, param->isNotificationEnabled);
    app_ai_env.isDataNotificationEnabled = param->isNotificationEnabled;

    /*
    if (!app_sv_env.isDataNotificationEnabled)
    {
        // widen the connection interval to save the power consumption
        app_sv_update_conn_parameter(LOW_SPEED_BLE_CONNECTION_INTERVAL_MIN_IN_MS, 
            LOW_SPEED_BLE_CONNECTION_INTERVAL_MAX_IN_MS, LOW_SPEED_BLE_CONNECTION_SUPERVISOR_TIMEOUT_IN_MS);
    }
    else
    {
        // narrow the connection interval to increase the speed
        app_sv_update_conn_parameter(HIGH_SPEED_BLE_CONNECTION_INTERVAL_MIN_IN_MS, 
            HIGH_SPEED_BLE_CONNECTION_INTERVAL_MAX_IN_MS, HIGH_SPEED_BLE_CONNECTION_SUPERVISOR_TIMEOUT_IN_MS);
    }
    */
    
    return (KE_MSG_CONSUMED);
}

static int app_voice_tx_data_sent_handler(ke_msg_id_t const msgid,
                              struct ble_ai_tx_sent_ind_t *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id)
{
    ai_function_handle(CALLBACK_DATA_SEND_DONE, NULL, 0);

    return (KE_MSG_CONSUMED);
}

static int app_voice_rx_cmd_received_handler(ke_msg_id_t const msgid,
                              struct ble_ai_rx_data_ind_t *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id)
{
    TRACE(2,"%s length is %d", __func__, param->length);
    ai_function_handle(CALLBACK_DATA_RECEIVE, param->data, param->length);
    return (KE_MSG_CONSUMED);
}

static int app_voice_rx_data_received_handler(ke_msg_id_t const msgid,
                              struct ble_ai_rx_data_ind_t *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id)
{
    ai_function_handle(CALLBACK_DATA_RECEIVE, param->data, param->length);
    return (KE_MSG_CONSUMED);
}

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_voice_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,            (ke_msg_func_t)app_voice_msg_handler},

    {VOICE_CMD_CCC_CHANGED,        (ke_msg_func_t)app_voice_cmd_ccc_changed_handler},
    {VOICE_DATA_CCC_CHANGED,       (ke_msg_func_t)app_voice_data_ccc_changed_handler},
    {VOICE_TX_DATA_SENT,           (ke_msg_func_t)app_voice_tx_data_sent_handler},
    {VOICE_CMD_RECEIVED,           (ke_msg_func_t)app_voice_rx_cmd_received_handler},
    {VOICE_DATA_RECEIVED,          (ke_msg_func_t)app_voice_rx_data_received_handler},
};

const struct ke_state_handler app_voice_table_handler =
    {&app_voice_msg_handler_list[0], (sizeof(app_voice_msg_handler_list)/sizeof(struct ke_msg_handler))};

AI_BLE_HANDLER_TO_ADD(AI_SPEC_COMMON, &app_voice_table_handler)

#endif //__AI_VOICE_BLE_ENABLE__
