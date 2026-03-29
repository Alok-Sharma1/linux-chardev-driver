#Kernel module name
obj-m += chardev.o

#Path to kernel build system for current running kernel
KDIR := /lib/modules/$(shell uname -r)/build

#Current working directory
PWD := $(shell pwd)

#Default target: build kernel module
all:
	make -C $(KDIR) M=$(PWD) modules

#Build the user-space test application
app:
	gcc -Wall -o test_app test_app.c

#Clean all build artifacts
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f test_app
