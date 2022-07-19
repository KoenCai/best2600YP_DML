cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj-y := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.c $(cur_dir)*.cpp $(cur_dir)*.S))
obj-y := $(obj-y:.c=.o)
obj-y := $(obj-y:.cpp=.o)
obj-y := $(obj-y:.S=.o)

ccflags-y += -Iplatform/drivers/sensor_hub -Iutils/cqueue -Iutils/heap -Iplatform/hal \
	-Iplatform/drivers/ana \
	-Itests/sensor_hub/ \
	-Itests/sensor_hub/capsensor \
	-Iapps/common \
	-Iapps/sensorhub \
	-Iapps/main \
	-Iapps/key \
	-Iservices/audioflinger \
	-Iservices/ibrt_middleware/inc \
	-Iutils/crc32 \
	-Iservices/app_ibrt_v2/inc \
	-Iservices/ibrt_core_v2/inc \
	-Iservices/ibrt_ui_v2/inc \
	-Iservices/bt_if_enhanced/inc \
	-Iutils/cqueue \
	-Iservices/multimedia/audio/codec/sbc/inc \
	-Iservices/bt_app \
	-Iplatform/drivers/bt \
	-Iservices/ibrt_middleware/inc

