#include "cmsis_os.h"
#include "cmsis.h"
#include "cqueue.h"
#include "hal_trace.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "hal_timer.h"
#include "hal_trace.h"
#include "hal_i2c.h"
#include "hal_iomux.h"
#include "heap_api.h"

#include "cwm_lib.h"
#include <cwm_lib_dml.h>
#include "cwm_api.h"

#define CWM_API_TEST (0)
#define i2c_mode_task (1)
#define SENS_BUS_ID        (HAL_I2C_ID_1)
#define SENS_I2C_BUS_SPEED_KHZ (400)
#define CWM42607_IIC_ADDR  (0x68)
#define CWM42607_WHOMI  (0x75)
enum HAL_I2C_ID_T sensor_bus_id = SENS_BUS_ID;
#define CMED_MEM_POOL_SIZE (40*1024)
static uint8_t g_cmedMemPool[CMED_MEM_POOL_SIZE] = {0};


#define CWM_DEBUG_EN
#ifdef CWM_DEBUG_EN
#define CWM_DEBUG(num, str, ...) TRACE(num, str, ##__VA_ARGS__)
#else
#define CWM_DEBUG(...)
#endif


int CWM_OS_dbgOutput(const char *format)
{
    CWM_DEBUG(0, "%s", format);
    //    app_demo_spp_send_data_handler((uint8_t*)format,120);
    return 0;
}

int CWM_OS_dbgPrintf(const char *format, ...)
{
    va_list argList;
    int size = 0;
    static char tBuffer[1000];
    memset(tBuffer, 0, 1000);
    va_start(argList, format);
    size = vsnprintf(tBuffer, sizeof(tBuffer), format, argList);
    va_end(argList);
    //strcat(tBuffer, "\n");
    CWM_OS_dbgOutput(tBuffer);
    return size;
}

uint64_t CWM_OS_GetTimeNs(void)
{
#if 0
	uint32_t tick;
	tick = hal_fast_sys_timer_get();
	return FAST_TICKS_TO_US(tick)*1000ULL;
#endif
	uint32_t tick;
	tick = hal_sys_timer_get();
	return (uint64_t)(TICKS_TO_US(tick)) * 1000ULL;
}

static void *CWM_OS_malloc(int size)
{
    return med_malloc(size);
	//return malloc(size);
}

static void CWM_OS_free(void* ptr)
{
    med_free(ptr);
    //free(ptr);
}

void CWM_OS_uSleep(uint32_t time)
{
	hal_sys_timer_delay_us(time);
}

static void sensor_i2c_bus_init(enum HAL_I2C_ID_T id, uint8_t addr, int speed_khz)
{
	uint32_t ret;

	struct HAL_I2C_CONFIG_T cfg;
    memset(&cfg, 0, sizeof(cfg));
	#if (i2c_mode_task)
    cfg.mode = HAL_I2C_API_MODE_TASK;
	CWM_OS_dbgPrintf("HAL_I2C_API_MODE_TASK.\n"); 
	#else
	cfg.mode = HAL_I2C_API_MODE_SIMPLE;
	CWM_OS_dbgPrintf("HAL_I2C_API_MODE_SIMPLE.\n"); 
	#endif
	cfg.use_dma = 0;
    cfg.use_sync = 1;
    cfg.speed = speed_khz * 1000;
    cfg.as_master = 1;

    ret = hal_i2c_open(id, &cfg);
	if (ret)         
		CWM_OS_dbgPrintf("hal_i2c0_open failed, ret = %d.\n",ret);    
}


void heap_init(void)
{
	med_heap_init(&g_cmedMemPool[0],CMED_MEM_POOL_SIZE);
}

void sensor_task_i2c_init(void)
{
	CWM_OS_dbgPrintf("i2c init");
	hal_iomux_set_i2c1();
	CWM_OS_dbgPrintf("i2c1 open ok.\n");  
	sensor_i2c_bus_init(SENS_BUS_ID,CWM42607_IIC_ADDR,SENS_I2C_BUS_SPEED_KHZ);
}

int CWM_OS_i2cRead(uint16_t slaveAddr, uint16_t reg, int regLength, uint8_t *readData, int readDataSize, int argument)
{
#if (i2c_mode_task)
	uint8_t i2c_read_buf[2] = { 0 };
	i2c_read_buf[0] = reg;
	return hal_i2c_task_mrecv(SENS_BUS_ID, slaveAddr, i2c_read_buf , 1, readData, readDataSize,1, 0, NULL);

	
#else
	 int ret = 0;
	uint8_t read_buf[4] = {0};
    if (readDataSize > 1024)
        readDataSize = 1024;
    read_buf[0] = reg;
    ret = hal_i2c_simple_recv(SENS_BUS_ID, slaveAddr, read_buf, 1, readData, readDataSize);
    if (ret) {
        CWM_OS_dbgPrintf( " read failed %d", ret);
        return ret;
    }
    return ret;

#endif

}


int CWM_OS_i2cWrite(uint16_t slaveAddr, uint16_t reg, int regLength, uint8_t *writeData, int writeDataSize, int argument)
{
#if (i2c_mode_task)
	uint8_t write_buf[4]={0};
	write_buf[0] =reg;
	memcpy(&write_buf[1], writeData, writeDataSize);
	return hal_i2c_task_msend(SENS_BUS_ID, slaveAddr, write_buf, (writeDataSize+1), 1, 0,NULL);	
#else
	int ret = 0;
	uint32_t txlen = 0;
	uint32_t i = 0;
	uint8_t wr_txbuf[4] = {0};
    if (writeDataSize > 1024)
        writeDataSize = 1024;
    wr_txbuf[0] = reg;
    txlen += 1;

    for(i = 0; i < writeDataSize; i++)
    {
        wr_txbuf[i+1] = writeData[i];
    }
    txlen += writeDataSize;
  
    ret = hal_i2c_simple_send(SENS_BUS_ID, slaveAddr, wr_txbuf, (uint16_t)txlen);
    if (ret) {
        CWM_OS_dbgPrintf( "write  failed %d",  ret);
        return ret;
    }
    return ret;
#endif
}

void test_GetTimeNs_1() 
{    
	uint64_t timeNow, timeLast, timeDiff;    
	CWM_OS_dbgPrintf("[test_GetTimeNs_1] test 10 seconds start\r\n");    
	timeLast = CWM_OS_GetTimeNs();    
	while (1) {        
		timeNow = CWM_OS_GetTimeNs();        
		timeDiff = timeNow - timeLast;        
		//CWM_OS_dbgPrintf("timeNow=%lld\ttimeLast=%lld\ttimeDiff=%lld\n", timeNow, timeLast, timeDiff);        
		if (timeDiff >= (uint64_t)10 * 1000000000) {            
			CWM_OS_dbgPrintf("[test_GetTimeNs_1] test 10 seconds end\r\n");            
			break;        
			}    
		}
}
void test_GetTimeNs_2() 
{    
	uint64_t timeNow, timeLast, timeDiff;    
	uint64_t count;    
	CWM_OS_dbgPrintf("[test_GetTimeNs_2] test 10 seconds start\r\n");    
	timeLast = CWM_OS_GetTimeNs();    
	count = 0;    
	while (1) {        
		timeNow = CWM_OS_GetTimeNs();        
		timeDiff = timeNow - timeLast;        
		//CWM_OS_dbgPrintf("timeNow=%lld\ttimeLast=%lld\ttimeDiff=%lld\n", timeNow, timeLast, timeDiff);        
		if (timeDiff >= (uint64_t)1 * 1000000) {            
			count ++;            
			//CWM_OS_dbgPrintf("count=%lld\n", count);            
			timeLast = timeNow;            
			if (count >= (uint64_t)10 * 1000) {                
				CWM_OS_dbgPrintf("[test_GetTimeNs_2] test 10 seconds end\r\n");                
				break;            
				}        
			}    
		}
}

int test_malloc_1() {
    char *pBuf1;
    int alloc_size;
    char buf1[0x100];

    CWM_OS_dbgPrintf("[test_malloc_1] start.\r\n");

    alloc_size = 10 * 1024;
    pBuf1 = (char*)CWM_OS_malloc(alloc_size);
    if (pBuf1 == NULL)
        goto __FAIL;

    memset(pBuf1, 0x7a, alloc_size);
    if (pBuf1[0] != 0x7a)
        goto __FAIL;

    if (pBuf1[alloc_size - 1] != 0x7a)
        goto __FAIL;

    strcpy(pBuf1, "success!!!!");

    sprintf(buf1, "[test_malloc_1] end. %s\r\n", pBuf1);
    CWM_OS_dbgPrintf(buf1);

    return 1;

__FAIL:
    CWM_OS_dbgPrintf("[test_malloc_1] end. failure!!!!\r\n");

    return 0;
}

int test_free_1() {
    char *pBuf1;
    char *pBuf2;
    int alloc_size;

    CWM_OS_dbgPrintf("[test_free_1] start.\r\n");

    alloc_size = 16;
    pBuf1 = (char*)CWM_OS_malloc(alloc_size);
    if(pBuf1 == NULL)
        goto __FAIL;

    //In none-OS or single thread system, if we use malloc() to acquire a memory block and free it immediately,
    //the next malloc() would get same memory block address
    //THIS IS A VERY ROUGHLY TEST
    CWM_OS_free(pBuf1);

    pBuf2 = (char*)CWM_OS_malloc(alloc_size);
    if(pBuf2 == NULL)
        goto __FAIL;

    if(pBuf1 == pBuf2)
        CWM_OS_dbgPrintf("[test_free_1] end. success!!!\r\n");
    else
        goto __FAIL;

    return 1;
__FAIL:
    CWM_OS_dbgPrintf("[test_free_1] end. failure!!!!\r\n");

    return 0;
}



void test_GetTimeNs_3() 
{
    uint64_t timeNow, timeLast, timeDiff;
    CWM_OS_dbgPrintf("[test_GetTimeNs_3] test 20 minutes start\r\n");
    timeLast = CWM_OS_GetTimeNs();
    while (1) {
        timeNow = CWM_OS_GetTimeNs();
        timeDiff = timeNow - timeLast;
        if (timeDiff >= (uint64_t)20 * 60 * 1000000000) {
            CWM_OS_dbgPrintf("[test_GetTimeNs_3] test 20 minutes end\r\n");
            break;
			}
		}
}

void test_uSleep_1() {

    CWM_OS_dbgPrintf("[test_uSleep_1] test 10 seconds start\r\n");

    CWM_OS_uSleep(10 * 1000000);

    CWM_OS_dbgPrintf("[test_uSleep_1] test 10 seconds end\r\n");
}

void test_uSleep_2() {
    int i, j;

    CWM_OS_dbgPrintf("[test_uSleep_2] test 10 seconds start\r\n");

    for (j = 0; j < 10; j ++) {
        for (i = 0; i < 1000; i++) {
            CWM_OS_uSleep(1000);
        }
    }

    CWM_OS_dbgPrintf("[test_uSleep_2] test 10 seconds end\r\n");
}

void test_uSleep_3() {
    int i, j, k;

    CWM_OS_dbgPrintf("[test_uSleep_3] test 10 seconds start\r\n");

    for (j = 0; j < 10; j ++) {
        for (i = 0; i < 1000; i ++) {
            for (k = 0; k < 1000; k ++) {
                CWM_OS_uSleep(1);
            }
        }
    }

    CWM_OS_dbgPrintf("[test_uSleep_3] test 10 seconds end\r\n");
}

void cwm_api_test()
{
	test_GetTimeNs_1(); // 10 seconds    
	test_GetTimeNs_2(); // 10 seconds, accurate to 1ms
	//test_GetTimeNs_3() ;// 20 minutes
	test_malloc_1();    
	test_free_1();
    test_uSleep_1(); // 10 seconds    
    test_uSleep_2(); // 10 seconds, accurate to 1ms    
    test_uSleep_3(); // 10 seconds, accurate to 1us	

}

int cwm_os_i2c_test(void)
{
	int rc = 0;
	uint8_t rbuffer[2] = {0};
	rc = CWM_OS_i2cRead(CWM42607_IIC_ADDR,CWM42607_WHOMI,1,rbuffer,1,1);
	CWM_OS_dbgPrintf("whoami = 0x%02x\n",rbuffer[0]);
	return rc;
}




void CWM_AP_SensorListen(pSensorEVT_t sensorEVT) {
    switch (sensorEVT->sensorType) {
        case 100:
            CWM_OS_dbgPrintf("orientation: yaw=%d, pitch=%d, roll=%d\n",
                                                       (int)(sensorEVT->fData[0] * 1000),
                                                        (int)(sensorEVT->fData[1] * 1000),
                                                       (int)(sensorEVT->fData[2] * 1000)
                                                        );
            break;

		case 102:
			CWM_OS_dbgPrintf("\n/***************************************************************/\n");
			CWM_OS_dbgPrintf("trigger counter=%d, TAP_TYPE=%d\n",														
														(int)(sensorEVT->fData[1] ),
														(int)(sensorEVT->fData[2] )
														);
			CWM_OS_dbgPrintf("\n/***************************************************************/\n");
			break;
        default:
            CWM_OS_dbgPrintf("IDX[%d]: %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n", sensorEVT->sensorType,
                                                        (int)(sensorEVT->fData[0] * 1000),
                                                        (int)(sensorEVT->fData[1] * 1000),
                                                        (int)(sensorEVT->fData[2] * 1000),
                                                        (int)(sensorEVT->fData[3] * 1000),
                                                        (int)(sensorEVT->fData[4] * 1000),
                                                        (int)(sensorEVT->fData[5] * 1000),
                                                        (int)(sensorEVT->fData[6] * 1000),
                                                        (int)(sensorEVT->fData[7] * 1000)
                                                        );
    }
}

void cwm_lib_init(void)
{
		SettingControl_t scl;
	
		OsAPI device_func = 
		{
			.malloc 	 = CWM_OS_malloc,
			.free		 = CWM_OS_free,
			.GetTimeNs	 = CWM_OS_GetTimeNs,
			.dbgOutput	 = CWM_OS_dbgOutput,
			.i2cRead	 = CWM_OS_i2cRead,
			.i2cWrite	 = CWM_OS_i2cWrite,
			.uSleep 	 = CWM_OS_uSleep,
		};
	
		//get lib version information
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		CWM_SettingControl(SCL_GET_LIB_INFO, &scl);
		CWM_OS_dbgPrintf("version:%d.%d.%d.%d product:%d\n", scl.iData[1], scl.iData[2], scl.iData[3], scl.iData[4], scl.iData[5]);
		
		CWM_LibPreInit(&device_func);
	
		//set mcu_chip information, must call this before CWM_LibPostInit()
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[1] = 2; // 0: mcu_auto_detect 2: skip_mcu_auto_detect
		//scl.iData[2] = 110020000;
		CWM_SettingControl(SCL_CHIP_VENDOR_CONFIG, &scl);
	
		// Init CWM lib
		CWM_LibPostInit(CWM_AP_SensorListen);
	
		//get chip information
		char chipInfo[64];
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[1] = 1;
		scl.iData[2] = (int)chipInfo;
		scl.iData[3] = sizeof(chipInfo);
		scl.iData[4] = 0;
		scl.iData[5] = 0;
		scl.iData[6] = 0;
		CWM_SettingControl(SCL_GET_CHIP_INFO, &scl);
		CWM_OS_dbgPrintf("have_security = %d.%d ret_buff_size = %d	chipInfo = %s\n", scl.iData[5], scl.iData[6], scl.iData[4], chipInfo);
		CWM_OS_dbgPrintf("chip_settings = %d, %d, %d\n", scl.iData[9], scl.iData[10], scl.iData[11]);
		if (scl.iData[5] == 1)
			CWM_OS_dbgPrintf("verify security_code Pass\n");
		else
			CWM_OS_dbgPrintf("verify security_code Fail\n");
	#if 1 // log control
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[3] = 3;	//3;
		scl.iData[4] = 5+64;	// 5 + 64; 
		scl.iData[5] = 3+16;
		scl.iData[6] = 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384;
		CWM_SettingControl(SCL_LOG, &scl);	
    #endif
	
		//get request sensor
		//CWM_Sensor_Enable(IDX_REQUEST_SENSOR);
		
    #ifndef USE_DML
		CWM_Dml_LibInit();
	
		//get dml lib version
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		CWM_SettingControl(SCL_DML_GET_LIB_INFO, &scl);
		CWM_OS_dbgPrintf("DML version:%d.%d.%d.%d product:%d\n\n", scl.iData[1], scl.iData[2], scl.iData[3], scl.iData[4], scl.iData[5]);

		//dml lib find sensor device 
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[1] = 1;
		scl.iData[2] = 1+4; 		//1+4 68 1+8 69
		CWM_SettingControl(SCL_DML_DRV_HW_CONFIG, &scl);

	    //dml lib setting sensor
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[3] = 2; // 1:poll 2:fifo
		scl.iData[4] = 1; // 1:any 2:dt 3:custom
		//scl.iData[5] = 20000; // custom_data_period (default: 0)
		scl.iData[6] = 1; //fifo count
		scl.iData[7] = 1; //axis
		scl.iData[8] = 50; //odr (default: 50)
		scl.iData[9] = 16; //acc range (default: 16)
		scl.iData[10] = 2000; // gyro range (default: 2000)
		scl.iData[11] = 0; // int1	1:data_ready 2:any_motion 4:watermark
		scl.iData[12] = 0; // int2	1:data_ready 2:any_motion 4:watermark
		CWM_SettingControl(SCL_DML_DRV_AG_CONFIG, &scl);

	#if 0 //dml debug log		
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[1] = 5;  //32: DGD
		CWM_SettingControl(SCL_DML_DEBUG, &scl);	
	#endif

		//dml lib process type
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[1] = 0; // 2:proce 3:proc2
		CWM_SettingControl(SCL_DML_DRV_INIT, &scl); 				
		CWM_OS_dbgPrintf("============================test i2c!!===============================");
		CWM_OS_dbgPrintf("found device=%d\n", scl.iData[2]);
		CWM_OS_dbgPrintf("============================end test i2c!!===============================");
	#if 0 //if can't find device, cheak the i2c 

		cwm_os_i2c_test();
	#endif

		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[1] = 1;//manual enable
		scl.iData[2] = 3;//open 1:acc + 2:gyro
		CWM_SettingControl(SCL_DML_DRV_ENABLE, &scl);
		
    #endif
		
    #if 1 // dt calibration
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[1] = 1; // turn on dt calibration (default: 2) (0: default, 1: turn_on, 2: turn_off)
		CWM_SettingControl(SCL_INPUT_DT_CONFIG, &scl);
    #endif
	
    #if 0 // headset orient config
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[2] = 0;	// sensitivity (default: 2) (0: default, -1: disable, 1 - 100)
		scl.iData[6] = 1;
		//scl.iData[9] = 52;  // sensor ODR 25/50 default:50
		CWM_SettingControl(SCL_HS_ORIEN_CONFIG, &scl);
    #endif

	#if 0 // Sensor axial config
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[1] = 10; 
	    CWM_SettingControl(SCL_INPUT_SENSOR_CONFIG, &scl);
	#endif	

		CWM_Sensor_Enable(100);
		CWM_Sensor_Enable(102);
		
    #if CWM_API_TEST
		//cwm_api_test();
    #endif



}



static void icm_thread(void const *argument);
osThreadDef(icm_thread, osPriorityNormal, 1, 2048 * 2, "icm_thread");
static osThreadId icm_tid = NULL;
static void icm_thread(void const *argument)
{
	sensor_task_i2c_init();
	heap_init();
	cwm_lib_init();
	CWM_DEBUG(0, "Start processing");

	while(1)
	{	 
		osDelay(20);
		#if CWM_API_TEST
		cwm_api_test();
		#else
		CWM_Dml_process();
		#endif
	}

}

int imu_init(void)
{
	//if(!user_right_ear())
	{
		CWM_DEBUG(1, "%s\r\n", __func__);

		if (icm_tid == NULL) {
			icm_tid = osThreadCreate(osThread(icm_thread), NULL);
		}

		if (icm_tid == NULL) {
			CWM_DEBUG(0, "icm_init Failed to create thread");

			return -1;
		}
    }
    return 0;
}





