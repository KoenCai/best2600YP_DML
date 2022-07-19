
cmd_services/ibrt_middleware/built-in.a := ( /usr/bin/printf 'create services/ibrt_middleware/built-in.a\n addmod services/ibrt_middleware/src/app_ibrt_middleware.o\n save\nend' | arm-none-eabi-ar -M )
