#include <linux/module.h>     /* MODULE_LICENSE, module_init/exit */
#include <linux/kernel.h>     /* printk, KERN_INFO */
#include <linux/init.h>       /* __init, __exit */
#include <linux/fs.h>         /* file_operations, register_chardev */
#include <linux/uaccess.h>    /* copy_to_user, copy_from_user */
#include <linux/mutex.h>      /* mutex */
#include <linux/device.h>     /* class_create, device_create */
#include <linux/cdev.h>       /* cdev */

#include "chardev.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alok Sharma");
MODULE_DESCRIPTION("A simple Linux character device driver");
MODULE_VERSION("1.0");

/*-----Driver State-----*/
static int    major_number;
static char   device_buffer[BUFFER_SIZE];
static int    data_len = 0;
static struct class *chardev_class = NULL;
static struct device *chardev_device = NULL;
static DEFINE_MUTEX(chardev_mutex);

/*-----Function Prototypes----*/
static int     dev_open   (struct inode*, struct file*);
static int     dev_release(struct inode*, struct file*);
static ssize_t dev_read   (struct file*, char __user*, size_t, loff_t*);
static ssize_t dev_write  (struct file*, const char __user*, size_t, loff_t*);
static long    dev_ioctl  (struct file*, unsigned int, unsigned long);

/*-----File Operations Struct-----*/
static struct file_operations fops = {
	.owner		= THIS_MODULE,
	.open		= dev_open,
	.release	= dev_release,
	.read		= dev_read,
	.write		= dev_write,
	.unlocked_ioctl = dev_ioctl,
};

/*-----open()----*/
static int dev_open(struct inode* inodep, struct file* filep)
{
	if(!mutex_trylock(&chardev_mutex))
	{
		printk(KERN_ALERT "chardev: Device is busy\n");
		return -EBUSY;
	}
	printk(KERN_INFO "chardev: Device opened\n");
	return 0;
}

/*-----release() - called on close()---*/
static int dev_release(struct inode* inodep, struct file* filep)
{
	mutex_unlock(&chardev_mutex);
	printk(KERN_INFO "chardev: Device closed\n");
	return 0;
}

/*---------read() - sends data from kernel buffer --> user-space----*/
static ssize_t dev_read(struct file* filep, char __user* user_buf,
			size_t count, loff_t *offset)
{
	int bytes_to_read;
	
	/*Calculate how many bytes are left to read from current offset */
	bytes_to_read = data_len - (int)(*offset);
	
	if(bytes_to_read <= 0)
	{
		printk(KERN_INFO "chardev: End of buffer reached\n");
		return 0;
	}
	/* Don't read more than asked */
	if(bytes_to_read > count)
		bytes_to_read = count;
	
	/* Safe copy from kernel buffer to user space */
	if(copy_to_user(user_buf,device_buffer + *offset, bytes_to_read))
	{
		printk(KERN_ALERT "chardev: Failed to copy to user\n");
		return -EFAULT;
	}

	*offset +=bytes_to_read;
	printk(KERN_INFO "chardev: Sent %d bytes to user\n", bytes_to_read);
	return bytes_to_read;
}

/*--------write() - receives data from user space --> kernel space */
static ssize_t dev_write(struct file* filep, const char __user* user_buf, 
			 size_t count, loff_t *offset)
{
	/* Clamp to buffer size */
	if(count > BUFFER_SIZE)
	{
		printk(KERN_ALERT "chardev: Write loop too large, truncating\n");
		count = BUFFER_SIZE;
	}

	/*Safe copy from user space to kernel buffer */
	if(copy_from_user(device_buffer, user_buf, count))
	{
		printk(KERN_ALERT "chardev: Failed to copy from user\n");
		return -EFAULT;
	}
	
	data_len = count;
	printk(KERN_INFO "chardev: Received %zu bytes from user\n", count);
	return count;
}

/*----ioctl() - handle control commands---*/
static long dev_ioctl(struct file* filep, unsigned int cmd, unsigned long arg)
{
	switch (cmd)
	{
		case CHARDEV_IOCTL_RESET:
			memset(device_buffer, 0, BUFFER_SIZE);
			data_len = 0;
			printk(KERN_INFO "chardev: Buffer reset via ioctl\n");
			break;
		
		case CHARDEV_IOCTL_GET_LEN:
			/* copy data_len back to user space */
			if(copy_to_user((int __user *)arg, &data_len, sizeof(int)))
			{
				return -EFAULT;
			}
			printk(KERN_INFO "chardev: Reported length %d via ioctl\n", data_len);
			break;
		default:
			return -EINVAL; /*Invalid command */
		}
	return 0;
}

/*------Module Init-------*/
static int __init chardev_init(void)
{
	/* 1. Register the character device and get a major number */
	major_number = register_chrdev(0, DEVICE_NAME, &fops);
	if(major_number < 0)
	{
		printk(KERN_ALERT "chardev: Failed to register, error %d\n", major_number);
		return major_number;
	}
	printk(KERN_INFO "chardev: Registered with major number %d\n", major_number);
	
	/* 2. Create device class (appears under /sys/class/) */
	chardev_class = class_create(DEVICE_NAME);
	if(IS_ERR(chardev_class))
	{
		unregister_chrdev(major_number, DEVICE_NAME);
		printk(KERN_ALERT "chardev: Failed to create class\n");
		return PTR_ERR(chardev_class);
	}

	/*3. Create the device node (/dev/chardev) automatically */
	chardev_device = device_create(chardev_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);

	if(IS_ERR(chardev_device))
	{
		class_destroy(chardev_class);
		unregister_chrdev(major_number, DEVICE_NAME);
		printk(KERN_ALERT "chardev: Failed to create device\n");
		return PTR_ERR(chardev_device);
	}
	
	/* 4. Initialize the mutex */
	mutex_init(&chardev_mutex);
	
	printk(KERN_INFO "chardev: Driver loaded. /dev/%s ready.\n", DEVICE_NAME);
	return 0;
}

/*------Module Exit-------*/
static void __exit chardev_exit(void)
{
	device_destroy(chardev_class, MKDEV(major_number, 0));
	class_destroy(chardev_class);
	unregister_chrdev(major_number, DEVICE_NAME);
	mutex_destroy(&chardev_mutex);
	printk(KERN_INFO "chardev: Driver unloaded.\n");
}

module_init(chardev_init);
module_exit(chardev_exit);
