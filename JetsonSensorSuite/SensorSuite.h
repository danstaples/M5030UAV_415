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
#define DEBUG 1

// Adapted from Canonical Arduino read by Chris Heydrick - 
// https://github.com/cheydrick/Canonical-Arduino-Read/blob/master/canonicalarduinoread.c

//This is used for the object detection and avoidance, for MECH5030M Team Project, Project 415: Fire-fighting UAV Drones

int init();
bool get_tof(int fd);
bool get_thermal(int fd);
bool read_data(int fd, bool therm_read);
bool chk_rdy(int fd);
void test(int fd);
int read_test(int fd, bool therm_read);

