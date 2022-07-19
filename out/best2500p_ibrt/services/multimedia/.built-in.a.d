
cmd_services/multimedia/built-in.a := ( /usr/bin/printf 'create services/multimedia/built-in.a\n addmod \n addlib services/multimedia/best1501_libmultimedia_cp.a\n addlib services/multimedia/audio/process/anc/cfg/built-in.a\n addlib services/multimedia/audio/process/filters/cfg/built-in.a\n addlib services/multimedia/audio/process/resample/coef/built-in.a\nsave\nend' | arm-none-eabi-ar -M )
