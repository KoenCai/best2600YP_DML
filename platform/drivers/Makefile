obj-y := ana/ codec/ norflash/ bt/ btpcm/ sbcacc/ spi_norflash/

ifneq ($(filter-out 1 2 3 4,$(CHIP_FLASH_CTRL_VER)),)
ifeq ($(NANDFLASH_SUPPORT),1)
obj-y += nandflash/
endif
endif

ifeq ($(CHIP_HAS_USB),1)
obj-y += usb/
endif

ifeq ($(CHIP_HAS_FM),1)
obj-y += fm/
endif

ifeq ($(CHIP_HAS_CP),1)
obj-y += cp_accel/
endif

ifeq ($(SENSOR_HUB),1)
obj-y += sensor_hub/
endif

ifeq ($(SENSOR),1)
obj-y += sensor/
endif

