KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
USERLAND_DIR := userland
PREFIX ?= /usr/local

ifneq ($(KERNELRELEASE),)

CONFIG_DRM_GNA := m
obj-$(CONFIG_DRM_GNA) += drivers/gpu/drm/gna/

else

.PHONY: all install kernel kernel_install userland userland_install test clean

all: kernel userland

install: kernel_install userland_install

kernel:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules

kernel_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules_install

userland:
	$(MAKE) -C $(USERLAND_DIR)

userland_install:
	install -D $(USERLAND_DIR)/gna_tool $(PREFIX)/bin/gna_tool

test:
	sudo modprobe gna
	$(USERLAND_DIR)/gna_tool --version

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) clean
	$(MAKE) -C $(USERLAND_DIR) clean

endif
