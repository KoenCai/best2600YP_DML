
cmd_services/nv_section/userdata_section/built-in.a := ( /usr/bin/printf 'create services/nv_section/userdata_section/built-in.a\n addmod services/nv_section/userdata_section/nvrecord_env.o,services/nv_section/userdata_section/nvrecord_extension.o,services/nv_section/userdata_section/nvrecord_dma_config.o,services/nv_section/userdata_section/nvrecord_fp_account_key.o,services/nv_section/userdata_section/nvrecord_bt.o\n save\nend' | arm-none-eabi-ar -M )