#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>

/* Magic number - uniquely identifies our ioctl commands */
#define CHARDEV_MAGIC 'k'

/* IOCTL command definitions */
#define CHARDEV_IOCTL_RESET _IO(CHARDEV_MAGIC, 0) /*Reset buffer*/
#define CHARDEV_IOCTL_GET_LEN _IOR(CHARDEV_MAGIC, 1, int) /*Get data length*/

#define DEVICE_NAME "chardev"
#define BUFFER_SIZE 1024

#endif /*CHARDEV_H*/
