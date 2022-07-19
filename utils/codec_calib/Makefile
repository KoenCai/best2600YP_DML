cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj-y := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.c))
obj-y := $(obj-y:.c=.o)

ccflags-y := -Iplatform/hal -Iplatform/cmsis -Iservices/audioflinger \
	-Iplatform/drivers/ana \
	-Iservices/nvrecord \
	-Iservices/bt_if_enhanced/inc \
	-Iutils/hwtimer_list \
	-Iutils/heap 

AUDIO_OUTPUT_ERROR_LARGE_DC ?= 1
ifeq ($(AUDIO_OUTPUT_DC_PROD_TEST),1)
CODEC_DAC_DC_DYN_BUF ?= 1
ccflags-y += -DAUDIO_OUTPUT_DC_PROD_TEST
endif
ifeq ($(CODEC_DAC_DC_DYN_BUF),1)
ccflags-y += -DCODEC_DAC_DC_DYN_BUF
endif
ifeq ($(AUDIO_OUTPUT_SET_LARGE_ANA_DC),1)
ccflags-y += -DAUDIO_OUTPUT_SET_LARGE_ANA_DC
endif
ifeq ($(AUDIO_OUTPUT_ERROR_LARGE_DC),1)
ccflags-y += -DAUDIO_OUTPUT_ERROR_LARGE_DC
endif
ifeq ($(AUDIO_OUTPUT_DC_CALIB_DUAL_CHAN),1)
ccflags-y += -DAUDIO_OUTPUT_DC_CALIB_DUAL_CHAN
endif
ifeq ($(CODEC_DAC_DC_MUTE_TEST),1)
ccflags-y += -DCODEC_DAC_DC_MUTE_TEST
endif
ifeq ($(CODEC_DAC_ON_OFF_TEST),1)
ccflags-y += -DCODEC_DAC_ON_OFF_TEST
endif
