platform/cmsis/cmsis_nvic.o: ../../platform/cmsis/cmsis_nvic.c \
 ../../platform/hal/plat_types.h \
 /usr/bin/gcc-arm-none-eabi-10-2020-q4-major/lib/gcc/arm-none-eabi/10.2.1/include/stddef.h \
 /usr/bin/gcc-arm-none-eabi-10-2020-q4-major/lib/gcc/arm-none-eabi/10.2.1/include/stdint.h \
 /usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/stdint.h \
 /usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/machine/_default_types.h \
 /usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/sys/features.h \
 /usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/_newlib_version.h \
 /usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/sys/_intsup.h \
 /usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/sys/_stdint.h \
 /usr/bin/gcc-arm-none-eabi-10-2020-q4-major/lib/gcc/arm-none-eabi/10.2.1/include/stdbool.h \
 ../../platform/hal/plat_addr_map.h \
 ../../platform/hal/best1501/plat_addr_map_best1501.h \
 ../../platform/cmsis/inc/cmsis_nvic.h ../../platform/cmsis/inc/cmsis.h \
 ../../platform/cmsis/inc/best1501.h ../../platform/cmsis/inc/core_cm33.h \
 ../../platform/cmsis/inc/cmsis_version.h \
 ../../platform/cmsis/inc/cmsis_compiler.h \
 ../../platform/cmsis/inc/cmsis_gcc.h \
 ../../platform/cmsis/inc/mpu_armv8.h \
 ../../platform/cmsis/inc/system_ARMCM.h \
 ../../platform/hal/hal_location.h ../../platform/cmsis/inc/mpu.h
../../platform/hal/plat_types.h:
/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/lib/gcc/arm-none-eabi/10.2.1/include/stddef.h:
/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/lib/gcc/arm-none-eabi/10.2.1/include/stdint.h:
/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/stdint.h:
/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/machine/_default_types.h:
/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/sys/features.h:
/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/_newlib_version.h:
/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/sys/_intsup.h:
/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/arm-none-eabi/include/sys/_stdint.h:
/usr/bin/gcc-arm-none-eabi-10-2020-q4-major/lib/gcc/arm-none-eabi/10.2.1/include/stdbool.h:
../../platform/hal/plat_addr_map.h:
../../platform/hal/best1501/plat_addr_map_best1501.h:
../../platform/cmsis/inc/cmsis_nvic.h:
../../platform/cmsis/inc/cmsis.h:
../../platform/cmsis/inc/best1501.h:
../../platform/cmsis/inc/core_cm33.h:
../../platform/cmsis/inc/cmsis_version.h:
../../platform/cmsis/inc/cmsis_compiler.h:
../../platform/cmsis/inc/cmsis_gcc.h:
../../platform/cmsis/inc/mpu_armv8.h:
../../platform/cmsis/inc/system_ARMCM.h:
../../platform/hal/hal_location.h:
../../platform/cmsis/inc/mpu.h:

cmd_platform/cmsis/cmsis_nvic.o := arm-none-eabi-gcc -MD -MP -MF platform/cmsis/.cmsis_nvic.o.d -MT platform/cmsis/cmsis_nvic.o   -I../../platform/cmsis -DBT_SBC_PLAY_DEFAULT_FREQ=0 -DIBRT_CORE_V2_ENABLE -DIBRT_UI_V2  -I../../platform/cmsis/inc  -I../../services/audioflinger  -I../../platform/hal -DAPP_AUDIO_BUFFER_SIZE=150*1024 -DCHARGER_PLUGINOUT_RESET=0 -DBTM_MAX_LINK_NUMS=3 -DBT_DEVICE_NUM=2 -DA2DP_DECODER_VER=2 -D__SW_TRIG__ -DHAL_TRACE_RX_ENABLE -DAUDIO_OUTPUT_DC_AUTO_CALIB -DCODEC_DAC_DC_NV_DATA -D__SNYC_FOUND_CHECK_HECERROR__ -DCHIP_BEST1501 -DCHIP_HAS_UART=3 -DCHIP_HAS_I2C=2 -DCHIP_HAS_USB -DCHIP_HAS_CP -DANC_HW_GAIN_SMOOTHING -DRTOS -DKERNEL_RTX5  -I../../include/rtos/rtx5/ -D__RTX_CPU_STATISTICS__=1 -DDEBUG -U__INT32_TYPE__ -D__INT32_TYPE__=int -U__UINT32_TYPE__ -DINTERSYS_DEBUG=1 -D__2M_PACK__ -D_SCO_BTPCM_CHANNEL_ -DA2DP_PROMPT_PLAY_ONLY_AVRCP_PLAY_RECEIVED -DSPP_SERVICE_NUM=7 -D__A2DP_PLAYER_USE_BT_TRIGGER__ -DSBC_SELECT_CHANNEL_SUPPORT -DHFP_1_6_ENABLE -DA2DP_AAC_ON -D__ACC_FRAGMENT_COMPATIBLE__ -DFDKAAC_VERSION=2 -DA2DP_CP_ACCEL -DSCO_CP_ACCEL -DUSE_CMSIS_F32_FFT -DBT_XTAL_SYNC_NEW_METHOD -DFIXED_BIT_OFFSET_TARGET -DPCM_FAST_MODE -DCVSD_BYPASS -DSCO_DMA_SNAPSHOT -DROM_UTILS_VER=2 -DIS_BES_BATTERY_MANAGER_ENABLED -DSUPPORT_BATTERY_REPORT -DSUPPORT_SIRI -DTWS_PROMPT_SYNC -DIBRT -DIBRT_BLOCKED -DIBRT_NOT_USE -D__A2DP_AUDIO_SYNC_FIX_DIFF_NOPID__ -DTWS_SYSTEM_ENABLED -DIBRT_RIGHT_MASTER -DBES_AUD -DIBRT_V2_MULTIPOINT -DSYSTEM_FREQ_208M=208 -D__FACTORY_MODE_SUPPORT__ -DNEW_NV_RECORD_ENABLED  -I../../services/nv_section/userdata_section -DBESBT_STACK_SIZE=1024*5+512 -DMEDIA_PLAYER_SUPPORT -DAPP_KEY_ENABLE -DBT_ADDR_OUTPUT_PRINT_NUM=6 -DAPP_TWS_CTRL_BUFFER_MAX_LEN=672 -DFLASH_SUSPEND -DMULTIPOINT_DUAL_SLAVE -DSPEECH_TX_DC_FILTER -DSPEECH_TX_AEC2FLOAT -DSPEECH_TX_COMPEXP -DSPEECH_TX_AEC_CODEC_REF -DSPEECH_PROCESS_FRAME_MS=15 -DSPEECH_SCO_FRAME_MS=15 -DSPEECH_CODEC_CAPTURE_CHANNEL_NUM=1 -DSPEECH_ASR_CAPTURE_CHANNEL_NUM= -DAUD_SECTION_STRUCT_VERSION=2 -DAUDIO_OUTPUT_VOLUME_DEFAULT=16 -DLARGE_RAM -DINT_LOCK_EXCEPTION  -I../../config/best2500p_ibrt  -I../../config/_default_cfg_src_ -DUNALIGNED_ACCESS -Werror -mthumb -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -D__WATCHER_DOG_RESET__ -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -fsigned-char -fno-aggressive-loop-optimizations -fno-isolate-erroneous-paths-dereference -fsingle-precision-constant -Wdouble-promotion -Wfloat-conversion -g -O2 -fstack-protector-strong -D__BTIF_AUTOPOWEROFF__ -Werror=date-time -Wlogical-op -Wimplicit-fallthrough -Wno-trigraphs -fno-strict-aliasing  -I../../platform/cmsis/inc  -I../../platform/hal  -I../../utils/hwtimer_list  -I../../platform/cmsis/inc  -I../../platform/drivers/ana -std=gnu99 -Werror=implicit-int -Werror-implicit-function-declaration -c -o platform/cmsis/cmsis_nvic.o ../../platform/cmsis/cmsis_nvic.c
