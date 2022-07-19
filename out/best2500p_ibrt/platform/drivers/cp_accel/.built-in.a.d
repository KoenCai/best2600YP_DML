
cmd_platform/drivers/cp_accel/built-in.a := ( /usr/bin/printf 'create platform/drivers/cp_accel/built-in.a\n addmod platform/drivers/cp_accel/cp_accel.o,platform/drivers/cp_accel/cp_queue.o\n save\nend' | arm-none-eabi-ar -M )
