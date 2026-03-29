# Linux Character Device Driver

A fully functional Linux kernel module implementing a character driver from scratch.

## Features
- Custom '/dev/chardev' device node
- 'open', 'read', 'write', 'release' file operations
- 'ioctl' commands: buffer reset and length query
- Safe kernel<->user data transfer via 'copy_to_user' / 'copy_from_user'
- Mutex-based locking to prevent race conditions

## Prerequisites
sudo apt install build-essential linux-headers-$(uname -r)

##Commands
make all 		#Build kernel module
make app		#Build test application
sudo insmod chardev.ko  #Load driver
sudo chmod 666 /dev/chardev
./test_app		#Run test
sudo rmmod chardev	#Unload driver
dmesg			#Check logs

