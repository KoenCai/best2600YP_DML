#ifndef SENSOR_ALGO_H
#define SENSOR_ALGO_H

#ifdef __cplusplus
extern "C" {
#endif

extern int CWM_OS_i2cRead(uint16_t slaveAddr, uint16_t reg, int regLength, uint8_t *readData, int readDataSize, int argument);
extern int CWM_OS_i2cWrite(uint16_t slaveAddr, uint16_t reg, int regLength, uint8_t *writeData, int writeDataSize, int argument);


int imu_init(void);

int cwm_os_i2c_test(void);

void sensor_task_i2c_init(void);

void cwm_lib_init(void);


#ifdef __cplusplus
}
#endif
#endif
