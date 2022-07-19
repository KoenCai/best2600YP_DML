cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/ahi/src/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/ble/hl/src/gap/gapc/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/ble/hl/src/gap/gapm/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/ble/hl/src/gatt/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/ble/hl/src/l2cap/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/ble/hl/src/rwble_hl/*.c))
ifeq ($(BLE_AUDIO_ENABLED),1)
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/ble/iso/data_path/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/ble/iso/data_path/isogen/src/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/ble/iso/data_path/isoohci/src/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/acc/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/acc/mc/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/acc/ot/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/acc/tb/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/al/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/arc/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/arc/aic/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/arc/mic/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/arc/vc/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/arc/voc/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/atc/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/atc/csi/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/atc/raa/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/bap/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/bap/bc/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/bap/capa/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/bap/codec/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/bap/uc/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/cap/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/gaf/src/iap/*.c))
endif
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)ip/hci/src/*.c))

obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)modules/aes/src/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)modules/common/src/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)modules/ecc_p256/src/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)modules/ke/src/*.c))
obj_c += $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)modules/rwip/src/*.c))

BTBLE_LIB_NAME := $(CHIP)_libble_stack

ifeq ($(HFP_1_6_ENABLE), 1)
BTBLE_LIB_NAME := $(BTBLE_LIB_NAME)_sbc_enc
endif

ifeq ($(USE_TRACE_ID), 1)
BTBLE_LIB_NAME := $(BTBLE_LIB_NAME)_log_compressed
endif

$(BTBLE_LIB_NAME)-y := $(obj_c:.c=.o)
obj-y := $(BTBLE_LIB_NAME).a

subdir-ccflags-y += \
    -Iservices/bt_app \
    -Iservices/bridge/ \
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
    -Iservices/ble_stack_v2/ip/ble/iso/data_path/ \
    -Iservices/ble_stack_v2/ip/ble/iso/data_path/isogen/api/ \
    -Iservices/ble_stack_v2/ip/ble/iso/data_path/isoohci/api/ \
    -Iservices/ble_stack_v2/ip/ble/iso/data_path/isogen/src/ \
    -Iservices/ble_stack_v2/ip/ble/iso/data_path/isoohci/src/ \
    -Iservices/ble_stack_v2/ip/gaf/api/ \
    -Iservices/ble_stack_v2/ip/gaf/api/acc/ \
    -Iservices/ble_stack_v2/ip/gaf/api/acc/mc/ \
    -Iservices/ble_stack_v2/ip/gaf/api/acc/ot/ \
    -Iservices/ble_stack_v2/ip/gaf/api/acc/tb/ \
    -Iservices/ble_stack_v2/ip/gaf/api/acc/vc/ \
    -Iservices/ble_stack_v2/ip/gaf/api/arc/ \
    -Iservices/ble_stack_v2/ip/gaf/api/arc/aic/ \
    -Iservices/ble_stack_v2/ip/gaf/api/arc/mic/ \
    -Iservices/ble_stack_v2/ip/gaf/api/arc/vc/ \
    -Iservices/ble_stack_v2/ip/gaf/api/arc/voc/ \
    -Iservices/ble_stack_v2/ip/gaf/api/atc/ \
    -Iservices/ble_stack_v2/ip/gaf/api/atc/csi/ \
    -Iservices/ble_stack_v2/ip/gaf/api/atc/raa/ \
    -Iservices/ble_stack_v2/ip/gaf/api/bap/ \
    -Iservices/ble_stack_v2/ip/gaf/api/bap/bc/ \
    -Iservices/ble_stack_v2/ip/gaf/api/bap/capa/ \
    -Iservices/ble_stack_v2/ip/gaf/api/bap/uc/ \
    -Iservices/ble_stack_v2/ip/gaf/api/cap/ \
    -Iservices/ble_stack_v2/ip/gaf/api/iap/ \
    -Iservices/ble_stack_v2/ip/gaf/inc/ \
    -Iservices/ble_stack_v2/ip/gaf/src/ \
    -Iservices/ble_stack_v2/ip/gaf/src/al/ \
    -Iservices/ble_stack_v2/ip/gaf/src/arc/ \
    -Iservices/ble_stack_v2/ip/gaf/src/bap/bc/ \
    -Iservices/ble_stack_v2/ip/gaf/src/bap/capa/ \
    -Iservices/ble_stack_v2/ip/gaf/src/bap/codec/ \
    -Iservices/ble_stack_v2/ip/gaf/src/iap/ \
    -Iservices/ble_stack_v2/itf/config/ \
    -Iservices/ble_stack_v2/itf/common/ \
    -Iservices/ble_stack_v2/itf/ble/ \
    -Iservices/ble_stack_v2/itf/ke/ \
    -Iservices/ble_stack_v2/itf/rwip/ \
    -Iplatform/drivers/bt \
    $(BLE_APP_INCLUDES) \
    $(BLE_STACK_INCLUDES) \
	$(BLE_PROFILES_INCLUDES) \
    $(BT_IF_INCLUDES) \
    $(BT_PROFILES_INCLUDES)
