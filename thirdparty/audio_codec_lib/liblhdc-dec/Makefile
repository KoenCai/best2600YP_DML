cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))
# -DHAVE_TEST_KEY

obj_s := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.S))
obj_s += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.s))
obj_c := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)dec_src/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)dec_src/lhdc/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)dec_src/llac/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.c))
obj_cpp := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.cpp))

LHDC_LIB_NAME := liblhdc-dec-best2500p_ibrt

$(LHDC_LIB_NAME)-y := $(obj_c:.c=.o) $(obj_s:.S=.o) $(obj_cpp:.cpp=.o)

obj-y += $(LHDC_LIB_NAME).a

#CFLAGS_dec.o += -lc -O3 -Wchar-subscripts -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant
#CFLAGS_cirbuf.o += -lc -O3 -Wchar-subscripts -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant
#CFLAGS_lhdcUtil.o += -lc -O3 -Wchar-subscripts -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant
#CFLAGS_encdec.o += -lc -O3 -Wchar-subscripts -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant
#CFLAGS_lic_check.o += -lc -O3 -Wchar-subscripts -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant
#CFLAGS_licdecrypt.o += -lc -O3 -Wchar-subscripts -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant
ccflags-y += -lc -O3 -Wchar-subscripts -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant $(DEF)

subdir-ccflags-y += \
	-Iservices/tws/inc \
	-Iutils/heap \
	-Iutils/cqueue \
	-Iapps/audioplayers \
	-Iservices/bt_app \
	-Iplatform \
	-Iplatform/hal \
    -Ithirdparty/audio_codec_lib/liblhdc-dec/dec_src/lhdc \
    -Ithirdparty/audio_codec_lib/liblhdc-dec/dec_src/llac \
    -Ithirdparty/audio_codec_lib/liblhdc-dec/dec_src \
	-Ithirdparty/audio_codec_lib/liblhdc-dec \
	-Iservices/ble_stack/ke/api \
	-Iservices/ble_stack/ble_ip \
	$(BT_PROFILES_INCLUDES) \
	-Iservices/nvrecord \
	-Iservices/bt_if_enhanced/inc \
	-Iconfig \
	-Iinclude/cmsis_dsp \
	-Iservices/ble_stack/hl/api
