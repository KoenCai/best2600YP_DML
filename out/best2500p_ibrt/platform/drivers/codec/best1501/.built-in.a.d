
cmd_platform/drivers/codec/best1501/built-in.a := ( /usr/bin/printf 'create platform/drivers/codec/best1501/built-in.a\n addmod platform/drivers/codec/best1501/codec_vad_best1501.o,platform/drivers/codec/best1501/codec_best1501.o\n save\nend' | arm-none-eabi-ar -M )
