
cmd_services/voice_dev/built-in.a := ( /usr/bin/printf 'create services/voice_dev/built-in.a\n addmod services/voice_dev/voice_tx_aec.o,services/voice_dev/voice_noise_est.o,services/voice_dev/voice_algo.o,services/voice_dev/voice_dev.o,services/voice_dev/voice_algo_cp.o\n save\nend' | arm-none-eabi-ar -M )
