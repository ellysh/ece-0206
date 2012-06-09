KVER := $(shell uname -r)
CC=gcc
obj-m := ECE0206lin.o

.PHONY: all
all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(MAKE) test

install:
	mkdir -p /lib/modules/$(KVER)/kernel/drivers/usb/serial
	cp -f ./*.ko /lib/modules/$(KVER)/kernel/drivers/usb/serial
	mkdir -p /lib/modules/$(KVER)/misc/
	cp -f ./*.ko /lib/modules/$(KVER)/misc/
	depmod -a
