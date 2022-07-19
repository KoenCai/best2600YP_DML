/**
 ****************************************************************************************
 *
 * @file itf_co_bt.h
 *
 * @brief This file contains the common Bluetooth defines, enumerations and structures
 *        definitions for use by all modules in RW stack.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef _ITF_CO_BT_H_
#define _ITF_CO_BT_H_

/**
 ****************************************************************************************
 * @addtogroup COMMON Common SW Block
 * @ingroup ROOT
 * @brief The Common RW SW Block.
 *
 * The COMMON is the block with Bluetooth definitions and structures shared
 * to all the protocol stack blocks. This also contain software wide error code
 * definitions, mathematical functions, help functions, list and buffer definitions.
 *
 * @{
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup CO_BT Common Bluetooth defines
 * @ingroup COMMON
 * @brief Common Bluetooth definitions and structures.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdbool.h>       // standard boolean definitions
#include <stddef.h>        // standard definitions
#include <stdint.h>        // standard integer definitions

/*
 * DEFINES
 ****************************************************************************************
 */

#include "itf_co_bt_defines.h" // Bluetooth defines
#include "itf_co_lmp.h"        // Bluetooth LMP definitions
#include "itf_co_hci.h"        // Bluetooth HCI definitions
#include "itf_co_error.h"      // Bluetooth error codes definitions

/// @} ITF_CO_BT
#endif // _ITF_CO_BT_H_
