/**
 * @file app_display.h
 * @author BES AI team
 * @version 0.1
 * @date 2021-05-10
 * 
 * @copyright Copyright (c) 2015-2021 BES Technic.
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
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#ifndef APP_DISPLAY_H_
#define APP_DISPLAY_H_

#include <co_bt.h>           // Common BT definitions

/**
 ****************************************************************************************
 * @brief Init the application display
 ****************************************************************************************
 */
void app_display_init(void);

/**
 ****************************************************************************************
 * @brief Update data of the advertising screen
 ****************************************************************************************
 */
void app_display_set_adv(bool enable);

/**
 ****************************************************************************************
 * @brief Update data of the advertising screen
 ****************************************************************************************
 */
void app_display_set_con(bool state);

/**
 ****************************************************************************************
 * @brief Update data of the bond status screen
 ****************************************************************************************
 */
void app_display_set_bond(bool bonded);

#if (KE_PROFILING)
//void app_display_hdl_env_size(uint16_t heap_size, uint16_t used_size);
//void app_display_hdl_db_size(uint16_t heap_size, uint16_t used_size);
//void app_display_hdl_msg_size(uint16_t heap_size, uint16_t used_size);
#endif //(KE_PROFILING)

#if (BLE_APP_HT)
/**
 ****************************************************************************************
 * @brief Display the screen containing the PIN Code to use during a pairing process.
 ****************************************************************************************
 */
void app_display_pin_code(uint32_t pin_code);

/**
 ****************************************************************************************
 * @brief Update data of the temperature value screen
 ****************************************************************************************
 */
void app_display_update_temp_val_screen(uint32_t value);

/**
 ****************************************************************************************
 * @brief Update data of the temperature type screen
 ****************************************************************************************
 */
void app_display_update_temp_type_screen(const char* type_string);
#endif //(BLE_APP_HT)

#if (BLE_APP_HID)
/**
 ****************************************************************************************
 * @brief Update mouse data of the HID Mouse screen
 ****************************************************************************************
 */
void app_display_update_hid_value_screen(uint8_t b, uint8_t x,
                                         uint8_t y, uint8_t w);
#endif //(BLE_APP_HID)


#if (BLE_APP_AM0)
/**
 ****************************************************************************************
 * @brief Update Audio over LE info
 ****************************************************************************************
 */
void app_display_update_audio_info(uint8_t state, uint8_t volume);
#endif //(BLE_APP_AM0)



#endif // APP_DISPLAY_H_

/// @} APP
