
cmd_services/custom_api/built-in.a := ( /usr/bin/printf 'create services/custom_api/built-in.a\n addmod \n addlib services/custom_api/libcustom_api_multipoint.a\nsave\nend' | arm-none-eabi-ar -M )
