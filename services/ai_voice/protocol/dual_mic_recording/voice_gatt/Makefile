cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj_s := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.S))
obj_c := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.c))
obj_cpp := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.cpp))

obj-y := $(obj_c:.c=.o) $(obj_s:.S=.o) $(obj_cpp:.cpp=.o)

subdir-ccflags-y += \
	-Iservices/ble_stack/hl/api  \
	-Iservices/ble_stack/hl/inc  \
	-Iservices/ble_stack/ble_ip/  \
	-Iservices/ble_stack/common/api  \
	-Iservices/ble_stack/ke/api  \
	-Iservices/ai_voice/manager \
	-Iservices/ai_voice/transport \
	-Iservices/app_ai/inc \
	$(BT_IF_INCLUDES) \
	-Iservices/bt_app