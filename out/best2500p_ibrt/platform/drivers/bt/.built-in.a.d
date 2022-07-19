
cmd_platform/drivers/bt/built-in.a := ( /usr/bin/printf 'create platform/drivers/bt/built-in.a\n addmod platform/drivers/bt/bt_drv_common.o,platform/drivers/bt/bt_drv_iq_common.o,platform/drivers/bt/bt_ramrun_bin.o\n addlib platform/drivers/bt/best1501/built-in.a\nsave\nend' | arm-none-eabi-ar -M )
