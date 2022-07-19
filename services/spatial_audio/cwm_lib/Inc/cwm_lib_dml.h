#ifndef __CWM_LIB_DML_H__
#define __CWM_LIB_DML_H__

#define SCL_DML_DRV_ENABLE              400
#define SCL_DML_DRV_INIT                401
#define SCL_DML_DRV_HW_CONFIG           402
#define SCL_DML_DRV_AG_CONFIG           403
#define SCL_DML_DRV_M_CONFIG            404
#define SCL_DML_DEBUG                   405
#define SCL_DML_GET_LIB_INFO            406

int CWM_Dml_LibInit(void);
int CWM_Dml_process(void);

#endif
