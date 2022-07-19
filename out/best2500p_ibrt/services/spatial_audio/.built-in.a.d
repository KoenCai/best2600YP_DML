
cmd_services/spatial_audio/built-in.a := ( /usr/bin/printf 'create services/spatial_audio/built-in.a\n addmod services/spatial_audio/cwm_api.o\n addlib services/spatial_audio/cwm_lib/built-in.a\nsave\nend' | arm-none-eabi-ar -M )
