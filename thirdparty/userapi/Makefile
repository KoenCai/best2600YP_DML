cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj-y := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.c $(cur_dir)*.cpp $(cur_dir)*.S))
obj-y := $(obj-y:.c=.o)
obj-y := $(obj-y:.cpp=.o)
obj-y := $(obj-y:.S=.o)

obj-y += $(foreach lib,$(THIRDPARTY_LIB),$(lib)_app/)

subdir-ccflags-y +=  \
	-Iutils/list \
	-Iservices/app_ai/inc \
	-Iservices/ai_voice


