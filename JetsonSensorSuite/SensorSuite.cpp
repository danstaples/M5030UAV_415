#include "SensorSuite.h"



int main(int argc, char *argv[]){

int fd;
bool rdy = 0;




fd = init();

test(fd);

while (!rdy) {
	rdy = chk_rdy(fd); // Waits for a byte "9" to be sent which indicates Arduino has finished sensor set-up and is ready to send data
	if (DEBUG) printf("Arduino ready");
}


rdy = get_tof(fd);

while (!rdy) {
	rdy = read_data(fd, 0);
}

rdy = get_thermal(fd);

while (!rdy) {
	rdy = read_data(fd, 1);
}

close(fd);

}

int init(){

  int fd;
  struct termios toptions;
	
  /* open serial port */
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
  printf("fd opened as %i\n", fd);
  
  /* wait for the Arduino to reboot */
  usleep(3500000);
  
  /* get current serial port settings */
  tcgetattr(fd, &toptions);
  /* set 9600 baud both ways */
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

  return fd;
	
}

bool get_tof(int fd){
	
	if(DEBUG) printf("\nSending 1 for ToF Data\n");
	
	write(fd, "1", 1);
	tcdrain(fd);
	//usleep(2000000);

	return 0;
	
}

bool get_thermal(int fd){
	
	if(DEBUG) printf("\nSending 2 for Thermal Data\n");
		
	
	write(fd, "2", 1);
	tcdrain(fd);
	//usleep(2000000);

	return 0;
	
}

bool read_data(int fd, bool therm_read){

	int n;
	char buf[BUFFER_SIZE] = "temp text";
	
	//if(DEBUG) printf("\nReading data\n");
	n = read(fd, buf, BUFFER_SIZE);
	buf[n] = 0;
	// if(n!=1)
	//if (DEBUG) printf("%i bytes read, buffer contains: %s\n", n, buf);
	if (n < 35)
	return 0;
	else if (((n == 35) && (thermRead == 0)) || ((n > 35) && (thermRead == 1)))
	{
	if (DEBUG) printf("%i bytes read, buffer contains: %s\n", n, buf);	
	return 1;
	}
}

bool chk_rdy(int fd){

	int n;
	char buf[BUFFER_SIZE] = "temp text";
	
	n = read(fd, buf, BUFFER_SIZE);
	buf[n] = 0;

if ((buf[0] == '9') && (n == 2)) return 1;

else return 0;


}

void test(int fd){

int therm_count = 0;
int tof_count = 0;
bool rdy = 0;
int return_val = 0;

while (!rdy) {
	rdy = chk_rdy(fd);
}

for (int i = 0; i < 10; i++){

	rdy = get_tof(fd);

	while (!rdy) {
		return_val = read_test(fd, 0);
		if (return_val == 1) tof_count++;
		rdy = 1;
	}

	rdy = get_thermal(fd);

	while (!rdy) {
		return_val = read_test(fd, 1);
		if (return_val == 2) therm_count++;
		rdy = 1;
	}

}

printf("Test complete, Thermal and ToF returned successfully, %i, and %i times respectively\n", therm_count, tof_count);

}

int read_test(int fd, bool therm_read){
	
	int n;
	char buf[BUFFER_SIZE] = "temp text";
	
	
	n = read(fd, buf, BUFFER_SIZE);
	buf[n] = 0;
	
	if (n < 35) return 0;
	
	else if ((n == 35) && (therm_read == 0)) {
		if (buf[0] == '1') {
			if (DEBUG) printf("%i bytes read, buffer contains: %s\n", n, buf);	
			return 1;
		}
		else return 4;
	}
	
	else if ((n > 35) && (therm_read == 1)) {
		if (buf[0] == '2') {
			if (DEBUG) printf("%i bytes read, buffer contains: %s\n", n, buf);	
			return 2;
		}
		else return 4;
	}
	
}
