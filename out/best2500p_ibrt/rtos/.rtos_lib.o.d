
cmd_rtos/rtos_lib.o := arm-none-eabi-gcc -Wl,-r,--whole-archive -nostdlib -nostartfiles  -mthumb -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o rtos/rtos_lib.o rtos/rtx5/built-in.a
