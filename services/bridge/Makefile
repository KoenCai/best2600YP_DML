
cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj_c := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)bridge_v2.c))

BTBRIDGE_LIB_NAME := $(CHIP)_libbridge

ifeq ($(HFP_1_6_ENABLE), 1)
BTBRIDGE_LIB_NAME := $(BTBRIDGE_LIB_NAME)_sbc_enc
endif

ifeq ($(USE_TRACE_ID), 1)
BTBRIDGE_LIB_NAME := $(BTBRIDGE_LIB_NAME)_log_compressed
endif

$(BTBRIDGE_LIB_NAME)-y := $(obj_c:.c=.o)

obj-y := $(BTBRIDGE_LIB_NAME).a

PROF_INC :=


subdir-ccflags-y += \
	-Iservices/bridge/ \
	-Iutils/intersyshci/ \
	-Iplatform/drivers/bt/ \
	-Iservices/bt_app/ \
	-Iservices/osif/ \
	$(BT_IF_INCLUDES) \
	$(BT_PROFILES_INCLUDES) \
    -Iservices/ble_stack_v2/ip/ahi/api/ \
    -Iservices/ble_stack_v2/ip/ble/hl/api/ \
    -Iservices/ble_stack_v2/ip/ble/hl/inc/ \
    -Iservices/ble_stack_v2/ip/ble/hl/src/gap/gapc/ \
    -Iservices/ble_stack_v2/ip/ble/hl/src/gap/gapm/ \
    -Iservices/ble_stack_v2/ip/ble/hl/src/gap/ \
    -Iservices/ble_stack_v2/ip/ble/hl/src/gatt/ \
    -Iservices/ble_stack_v2/ip/ble/hl/src/inc/ \
    -Iservices/ble_stack_v2/ip/ble/hl/src/l2cap/ \
    -Iservices/ble_stack_v2/ip/hci/api/ \
    -Iservices/ble_stack_v2/ip/hci/src/ \
    -Iservices/ble_stack_v2/modules/aes/api/ \
    -Iservices/ble_stack_v2/modules/common/api/ \
    -Iservices/ble_stack_v2/modules/ecc_p256/api/ \
    -Iservices/ble_stack_v2/itf/ke/ \
    -Iservices/ble_stack_v2/itf/rwip/ \
    -Iservices/ble_stack_v2/itf/common/ \
    -Iservices/ble_stack_v2/itf/config/


