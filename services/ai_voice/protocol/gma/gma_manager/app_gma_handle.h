#ifndef __APP_GMA_HANDLE_H
#define __APP_GMA_HANDLE_H

#define  UUID_128_ENABLE
#define IS_ENABLE_START_DATA_XFER_STEP          0

#define IS_USE_ENCODED_DATA_SEQN_PROTECTION     0
#define IS_GMA_DEBUG_ON                         1
#define IS_FORCE_STOP_VOICE_STREAM              0

#define APP_GMA_PATH_TYPE_SPP   (1 << 0)
#define APP_GMA_PATH_TYPE_BLE   (1 << 1)
#define APP_GMA_PATH_TYPE_HFP   (1 << 2)

#if IS_USE_ENCODED_DATA_SEQN_PROTECTION
#define GMA_ENCODED_DATA_SEQN_SIZE      sizeof(uint8_t)
#else
#define GMA_ENCODED_DATA_SEQN_SIZE      0
#endif

#define GMA_VOICE_ONESHOT_PROCESS_LEN 640


#ifdef __cplusplus
extern "C" {
#endif

enum {
    APP_GMA_STATE_IDLE,
    APP_GMA_STATE_MIC_STARTING,
    APP_GMA_STATE_MIC_STARTED,
    APP_GMA_STATE_SPEAKER_STARTING,
    APP_GMA_STATE_SPEAKER_STARTED,
    APP_GMA_STATE_MAX
};

typedef enum
{
    CLOUD_MUSIC_CONTROL_NEXT = 0,
    CLOUD_MUSIC_CONTROL_PRE,
    CLOUD_MUSIC_CONTROL_PAUSE,
    CLOUD_MUSIC_CONTROL_RESUME
} CLOUD_MUSIC_CONTROL_REQ_E;

extern const struct ai_handler_func_table gma_handler_function_table;

uint32_t app_gma_stop_voice_stream(void *param1, uint32_t param2);
void app_gma_start_voice_stream_cmd(void);
void app_gma_control_cloud_music_playing(CLOUD_MUSIC_CONTROL_REQ_E req);

bool ai_get_music_music_flag(void);

void app_gma_switch_stream_status(uint8_t device_id);

void app_gma_switch_cloud_platform(uint8_t device_id);

void app_gma_throughput_test_init(void);
void app_gma_stop_throughput_test(void);
uint32_t app_gma_throughput_test_transmission_handler(void *param1, uint32_t param2, uint8_t param3);

#ifdef BLE_V2
typedef struct
{
    uint8_t     gma_gatt_adv_lenth;
    uint8_t     gma_gatt_adv_type;
    uint8_t     gma_gatt_adv_cid[2];
    uint8_t     gma_gatt_adv_vid;
    uint8_t     gma_gatt_adv_fmsk;
    uint8_t     gma_gatt_adv_pid[4];
    uint8_t     gma_gatt_adv_mac[6];
    uint8_t     gam_gatt_adv_extend[2];
}__attribute__ ((__packed__)) GMA_GATT_ADV_DATA_PAC;

//extern GMA_GATT_ADV_DATA_PAC gma_gatt_adv_data;
#endif

#ifdef __cplusplus
}
#endif

#endif

