#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


int main (void){
int device = -1;
unsigned char input;

//open device
device = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
if (device == -1){
	printf("Failed to open device\r\n");
	printf("Error: %s\n",strerror(errno));
	return -1;
}

//setup
struct termios conf;
tcgetattr(device, &conf);
conf.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
conf.c_iflag = 0;
conf.c_oflag = 0;
conf.c_lflag = 0;
if(tcsetattr(device, TCSANOW,&conf) < 0) {
	printf("Failed to configur device\r\n");
	printf("Error: %s\n",strerror(errno));
	return -1;
}

if(tcsetattr(device, TCSAFLUSH, &conf) < 0) {
	printf("Failed to configure device\r\n");
	printf("Error: %s\n",strerror(errno));
	return -1;
}

while(1){
//r&w
if(read(device,&input,1)>0) { write(STDOUT_FILENO,&input,1);}
if(read(STDIN_FILENO,&input,1)>0) { write(device,&input,1);}
}

//close port
close(device);
}
