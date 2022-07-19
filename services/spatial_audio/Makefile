cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj-y := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.c $(cur_dir)*.cpp $(cur_dir)*.S))


obj-y += cwm_lib/

obj-y := $(obj-y:.c=.o)
obj-y := $(obj-y:.cpp=.o)
obj-y := $(obj-y:.S=.o)

ccflags-y +=  \
    $(BT_IF_INCLUDES) \
    $(BT_PROFILES_INCLUDES) \
    -Iservices/bt_app \
    -Iservices/bt_app/a2dp_codecs/include \
    -Iservices/audioflinger \
    -Iservices/nvrecord \
    -Iservices/overlay \
    -Iservices/resources \
    -Iservices/audio_process \
    -Iapps/apptester \
    -Iapps/factory \
	-Iapps/common \
	-Iapps/key \
    -Iutils/crc32 \
    -Iplatform/drivers/bt \
    -Iplatform/drivers/ana \
    -Iplatform/drivers/sensor_hub \
    -Iapps/audioplayers/rbplay \
    -Itests/anc_usb \
    -Iapps/anc/inc \
    -Iapps/ota \
    -Ithirdparty/userapi \
	-Ithirdparty/userapi/six_axis_sens_app \
    -Ithirdparty/six_axis_sens_lib/include \
    -Iservices/voicepath \
    -Iservices/voicepath/gsound/gsound_service \
    -Iservices/voicepath/gsound/gsound_target \
    -Iservices/communication \
	-Iservices/bt_app \
    -Iutils/cqueue \
    -Iservices/ai_voice/ama/ama_manager \
    -Iservices/ai_voice/manager \
	-Iservices/multimedia/audio/codec/sbc/inc \
	-Iservices/multimedia/audio/codec/sbc/src/inc \
    -Iservices/interconnection \
    -Iplatform/hal \
    -Iplatform/drivers/sensor_hub \
    -Iplatform/drivers/ana \
    -Iutils/hwtimer_list \
	-Iservices/bt_spp \
	-Iservices/spatial_audio \
	-Iutils/heap \
    -Iservices/spatial_audio/cwm_lib/Inc \

ifeq ($(TOTA_v2),1)
ccflags-y +=  \
    -Iservices/tota_v2
else
ccflags-y +=  \
    -Iservices/tota
endif