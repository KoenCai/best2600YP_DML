
cmd_utils/libc/built-in.a := ( /usr/bin/printf 'create utils/libc/built-in.a\n addmod \n addlib utils/libc/machine/arm/built-in.a\nsave\nend' | arm-none-eabi-ar -M )
