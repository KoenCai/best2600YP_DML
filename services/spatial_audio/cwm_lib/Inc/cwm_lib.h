#ifndef ___CWM_LIB__H_
#define ___CWM_LIB__H_

#include <stdint.h>
#include "os_api.h"
#include "CWM_EventStructure.h"
#include "CWM_CustomDataStruct.h"

#define SCL_LOG                 0
#define SCL_USER_INFO           1
#define SCL_DATE_TIME           2
#define SCL_SEDENTARY           3
#define SCL_PEDO_RESET          4
#define SCL_REQ_SLEEPING_DATA   5
#define SCL_LOW_POWER_MODE      6
#define SCL_SET_ACTIVITY_MODE   7
#define SCL_GET_CHIP_INFO       8
#define SCL_ACTIVITY_CONFIG     9
#define SCL_LIB_DEBUG           10
#define SCL_PEDO_CONFIG         11
#define SCL_GET_LIB_INFO        12
#define SCL_SWIM_CONFIG         13
#define SCL_BIKING_CONFIG       14
#define SCL_ACTIVITY_PAUSE      15
#define SCL_HAND_UPDOWN_CONFIG  16
#define SCL_CHIP_VENDOR_CONFIG  17
#define SCL_SLEEP_CONFIG        18
#define SCL_ABS_STATIC_CONFIG   19
#define SCL_HEART_RATE_CONFIG   20
#define SCL_REQ_SWIM_EXIT       21
#define SCL_WATCH_FALL_CONFIG   22
#define SCL_AR_ALERT_CONFIG     23
#define SCL_ACT_PAUSE_DETECT    24
#define SCL_WM_CONFIG           25
#define SCL_INACTIVITY_CONFIG   26
#define SCL_SET_INACTIVITY_MODE 27
#define SCL_STAND_CONFIG        28
#define SCL_SS_CONFIG           29
#define SCL_ACT_INFO_CONFIG     30
#define SCL_REQ_ACTIVITY_EXIT   31

#define SCL_SZ_BREACH_CONFIG    50
#define SCL_WASH_HAND_CONFIG    52

#define SCL_ALGO_PROC_CONFIG    100
#define SCL_INPUT_SENSOR_CONFIG 101
#define SCL_INPUT_DT_CONFIG     102
#define SCL_SENS_CALI_CONFIG    110
#define SCL_SENS_CALI_CTRL_MAG  111
#define SCL_SENS_CALI_CTRL_A    112
#define SCL_SPV_CONFIG          113
#define SCL_SPV_MODE            114

#define SCL_GET_HEAP_INFO       120

#define SCL_SEC_VERIFY_CLIENT   200
#define SCL_SEC_VERIFY_SERVER   201

#define SCL_HS_ORIEN_CONFIG     300
#define SCL_HS_ORIEN_CTRL_EXC   301
#define SCL_HS_ORIEN_RESET      302
#define SCL_HS_PHONE_CONFIG     303
#define SCL_EAR_FALL_CONFIG     304

#define SCL_FUSION_MOUSE_CONFIG 501

/**
  * @brief Custom Sensor Input sensorId
  * @api CWM_CustomSensorInput
  * @param CustomSensorData.sensorType extSensorId
  * @
  */
#define CUSTOM_ACC              0
#define CUSTOM_GYRO             1
#define CUSTOM_MAG              2
#define CUSTOM_BARO             3
#define CUSTOM_TEMP             4
#define CUSTOM_HEARTRATE        5
#define CUSTOM_GNSS             6
#define CUSTOM_OFFBODY_DETECT   7
#define CUSTOM_ON_CHARGING      8
#define CUSTOM_ACC_ANY_MOTION   9
#define CUSTOM_SENS1            10


/**
  * @brief sensor enable/disable
  * @api CWM_Sensor_Enable CWM_Sensor_Disable
  * @param sensorId
  */
#define IDX_ACCEL                    0
#define IDX_GYRO                     1
#define IDX_MAG                      2
#define IDX_BARO                     3
#define IDX_TEMP                     4
#define IDX_HEARTRATE                5
#define IDX_GNSS                     6
#define IDX_OFFBODY_DETECT           7
#define IDX_REQUEST_SENSOR           11
#define IDX_ALGO_ANY_MOTION          15
#define IDX_ALGO_NO_MOTION           16
#define IDX_ONCHARGING_DETECT        18
#define IDX_ALGO_ABSOLUTE_STATIC     19
#define IDX_ALGO_SPV                 20
#define IDX_ALGO_SENS_CALIBRATION    21

typedef struct {
    int iData[16];
} SettingControl_t;


typedef void (*FN_SENSOR_LISTEN)(pSensorEVT_t);

/**
  * @brief  CyweeMotion internal process Initial
  */
void CWM_LibPreInit(os_api *api);

/**
  * @brief  Sensor Event register callback, when sensor event is triggered,
  *         this function will be called to notify
  */
void CWM_LibPostInit(FN_SENSOR_LISTEN pfn);

/**
  * @brief  Sensor enable based on sensor index provided
  * @param  sensorId: sensor register handle, if not setup this handle don't care
  * @retval 0 is success, otherwise is failed
  */
int CWM_Sensor_Enable(uint32_t sensorId);

/**
  * @brief  Sensor disable based on sensor index provided
  * @param  sensorId: sensor register handle, if not setup this handle don't care
  * @retval 0 is success, otherwise is failed
  */
int CWM_Sensor_Disable(uint32_t sensorId);

/**
  * @brief  Read sensor data and output through sensor call-back function
  */
int CWM_process2(int dt_us);

/**
  * @brief  Put custom sensor data.
  * @param  SensorDataInput: sensor data input handle, input format as below:
  *                          sensorType= CustomSensorDataIndex
  *                          fData     = sensor raw data
  * @retval 0 is success, otherwise is failed
  */
int CWM_CustomSensorInput(CustomSensorData *SensorDataInput);

/**
  * @brief  Switch log output by input parameters
  * @param  Control ID
  * @param  Setting control array
  * @retval 0 is success, otherwise is failed
  */
int CWM_SettingControl(int CtrId, SettingControl_t *pData);


#endif //___CWM_LIB__H_
