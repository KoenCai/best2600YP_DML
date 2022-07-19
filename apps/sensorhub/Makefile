cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj-y := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.c $(cur_dir)*.cpp $(cur_dir)*.S))

ifeq ($(VAD_IF_TEST),1)
ccflags-y += -DVAD_IF_TEST
endif


ifeq ($(CAPSENSOR_ENABLE), 1)
obj-y += capsensor/
ccflags-y += \
	-Itests/sensor_hub/capsensor \
	-Iservices/ibrt_middleware/inc \
	-DCAPSENSOR_ENABLE
endif


obj-y := $(obj-y:.c=.o)
obj-y := $(obj-y:.cpp=.o)
obj-y := $(obj-y:.S=.o)

ccflags-y += -Iplatform/drivers/sensor_hub -Iutils/cqueue -Iutils/heap -Iplatform/hal \
	-Iplatform/drivers/ana \
	-Itests/sensor_hub \
	-Itests/sensor_hub/capsensor \
	-Itests/sensor_hub/sensor_hub_ai \
	-Iapps/common \
	-Iapps/sensorhub/capsensor \
	-Iservices/audioflinger \
	-Iservices/ibrt_middleware/inc \
	-Iutils/crc32
