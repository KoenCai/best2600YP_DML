
cmd_platform/drivers/spi_norflash/built-in.a := ( /usr/bin/printf 'create platform/drivers/spi_norflash/built-in.a\n addmod platform/drivers/spi_norflash/spi_norflash_drv.o\n save\nend' | arm-none-eabi-ar -M )
