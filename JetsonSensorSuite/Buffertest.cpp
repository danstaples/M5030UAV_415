#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

// Adapted from Canonical Arduino read by Chris Heydrick - 
// https://github.com/cheydrick/Canonical-Arduino-Read/blob/master/canonicalarduinoread.c
  
int main(int argc, char *argv[])
{
  int fd, n, i;
   char buf[BUFFER_SIZE] = "temp text";
  bool ready = 0;
  bool second = 1;
  
  
  struct termios toptions;

  /* open serial port */
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
  printf("fd opened as %i\n", fd);
  
  /* wait for the Arduino to reboot */
  usleep(3500000);
  
  /* get current serial port settings */
  tcgetattr(fd, &toptions);
  /* set 115200 baud both ways */
  cfsetispeed(&toptions, B115200);
  cfsetospeed(&toptions, B115200);
  /* 8 bits, no parity, no stop bits */
  toptions.c_cflag &= ~PARENB;
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;
  /* Canonical mode */
  toptions.c_lflag |= ICANON;
  /* commit the serial port settings */
  tcsetattr(fd, TCSANOW, &toptions);
	while(1){
	
		n = read(fd, buf, BUFFER_SIZE);
		buf[n] = 0;
		if(n!=1)
		//printf("%i bytes read, buffer contains: %s\n", n, buf);

		//printf("Ready status: %i\n", ready);


		if ((buf[0] == '9') && (n == 2)) 
		{ready = 1;}
		
		if((ready == 1) && (second == 0)){
	
			printf("\nSending 1\n");
			write(fd, "1\n", 1);
			tcdrain(fd); // Orders jetson to wait until its send buffer clears, prevents funky serial outputs
			printf("\nSending 2\n");
			write(fd, "2\n", 1);
			printf("Second flag status: %i\n", second);
			tcdrain(fd);
			ready = 0;
			second = 1;
			printf("Ready status: %i\n", ready);
			//usleep(1000);
		}
		
		if (n > 4)
		{
			printf("%i bytes read, buffer contains: %s\n", n, buf);
			ready = 1;
		}

		
		for (int i = 0; i < 10; i++) {
			buf[i] = 'T';
		}
		//for (int i = 0; i < n; i++){
		//	buf[i] = '\0' ;
		//}

		 tcflush(fd, TCIOFLUSH);
		
	
	  /* Receive string from Arduino */
		// n = read(fd, buf, BUFFER_SIZE);
	  /* insert terminating zero in the string */
		//printf("%i bytes read, buffer contains: %s\n", n, buf);
		//usleep(100000);
	}
}

