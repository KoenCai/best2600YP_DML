
cmd_apps/audio_test/built-in.a := ( /usr/bin/printf 'create apps/audio_test/built-in.a\n addmod apps/audio_test/audio_test_cmd.o,apps/audio_test/audio_test_stream.o\n save\nend' | arm-none-eabi-ar -M )
