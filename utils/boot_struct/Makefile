cur_dir := $(dir $(lastword $(MAKEFILE_LIST)))

obj-y := $(patsubst $(cur_dir)%,%,$(wildcard $(cur_dir)*.c $(cur_dir)*.S))
obj-y := $(obj-y:.c=.o)
obj-y := $(obj-y:.S=.o)

ccflags-y := -Iplatform/hal

ifeq ($(SECURE_BOOT),1)
ccflags-y += -DSECURE_BOOT
endif

ifneq ($(SECURE_BOOT_CODE_VER),)
ccflags-y += -DSECURE_BOOT_CODE_VER=$(SECURE_BOOT_CODE_VER)
endif

