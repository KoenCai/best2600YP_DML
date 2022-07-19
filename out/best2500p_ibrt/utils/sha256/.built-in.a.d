
cmd_utils/sha256/built-in.a := ( /usr/bin/printf 'create utils/sha256/built-in.a\n addmod utils/sha256/sha256.o\n save\nend' | arm-none-eabi-ar -M )
