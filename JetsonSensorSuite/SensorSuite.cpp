#include SensorSuite.h

void init(){
	
	int fd, n, i;
  char buf[BUFFER_SIZE] = "temp text";
  
  struct termios toptions;

  /* open serial port */
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
  printf("fd opened as %i\n", fd);
  
  /* wait for the Arduino to reboot */
  usleep(3500000);
  
  /* get current serial port settings */
  tcgetattr(fd, &toptions);
  /* set 9600 baud both ways */
  cfsetispeed(&toptions, B9600);
  cfsetospeed(&toptions, B9600);
  /* 8 bits, no parity, no stop bits */
  toptions.c_cflag &= ~PARENB;
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;
  /* Canonical mode */
  toptions.c_lflag |= ICANON;
  /* commit the serial port settings */
  tcsetattr(fd, TCSANOW, &toptions);
	
}

void get_tof(){
	
	if(DEBUG) printf("\nSending 1 for ToF Data\n");
	
	write(fd, "1", 1);
	tcdran(fd);
	
}

void get_thermal(){
	
	if(DEBUG) printf("\nSending 2 for Thermal Data\n");
		
	
	write(fd, "2", 1);
	tcdran(fd);
	
}

void read_data(){
	
	if(DEBUG) printf("\nReading data\n");
	n = read(fd, buf, BUFFER_SIZE);
	buf[n] = 0;
	if(n!=1)
	if (DEBUG) printf("%i bytes read, buffer contains: %s\n", n, buf);
	
}