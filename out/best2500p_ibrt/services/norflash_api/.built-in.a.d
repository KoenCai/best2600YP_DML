
cmd_services/norflash_api/built-in.a := ( /usr/bin/printf 'create services/norflash_api/built-in.a\n addmod services/norflash_api/norflash_api.o,services/norflash_api/app_flash_api.o\n save\nend' | arm-none-eabi-ar -M )
