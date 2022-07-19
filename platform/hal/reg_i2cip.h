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
#ifndef __REG_I2CIP_H_
#define __REG_I2CIP_H_

#include "plat_types.h"

#define I2CIP_TX_FIFO_DEPTH (8)
#define I2CIP_RX_FIFO_DEPTH (8)

#define NANO_TO_MICRO       1000

#define I2CIP_SS_SCL_HCNT_REG_OFFSET 0x14
#define I2CIP_SS_SCL_HCNT_SHIFT (0)
#define I2CIP_SS_SCL_HCNT_MASK ((0xffff)<<I2CIP_SS_SCL_HCNT_SHIFT)

#define I2CIP_SS_SCL_LCNT_REG_OFFSET 0x18
#define I2CIP_SS_SCL_LCNT_SHIFT (0)
#define I2CIP_SS_SCL_LCNT_MASK ((0xffff)<<I2CIP_SS_SCL_LCNT_SHIFT)

#define I2CIP_FS_SCL_HCNT_REG_OFFSET 0x1C
#define I2CIP_FS_SCL_HCNT_SHIFT (0)
#define I2CIP_FS_SCL_HCNT_MASK ((0xffff)<<I2CIP_FS_SCL_HCNT_SHIFT)

#define I2CIP_FS_SCL_LCNT_REG_OFFSET 0x20
#define I2CIP_FS_SCL_LCNT_SHIFT (0)
#define I2CIP_FS_SCL_LCNT_MASK ((0xffff)<<I2CIP_FS_SCL_LCNT_SHIFT)

#define I2CIP_HS_SCL_HCNT_REG_OFFSET 0x24
#define I2CIP_HS_SCL_HCNT_SHIFT (0)
#define I2CIP_HS_SCL_HCNT_MASK ((0xffff)<<I2CIP_HS_SCL_hCNT_SHIFT)

#define I2CIP_HS_SCL_LCNT_REG_OFFSET 0x28
#define I2CIP_HS_SCL_LCNT_SHIFT (0)
#define I2CIP_HS_SCL_LCNT_MASK ((0xffff)<<I2CIP_HS_SCL_LCNT_SHIFT)

/* High and low times in different speed modes (in ns) */
#define MIN_SS_SCL_HIGHTIME 4000
#define MIN_SS_SCL_LOWTIME  4700
#define MIN_FS_SCL_HIGHTIME 600
#define MIN_FS_SCL_LOWTIME  1300
#define MIN_HS_SCL_HIGHTIME 60
#define MIN_HS_SCL_LOWTIME  160

/* Worst case timeout for 1 byte is kept as 2ms */
#define I2C_BYTE_TO     (CONFIG_SYSTICK_HZ/500)
#define I2C_STOPDET_TO      (CONFIG_SYSTICK_HZ/500)
#define I2C_BYTE_TO_BB      (I2C_BYTE_TO * 16)

/* i2c control register definitions */
#define I2CIP_CTRL_REG_OFFSET 0x0
#define I2CIP_SPEED_SHIFT (1)
#define I2CIP_SPEED_MASK ((0x3)<<I2CIP_SPEED_SHIFT)
#define I2CIP_HIGH_SPEED_SHIFT (1)
#define I2CIP_HIGH_SPEED_MASK ((0x3)<<I2CIP_HIGH_SPEED_SHIFT)
#define I2CIP_FAST_SPEED_SHIFT (1)
#define I2CIP_FAST_SPEED_MASK ((0x2)<<I2CIP_HIGH_SPEED_SHIFT)
#define I2CIP_STANDARD_SPEED_SHIFT (1)
#define I2CIP_STANDARD_SPEED_MASK ((0x1)<<I2CIP_STANDARD_SPEED_SHIFT)
#define I2CIP_SLAVE_DISABLE_SHIFT (6)
#define I2CIP_SLAVE_DISABLE_MASK ((0x1)<<I2CIP_SLAVE_DISABLE_SHIFT)
#define I2CIP_RESTART_ENABLE_SHIFT (5)
#define I2CIP_RESTART_ENABLE_MASK ((0x1)<<I2CIP_RESTART_ENABLE_SHIFT)
#define I2CIP_MASTER_MODE_SHIFT (0)
#define I2CIP_MASTER_MODE_MASK ((0x1)<<I2CIP_MASTER_MODE_SHIFT)
#define I2CIP_10BITADDR_MASTER_SHIFT (4)
#define I2CIP_10BITADDR_MASTER_MASK ((0x1)<<I2CIP_10BITADDR_MASTER_SHIFT)
#define I2CIP_10BITADDR_SLAVE_SHIFT (3)
#define I2CIP_10BITADDR_SLAVE_MASK ((0x1)<<I2CIP_10BITADDR_SLAVE_SHIFT)

/* i2c target address register definitions */
#define I2CIP_TARGET_ADDRESS_REG_OFFSET 0x04
#define I2CIP_TARGET_ADDRESS_SHIFT 0
#define I2CIP_TARGET_ADDRESS_MASK ((0x3ff)<<I2CIP_TARGET_ADDRESS_SHIFT)
#define I2CIP_TARGET_ADDRESS_IC_10BITADDR_MASTER_SHIFT 12
#define I2CIP_TARGET_ADDRESS_IC_10BITADDR_MASTER_MASK (1 << I2CIP_TARGET_ADDRESS_IC_10BITADDR_MASTER_SHIFT)
#define I2CIP_TARGET_ADDRESS_SPECIAL_BIT_SHIFT 11
#define I2CIP_TARGET_ADDRESS_SPECIAL_BIT_MASK ((0x1)<<I2CIP_TARGET_ADDRESS_SPECIAL_BIT_SHIFT)
#define I2CIP_TARGET_ADDRESS_GC_OR_START_SHIFT 10
#define I2CIP_TARGET_ADDRESS_GC_OR_START_MASK ((0x1)<<I2CIP_TARGET_ADDRESS_GC_OR_START_SHIFT)

/* i2c slave address register definitions */
#define I2CIP_ADDRESS_AS_SLAVE_REG_OFFSET 0x08
#define I2CIP_ADDRESS_AS_SLAVE_SHIFT (0)
#define I2CIP_ADDRESS_AS_SLAVE_MASK ((0x3ff)<<I2CIP_ADDRESS_AS_SLAVE_SHIFT)

/* i2c data buffer and command register definitions */
#define I2CIP_CMD_DATA_REG_OFFSET 0x10
#define I2CIP_CMD_DATA_CMD_SHIFT (8)
#define I2CIP_CMD_DATA_CMD_MASK ((0x1)<<I2CIP_CMD_DATA_CMD_SHIFT)
#define I2CIP_CMD_DATA_CMD_READ_SHIFT (8)
#define I2CIP_CMD_DATA_CMD_READ_MASK ((0x1)<<I2CIP_CMD_DATA_CMD_READ_SHIFT)
#define I2CIP_CMD_DATA_CMD_WRITE_SHIFT (8)
#define I2CIP_CMD_DATA_CMD_WRITE_MASK ((0x0)<<I2CIP_CMD_DATA_CMD_WRITE_SHIFT)
#define I2CIP_CMD_DATA_STOP_SHIFT (9)
#define I2CIP_CMD_DATA_STOP_MASK ((0x1)<<I2CIP_CMD_DATA_STOP_SHIFT)
#define I2CIP_CMD_DATA_RESTART_SHIFT (10)
#define I2CIP_CMD_DATA_RESTART_MASK ((0x1)<<I2CIP_CMD_DATA_RESTART_SHIFT)

/* i2c interrupt status register definitions */
#define I2CIP_INT_STATUS_REG_OFFSET 0x2C
#define I2CIP_INT_STATUS_GEN_CALL_SHIFT (11)
#define I2CIP_INT_STATUS_GEN_CALL_MASK ((0x1)<<I2CIP_INT_STATUS_GEN_CALL_SHIFT)
#define I2CIP_INT_STATUS_START_DET_SHIFT (10)
#define I2CIP_INT_STATUS_START_DET_MASK ((0x1)<<I2CIP_INT_STATUS_START_DET_SHIFT)
#define I2CIP_INT_STATUS_STOP_DET_SHIFT (9)
#define I2CIP_INT_STATUS_STOP_DET_MASK ((0x1)<<I2CIP_INT_STATUS_STOP_DET_SHIFT)
#define I2CIP_INT_STATUS_ACTIVITY_SHIFT (8)
#define I2CIP_INT_STATUS_ACTIVITY_MASK  ((0x1)<<I2CIP_INT_STATUS_ACTIVITY_SHIFT)
#define I2CIP_INT_STATUS_RX_DONE_SHIFT  (7)
#define I2CIP_INT_STATUS_RX_DONE_MASK  ((0x1)<<I2CIP_INT_STATUS_RX_DONE_SHIFT)
#define I2CIP_INT_STATUS_TX_ABRT_SHIFT (6)
#define I2CIP_INT_STATUS_TX_ABRT_MASK ((0x1)<<I2CIP_INT_STATUS_TX_ABRT_SHIFT)
#define I2CIP_INT_STATUS_RD_REQ_SHIFT (5)
#define I2CIP_INT_STATUS_RD_REQ_MASK ((0x1)<<I2CIP_INT_STATUS_RD_REQ_SHIFT)
#define I2CIP_INT_STATUS_TX_EMPTY_SHIFT (4)
#define I2CIP_INT_STATUS_TX_EMPTY_MASK ((0x1)<<I2CIP_INT_STATUS_TX_EMPTY_SHIFT)
#define I2CIP_INT_STATUS_TX_OVER_SHIFT (3)
#define I2CIP_INT_STATUS_TX_OVER_MASK ((0x1)<<I2CIP_INT_STATUS_TX_OVER_SHIFT)
#define I2CIP_INT_STATUS_RX_FULL_SHIFT (2)
#define I2CIP_INT_STATUS_RX_FULL_MASK ((0x1)<<I2CIP_INT_STATUS_RX_FULL_SHIFT)
#define I2CIP_INT_STATUS_RX_OVER_SHIFT (1)
#define I2CIP_INT_STATUS_RX_OVER_MASK ((0x1)<<I2CIP_INT_STATUS_RX_OVER_SHIFT)
#define I2CIP_INT_STATUS_RX_UNDER_SHIFT (0)
#define I2CIP_INT_STATUS_RX_UNDER_MASK ((0x1)<<I2CIP_INT_STATUS_RX_UNDER_SHIFT)

/* i2c raw interrupt status register definitions */
#define I2CIP_RAW_INT_STATUS_REG_OFFSET 0x34
#define I2CIP_RAW_INT_STATUS_GEN_CALL_SHIFT (11)
#define I2CIP_RAW_INT_STATUS_GEN_CALL_MASK ((0x1)<<I2CIP_RAW_INT_STATUS_GEN_CALL_SHIFT)
#define I2CIP_RAW_INT_STATUS_START_DET_SHIFT (10)
#define I2CIP_RAW_INT_STATUS_START_DET_MASK ((0x1)<<I2CIP_RAW_INT_STATUS_START_DET_SHIFT)
#define I2CIP_RAW_INT_STATUS_STOP_DET_SHIFT (9)
#define I2CIP_RAW_INT_STATUS_STOP_DET_MASK ((0x1)<<I2CIP_RAW_INT_STATUS_STOP_DET_SHIFT)
#define I2CIP_RAW_INT_STATUS_ACTIVITY_SHIFT (8)
#define I2CIP_RAW_INT_STATUS_ACTIVITY_MASK  ((0x1)<<I2CIP_RAW_INT_STATUS_ACTIVITY_SHIFT)
#define I2CIP_RAW_INT_STATUS_RX_DONE_SHIFT  (7)
#define I2CIP_RAW_INT_STATUS_RX_DONE_MASK  ((0x1)<<I2CIP_RAW_INT_STATUS_RX_DONE_SHIFT)
#define I2CIP_RAW_INT_STATUS_TX_ABRT_SHIFT (6)
#define I2CIP_RAW_INT_STATUS_TX_ABRT_MASK ((0x1)<<I2CIP_RAW_INT_STATUS_TX_ABRT_SHIFT)
#define I2CIP_RAW_INT_STATUS_RD_REQ_SHIFT (5)
#define I2CIP_RAW_INT_STATUS_RD_REQ_MASK ((0x1)<<I2CIP_RAW_INT_STATUS_RD_REQ_SHIFT)
#define I2CIP_RAW_INT_STATUS_TX_EMPTY_SHIFT (4)
#define I2CIP_RAW_INT_STATUS_TX_EMPTY_MASK ((0x1)<<I2CIP_RAW_INT_STATUS_TX_EMPTY_SHIFT)
#define I2CIP_RAW_INT_STATUS_TX_OVER_SHIFT (3)
#define I2CIP_RAW_INT_STATUS_TX_OVER_MASK ((0x1)<<I2CIP_RAW_INT_STATUS_TX_OVER_SHIFT)
#define I2CIP_RAW_INT_STATUS_RX_FULL_SHIFT (2)
#define I2CIP_RAW_INT_STATUS_RX_FULL_MASK ((0x1)<<I2CIP_RAW_INT_STATUS_RX_FULL_SHIFT)
#define I2CIP_RAW_INT_STATUS_RX_OVER_SHIFT (1)
#define I2CIP_RAW_INT_STATUS_RX_OVER_MASK ((0x1)<<I2CIP_RAW_INT_STATUS_RX_OVER_SHIFT)
#define I2CIP_RAW_INT_STATUS_RX_UNDER_SHIFT (0)
#define I2CIP_RAW_INT_STATUS_RX_UNDER_MASK ((0x1)<<I2CIP_RAW_INT_STATUS_RX_UNDER_SHIFT)

/* fifo threshold register definitions */
#define I2CIP_RX_THRESHOLD_REG_OFFSET 0x38
#define I2CIP_RX_TL_DEPTH     (I2CIP_RX_FIFO_DEPTH - 1)
#define I2CIP_RX_TL_QUARTER   (I2CIP_RX_FIFO_DEPTH/4 - 1)
#define I2CIP_RX_TL_HALF      (I2CIP_RX_FIFO_DEPTH/2 - 1)
#define I2CIP_RX_TL_THREE_QUARTER   (I2CIP_RX_FIFO_DEPTH*3/4 - 1)
#define I2CIP_RX_TL_1_BYTE    (0)
#define I2CIP_TX_THRESHOLD_REG_OFFSET 0x3C
#define I2CIP_TX_TL_DEPTH     ((I2CIP_TX_FIFO_DEPTH))
#define I2CIP_TX_TL_QUARTER   (I2CIP_TX_FIFO_DEPTH/4)
#define I2CIP_TX_TL_HALF      (I2CIP_TX_FIFO_DEPTH/2)
#define I2CIP_TX_TL_THREE_QUARTER   (I2CIP_TX_FIFO_DEPTH*3/4)
#define I2CIP_TX_TL_1_BYTE    (1)
#define I2CIP_TX_TL_ZERO      (0)

/* i2c enable register definitions */
#define I2CIP_ENABLE_REG_OFFSET 0x6c
#define I2CIP_ENABLE_SHIFT 0
#define I2CIP_ENABLE_MASK ((0x1)<<I2CIP_ENABLE_SHIFT)

/* i2c status register  definitions */
#define I2CIP_STATUS_REG_OFFSET 0x70
#define I2CIP_STATUS_SA_SHIFT (6)
#define I2CIP_STATUS_SA_MASK ((0x1)<<I2CIP_STATUS_SA_SHIFT)
#define I2CIP_STATUS_SHIFT (5)
#define I2CIP_STATUS_MASK ((0x1)<<I2CIP_STATUS_SHIFT)
#define I2CIP_STATUS_RFF_SHIFT (4)
#define I2CIP_STATUS_RFF_MASK ((0x1)<<I2CIP_STATUS_RFF_SHIFT)
#define I2CIP_STATUS_RFNE_SHIFT (3)
#define I2CIP_STATUS_RFNE_MASK ((0x1)<<I2CIP_STATUS_RFNE_SHIFT)
#define I2CIP_STATUS_TFE_SHIFT (2)
#define I2CIP_STATUS_TFE_MASK ((0x1)<<I2CIP_STATUS_TFE_SHIFT)
#define I2CIP_STATUS_TFNF_SHIFT (1)
#define I2CIP_STATUS_TFNF_MASK ((0x1)<<I2CIP_STATUS_TFNF_SHIFT)
#define I2CIP_STATUS_ACT_SHIFT (0)
#define I2CIP_STATUS_ACT_MASK ((0x1)<<I2CIP_STATUS_ACT_SHIFT)

/* i2c interrupt mask register */
#define I2CIP_INT_MASK_REG_OFFSET 0x30
#define I2CIP_INT_MASK_GEN_CALL_SHIFT (11)
#define I2CIP_INT_MASK_GEN_CALL_MASK ((0x1)<<I2CIP_INT_MASK_GEN_CALL_SHIFT)
#define I2CIP_INT_MASK_START_DET_SHIFT (10)
#define I2CIP_INT_MASK_START_DET_MASK ((0x1)<<I2CIP_INT_MASK_START_DET_SHIFT)
#define I2CIP_INT_MASK_STOP_DET_SHIFT (9)
#define I2CIP_INT_MASK_STOP_DET_MASK ((0x1)<<I2CIP_INT_MASK_STOP_DET_SHIFT)
#define I2CIP_INT_MASK_ACTIVITY_SHIFT (8)
#define I2CIP_INT_MASK_ACTIVITY_MASK ((0x1)<<I2CIP_INT_MASK_ACTIVITY_SHIFT)
#define I2CIP_INT_MASK_RX_DONE_SHIFT (7)
#define I2CIP_INT_MASK_RX_DONE_MASK ((0x1)<<I2CIP_INT_MASK_RX_DONE_SHIFT)
#define I2CIP_INT_MASK_TX_ABRT_SHIFT (6)
#define I2CIP_INT_MASK_TX_ABRT_MASK ((0x1)<<I2CIP_INT_MASK_TX_ABRT_SHIFT)
#define I2CIP_INT_MASK_RD_REQ_SHIFT (5)
#define I2CIP_INT_MASK_RD_REQ_MASK ((0x1)<<I2CIP_INT_MASK_RD_REQ_SHIFT)
#define I2CIP_INT_MASK_TX_EMPTY_SHIFT (4)
#define I2CIP_INT_MASK_TX_EMPTY_MASK ((0x1)<<I2CIP_INT_MASK_TX_EMPTY_SHIFT)
#define I2CIP_INT_MASK_TX_OVER_SHIFT (3)
#define I2CIP_INT_MASK_TX_OVER_MASK ((0x1)<<I2CIP_INT_MASK_TX_OVER_SHIFT)
#define I2CIP_INT_MASK_RX_FULL_SHIFT (2)
#define I2CIP_INT_MASK_RX_FULL_MASK ((0x1)<<I2CIP_INT_MASK_RX_FULL_SHIFT)
#define I2CIP_INT_MASK_RX_OVER_SHIFT (1)
#define I2CIP_INT_MASK_RX_OVER_MASK ((0x1)<<I2CIP_INT_MASK_RX_OVER_SHIFT)
#define I2CIP_INT_MASK_RX_UNDER_SHIFT (0)
#define I2CIP_INT_MASK_RX_UNDER_MASK ((0x1)<<I2CIP_INT_MASK_RX_UNDER_SHIFT)
#define I2CIP_INT_MASK_NONE (0)
#define I2CIP_INT_MASK_ALL \
    (I2CIP_INT_MASK_GEN_CALL_MASK | \
     I2CIP_INT_MASK_START_DET_MASK | \
     I2CIP_INT_MASK_STOP_DET_MASK | \
     I2CIP_INT_MASK_ACTIVITY_MASK | \
     I2CIP_INT_MASK_RX_DONE_MASK | \
     I2CIP_INT_MASK_TX_ABRT_MASK | \
     I2CIP_INT_MASK_RD_REQ_MASK | \
     I2CIP_INT_MASK_TX_EMPTY_MASK | \
     I2CIP_INT_MASK_TX_OVER_MASK | \
     I2CIP_INT_MASK_RX_FULL_MASK | \
     I2CIP_INT_MASK_RX_OVER_MASK | \
     I2CIP_INT_MASK_RX_UNDER_MASK)
#define I2CIP_INT_MASK_ERROR_MASK \
    (I2CIP_INT_MASK_TX_ABRT_MASK | \
    I2CIP_INT_MASK_TX_OVER_MASK | \
    I2CIP_INT_MASK_RX_OVER_MASK | \
    I2CIP_INT_MASK_RX_UNDER_MASK)

/* i2c enable status register definitions */
#define I2CIP_ENABLE_STATUS_REG_OFFSET 0x9c
#define I2CIP_ENABLE_STATUS_ENABLE_SHIFT 0
#define I2CIP_ENABLE_STATUS_ENABLE_MASK ((0x1)<<I2CIP_ENABLE_STATUS_ENABLE_SHIFT)

/* i2c tx fifo level register definitions */
#define I2CIP_TX_FIFO_LEVEL_REG_OFFSET 0x74

/* i2c rx fifo level register definitions */
#define I2CIP_RX_FIFO_LEVEL_REG_OFFSET 0x78
/* i2c sda hold time register */
#define I2CIP_SDA_HOLD_REG_OFFSET 0x7c

/* i2c clear all intr register */
#define I2CIP_CLR_ALL_INTR_REG_OFFSET 0x40
/* i2c clear rx under register */
#define I2CIP_CLR_RX_UNDER_REG_OFFSET 0x44
/* i2c clear rx over register */
#define I2CIP_CLR_RX_OVER_REG_OFFSET 0x48
/* i2c clear tx over register */
#define I2CIP_CLR_TX_OVER_REG_OFFSET 0x4c
/* i2c clear rd req register */
#define I2CIP_CLR_RD_REQ_REG_OFFSET 0x50
/* i2c clear tx abrt register */
#define I2CIP_CLR_TX_ABRT_REG_OFFSET 0x54
/* i2c clear rx done register */
#define I2CIP_CLR_RX_DONE_REG_OFFSET 0x58
/* i2c clear activity register */
#define I2CIP_CLR_ACTIVITY_REG_OFFSET 0x5c
/* i2c clear stop det register */
#define I2CIP_CLR_STOP_DET_REG_OFFSET 0x60
/* i2c clear start det register */
#define I2CIP_CLR_START_DET_REG_OFFSET 0x64
/* i2c clear gen call register */
#define I2CIP_CLR_GEN_CALL_REG_OFFSET 0x68

/* i2c tx abrt source register */
#define I2CIP_TX_ABRT_SOURCE_REG_OFFSET 0x80
#define I2CIP_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_SHIFT (15)
#define I2CIP_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_SHIFT (14)
#define I2CIP_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_SHIFT (13)
#define I2CIP_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ARB_LOST_SHIFT (12)
#define I2CIP_TX_ABRT_SOURCE_ARB_LOST_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ARB_LOST_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_MASTER_DIS_SHIFT (11)
#define I2CIP_TX_ABRT_SOURCE_ABRT_MASTER_DIS_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_MASTER_DIS_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_SHIFT (10)
#define I2CIP_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_SHIFT (9)
#define I2CIP_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_SHIFT (8)
#define I2CIP_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_SHIFT (7)
#define I2CIP_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_HS_ACKDET_SHIFT (6)
#define I2CIP_TX_ABRT_SOURCE_ABRT_HS_ACKDET_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_HS_ACKDET_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_GCALL_READ_SHIFT (5)
#define I2CIP_TX_ABRT_SOURCE_ABRT_GCALL_READ_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_GCALL_READ_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_SHIFT (4)
#define I2CIP_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_SHIFT (3)
#define I2CIP_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_SHIFT (2)
#define I2CIP_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_SHIFT (1)
#define I2CIP_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_SHIFT)
#define I2CIP_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_SHIFT (0)
#define I2CIP_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_MASK ((0x1)<<I2CIP_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_SHIFT)

/* i2c dma control register */
#define I2CIP_DMA_CR_REG_OFFSET 0x88
#define I2CIP_DMA_CR_TDMAE_SHIFT (1)
#define I2CIP_DMA_CR_TDMAE_MASK ((0x1)<<I2CIP_DMA_CR_TDMAE_SHIFT)
#define I2CIP_DMA_CR_RDMAE_SHIFT (0)
#define I2CIP_DMA_CR_RDMAE_MASK ((0x1)<<I2CIP_DMA_CR_RDMAE_SHIFT)

/* i2c tx dma transfer threshold register */
#define I2CIP_DMA_TX_TL_REG_OFFSET 0x8c
#define I2CIP_DMA_TX_TL_DEPTH (I2CIP_TX_FIFO_DEPTH)
#define I2CIP_DMA_TX_TL_HALF (I2CIP_DMA_TX_TL_DEPTH/2)
#define I2CIP_DMA_TX_TL_1_BYTE (1)

/* i2c rx dma transfer threshold register */
#define I2CIP_DMA_RX_TL_REG_OFFSET 0x90
#define I2CIP_DMA_RX_TL_DEPTH (I2CIP_RX_FIFO_DEPTH)
#define I2CIP_DMA_RX_TL_HALF (I2CIP_DMA_RX_TL_DEPTH/2)
#define I2CIP_DMA_RX_TL_1_BYTE (1)

#define I2CIP_IC_ENABLE_STATUS_REG_OFFSET 0x9C
#define I2CIP_STATUS_IC_EN (1 << 0)
#define I2CIP_STATUS_SLV_RX_ABORTED (1 << 1)
#define I2CIP_STATUS_SLV_FIFO_FILLED_AND_FLUSHED (1 << 2)

#define I2CIP_IC_FS_SPKLEN_REG_OFFSET 0xA0
#define I2CIP_IC_FS_SPKLEN_SHIFT 0
#define I2CIP_IC_FS_SPKLEN_MASK (0xFF << I2CIP_IC_FS_SPKLEN_SHIFT)
#define I2CIP_IC_FS_SPKLEN(n) BITFIELD_VAL(I2CIP_IC_FS_SPKLEN, n)

#define I2CIP_IC_HS_SPKLEN_REG_OFFSET 0xA4
#define I2CIP_IC_HS_SPKLEN_SHIFT 0
#define I2CIP_IC_HS_SPKLEN_MASK (0xFF << I2CIP_IC_FS_SPKLEN_SHIFT)
#define I2CIP_IC_HS_SPKLEN(n) BITFIELD_VAL(I2CIP_IC_FS_SPKLEN, n)

#define I2CIP_IC_VAD_PATH_REG_OFFSET 0xA8
#define I2CIP_IC_PUSH_DATA_BYPASS (1 << 0)

/* Speed Selection */
#define IC_SPEED_MODE_STANDARD  1
#define IC_SPEED_MODE_FAST  2
#define IC_SPEED_MODE_MAX   3

#define I2C_MAX_SPEED       3400000
#define I2C_FSP_SPEED       1000000
#define I2C_FAST_SPEED      400000
#define I2C_STANDARD_SPEED  100000

#endif /* __REG_I2CIP_H_ */
