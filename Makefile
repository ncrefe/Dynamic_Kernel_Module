obj-m += proc_info_module.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
test:
	sudo dmesg -C
	sudo insmod proc_info_module.ko
	sudo rmmod proc_info_module.ko
	dmesg