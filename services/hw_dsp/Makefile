
cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj_s 	:= $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.S))
obj_c 	:= $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.c))
obj_cpp := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.cpp))

obj_c 	+= $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)src/*.c))
obj_cpp += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)src/*.cpp))

ifeq ($(HW_FILTER_CODEC_IIR),1)
ifeq ($(ANC_APP),1)
$(error "error: ANC_APP and HW_FILTER_CODEC_IIR can't be used in the same")
endif
ifeq ($(HW_DAC_IIR_EQ_PROCESS),1)
$(error "error: HW_DAC_IIR_EQ_PROCESS and HW_FILTER_CODEC_IIR can't be used in the same")
endif
endif


obj-y := $(obj_c:.c=.o) $(obj_s:.S=.o) $(obj_cpp:.cpp=.o)

ccflags-y := \
    -Iservices/hw_dsp/inc \
	-Iservices/multimedia/audio/process/psap/include \
	-Iservices/nv_section/aud_section \
	-Iservices/nv_section/include \
	-Iservices/multimedia/audio/process/filters/include

ifeq ($(AUDIO_HW_LIMITER),1)
ccflags-y += -D__AUDIO_HW_LIMITER__
endif

ifeq ($(AUDIO_RESAMPLE),1)
ccflags-y += -D__AUDIO_RESAMPLE__
endif
