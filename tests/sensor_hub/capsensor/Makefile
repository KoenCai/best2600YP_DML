cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj_s += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.S))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.c))
obj_cpp += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.cpp))

CAP_CALCULATE_LIB_NAME := libcap_calculate
$(CAP_CALCULATE_LIB_NAME)-y := $(obj_c:.c=.o) $(obj_cpp:.cpp=.o)

obj-y += $(CAP_CALCULATE_LIB_NAME).a

ccflags-y +=  \
		-Iapps/key \
		-Iapps/common \
		-Iapps/sensorhub \
		-Itests/sensor_hub/ \
		-Itests/sensor_hub/core \
		-Iplatform/drivers/ana \
		-Iplatform/hal/best1501/ \
		-Iplatform/drivers/sensor_hub \
		-Iutils/hwtimer_list \
		-Iservices/audioflinger \
		-Itests/sensor_hub/capsensor \

ifeq ($(CAPSENSOR_ENABLE),1)
ccflags-y += -DCAPSENSOR_ENABLE
endif
