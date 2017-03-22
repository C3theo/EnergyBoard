/*
	Userspace application for I2C Communication
	
IMPORTANT: because of the use of inline functions, you *have* to use
'-O' or some variation when you compile your program!

*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "i2c-dev.h"

void repeat_write(int count){
	for (int i =0; i < count; i++){
		if (i2c_smbus_write_quick(file, I2C_SMBUS_WRITE) < 0){
			perror("I2C write operation failed.");
			exit(3);
		}
	}
}


int main()
{
	int file;
	int adapter_nr = 2;
	char filename[20];

	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) {
	/* ERROR HANDLING; you can check errno to see what went wrong */
	exit(3);
	}

	int addr = 0x21; /* I2C Slave address */

	if (ioctl(file, I2C_SLAVE, addr) < 0) {
	perror("Failed to create I2C_SLAVE.");
	exit(3);
	}
	
	_u8 reg = 0x10; /* Device register to access */
	__s32 res;
	char buf[10];
	
	/* Using SMBus commands */
	res = i2c_smbus_read_word_data(file, reg);
	if (res < 0) {
	/* ERROR HANDLING: i2c transaction failed */
	} else {
	/* res contains the read word */
	}
	



	
	return 0;
	
}


