CHIP		?= best1501

DEBUG		?= 0

FPGA		?= 0

RTOS		?= 1

WATCHER_DOG ?= 1

FLASH_CHIP	?= ALL
# GD25Q80C
# GD25Q32C
#GD25LQ32C
#EN25S80B
# ALL

IS_BEST1402 ?= 0

ifeq ($(IS_BEST1402),1)
CHIP_SUBTYPE	?= best1402
endif

ifeq ($(CHIP),best2300a)
export BT_ECC_CONFIG_BLK ?= 2
endif

ifeq ($(DEBUG),1)
ifneq ($(filter best1400 best2300 best2300p,$(CHIP)),)
export OTA_BOOT_SIZE ?= 0x20000
else
export OTA_BOOT_SIZE ?= 0x28000
endif
else
ifneq ($(filter best1400 best2300 best2300p,$(CHIP)),)
export OTA_BOOT_SIZE ?= 0x18000
else
export OTA_BOOT_SIZE ?= 0x22000
endif
endif

export REL_TRACE_ENABLE ?= $(DEBUG)

export OTA_PROGRAMMER ?= 1

export BES_OTA_BOOT ?= 1

FLASH_SIZE ?= 0x800000

export NEW_IMAGE_FLASH_OFFSET ?= $(FLASH_SIZE)/2

OTA_DUAL_BOOT ?= 1

GEN_CRC16 ?= 0

AUDIO_OUTPUT_MONO ?= 0

AUDIO_OUTPUT_DIFF ?= 0

HW_FIR_EQ_PROCESS ?= 0

SW_IIR_EQ_PROCESS ?= 0

AUDIO_RESAMPLE ?= 1

AUDIO_OUTPUT_VOLUME_DEFAULT ?= 10
# range:1~16

AUDIO_INPUT_CAPLESSMODE ?= 0

AUDIO_INPUT_LARGEGAIN ?= 0

AUDIO_CODEC_ASYNC_CLOSE ?= 0

AUDIO_SCO_BTPCM_CHANNEL ?= 1

AUDIO_EQ_PROCESS ?= 0

SBC_FUNC_IN_ROM ?= 0

ROM_UTILS_ON ?= 0

VOICE_DETECT ?= 0

VOICE_PROMPT ?= 0

VOICE_RECOGNITION ?= 0

BLE ?= 1

BTADDR_GEN ?= 1

BT_ONE_BRING_TWO ?= 0

A2DP_AAC_ON ?= 0

FACTORY_MODE ?= 1

ENGINEER_MODE ?= 1

ULTRA_LOW_POWER	?= 1

DAC_CLASSG_ENABLE ?= 1

NO_SLEEP ?= 1

DSP_LIB ?= 0

AI_VOICE ?= 0

export TX_IQ_CAL := 0

export POWER_MODE	?= DIG_DCDC
export DUMP_NORMAL_LOG ?= 0
export DUMP_CRASH_LOG ?= 0

export ANC_APP ?= 0
export FAULT_DUMP := 0
export SPEECH_CODEC ?= 0

export FLASH_SUSPEND ?= 1
export NO_OVERLAY := 1

export VCRYSTAL_OFF ?= 1
OSC_26M_X4_AUD2BB ?= 1

export SW_PLAYBACK_RESAMPLE ?= 0
export RESAMPLE_ANY_SAMPLE_RATE ?= 0

export BT_XTAL_SYNC ?= 1

export TRACE_BAUD_RATE := 10*115200

init-y		:=
core-y		:= tests/ota_boot/  platform/cmsis/ platform/hal/ platform/drivers/ utils/cqueue/ utils/crc32/ \
				utils/crc16/ utils/heap/ services/nv_section/factory_section/ services/nvrecord/  \
				services/bt_profiles_enhanced/ services/bt_if_enhanced/ services/osif/ utils/intersyshci/ services/bt_app/

KBUILD_CPPFLAGS += -Iplatform/cmsis/inc  -Iplatform/hal -Iservices/bt_profiles_enhanced/inc/ -Iservices/bt_if_enhanced/inc/

KBUILD_CPPFLAGS += \
    -D_BEST1000_QUAL_DCDC_ \
    -D__APP_KEY_FN_STYLE_A__

#    -D__PC_CMD_UART__

ifeq ($(OTA_DUAL_BOOT),1)
KBUILD_CPPFLAGS += -DDUAL_IMAGE_COPY_ENABLED=1
else
KBUILD_CPPFLAGS += -DDUAL_IMAGE_COPY_ENABLED=0
endif

ifeq ($(BES_OTA_BOOT),1)
KBUILD_CPPFLAGS += -DBES_OTA_BOOT
endif

ifeq ($(CHIP),best1501)
# enabled when 1501 flash remap is ready
export OTA_REBOOT_FLASH_REMAP ?= 0
else
export OTA_REBOOT_FLASH_REMAP ?= 0
endif

ifeq ($(OTA_REBOOT_FLASH_REMAP),1)
KBUILD_CPPFLAGS += -DOTA_REBOOT_FLASH_REMAP
endif # REMAP

OTA_BOOT_FAST_COPY ?= 1
ifeq ($(OTA_BOOT_FAST_COPY),1)
KBUILD_CPPFLAGS += -D_OTA_BOOT_FAST_COPY_ 
OSC_26M_X4_AUD2BB := 0
ULTRA_LOW_POWER := 0
endif

ifeq ($(GEN_CRC16),1)
KBUILD_CPPFLAGS += -D__CRC16__
endif

KBUILD_CPPFLAGS += -D__BES_OTA_MODE__=1
KBUILD_CPPFLAGS += -DDATA_ACK_FOR_SPP_DATAPATH_ENABLED=0
KBUILD_CPPFLAGS += -D__BOOT_SELECT
KBUILD_CPPFLAGS += -D__APP_IMAGE_FLASH_OFFSET__=$(OTA_BOOT_SIZE)
KBUILD_CPPFLAGS += -DDEBUG_PORT=1
KBUILD_CPPFLAGS += -DNEW_IMAGE_FLASH_OFFSET=$(NEW_IMAGE_FLASH_OFFSET)

KBUILD_CPPFLAGS += -DDATA_ACK_FOR_SPP_DATAPATH_ENABLED=0

KBUILD_CFLAGS	+= -Os

LDS_FILE    := best1000.lds

export TRACE_BUF_SIZE ?= 32*1024

KBUILD_CPPFLAGS +=

KBUILD_CFLAGS +=

LIB_LDFLAGS += -lstdc++ -lsupc++

#CFLAGS_IMAGE += -u _printf_float -u _scanf_float
