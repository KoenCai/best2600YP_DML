
cmd_utils/libc/machine/arm/built-in.a := ( /usr/bin/printf 'create utils/libc/machine/arm/built-in.a\n addmod utils/libc/machine/arm/memcpy.o,utils/libc/machine/arm/strcpy.o,utils/libc/machine/arm/memchr.o,utils/libc/machine/arm/aeabi_memset.o,utils/libc/machine/arm/aeabi_memset-soft.o\n save\nend' | arm-none-eabi-ar -M )
