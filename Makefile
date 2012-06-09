CC=gcc

obj-m := ECE0206lin.o

.PHONY: all
all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
