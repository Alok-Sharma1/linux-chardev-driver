#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>	/* open() */
#include <unistd.h>	/* read(), write(), close() */
#include <sys/ioctl.h>	/* ioctl() */

#include "chardev.h"	/* Our shared header for ioctl commands */

#define DEVICE_PATH "/dev/chardev"

int main(void)
{
	int fd;
	char write_buf[] = "Hello from user space";
	char read_buf[BUFFER_SIZE];
	int length = 0;

	printf("===Character Device Driver Test ===\n\n");

	/* Open the device */
	fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0)
	{
		perror("Failed to open device");
		return EXIT_FAILURE;
	}
	printf("[+] Device opened: %s\n", DEVICE_PATH);

	/*Write to device */
	printf("[+] Writing: \"%s\"\n", write_buf);
	if(write(fd, write_buf, strlen(write_buf)) < 0)
	{
		perror("Failed to write");
		close(fd);
		return EXIT_FAILURE;
	}

	/* Use ioctl to get buffer length */
	if(ioctl(fd, CHARDEV_IOCTL_GET_LEN, &length) < 0)
	{
		perror("ioctl GET_LEN failed");
	}
	else
	{
		printf("[+] ioctl reports buffer length: %d bytes\n", length);
	}

	/*Read back from the device */
	memset(read_buf, 0, sizeof(read_buf));
	/* Reset offset by seeking to start */
	lseek(fd, 0, SEEK_SET);
	if(read(fd, read_buf, sizeof(read_buf)) < 0)
	{
		perror("Failed to read");
		close(fd);
		return EXIT_FAILURE;
	}
	printf("[+] Read back: \"%s\"\n", read_buf);

	/*Reset buffer via ioctl*/
	if(ioctl(fd, CHARDEV_IOCTL_RESET) < 0)
	{
		perror("ioctl RESET failure");
	}
	else
	{
		printf("[+] ioctl RESET sent - buffer cleared\n");
	}
	
	/* Check length is now 0*/
	ioctl(fd, CHARDEV_IOCTL_GET_LEN, &length);
	printf("[+] Buffer length after reset: %d bytes\n", length);
	
	/* Close device */
	close(fd);
	printf("[+] Device closed.\n");
	
	return EXIT_SUCCESS;
}
