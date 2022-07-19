
cmd_services/../utils/hsm/built-in.a := ( /usr/bin/printf 'create services/../utils/hsm/built-in.a\n addmod \n addlib services/../utils/hsm/libhsm.a\nsave\nend' | arm-none-eabi-ar -M )
