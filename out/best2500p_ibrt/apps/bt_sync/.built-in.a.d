
cmd_apps/bt_sync/built-in.a := ( /usr/bin/printf 'create apps/bt_sync/built-in.a\n addmod apps/bt_sync/app_bt_sync.o\n save\nend' | arm-none-eabi-ar -M )
