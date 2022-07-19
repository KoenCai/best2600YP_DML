/**
 * @file ecc_p192.h
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

#ifndef ECC_P192_H_
#define ECC_P192_H_


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void ecc_gen_new_secret_key_192(uint8_t* secret_key192);
void ecc_gen_new_public_key_192(uint8_t* secret_key,uint8_t* out_public_key);

#ifdef __cplusplus
}
#endif


#endif /* ECC_P192_H_ */
