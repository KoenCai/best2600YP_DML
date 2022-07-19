cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))
obj-y := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)src/*.c $(cur_dir)src/*.cpp $(cur_dir)src/*.S))
obj-y := $(obj-y:.c=.o)
obj-y := $(obj-y:.cpp=.o)
obj-y := $(obj-y:.S=.o)


BODYSENSE_LIB_NAME := CWM_LIB_gcc_m33
$(BODYSENSE_LIB_NAME)-y := $(obj_c:.c=.o) $(obj_s:.S=.o) $(obj_cpp:.cpp=.o)
BODYSENSE_DML_LIB_NAME := CWM_LIB_DML_gcc_m33
$(BODYSENSE_DML_LIB_NAME)-y := $(obj_c:.c=.o) $(obj_s:.S=.o) $(obj_cpp:.cpp=.o)

LIB_BIN_DIR := lib/thirdparty/sixaxis
obj-y += $(BODYSENSE_LIB_NAME).a
obj-y += $(BODYSENSE_DML_LIB_NAME).a
ccflags-y += \
		-Wchar-subscripts -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant
ccflags-y +=  -lc -O3 -Wchar-subscripts -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant

subdir-ccflags-y += \
		-Iservices/spatial_audio/cwm_lib/Inc \
		-Iservices/spatial_audio/cwm_lib/lib /