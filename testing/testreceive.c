#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main (int argc, char** argv){
int device = -1;

//open device
device = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NONBLOCK);
if (device == -1){
	printf("Failed to open device\r\n");
	printf("Error: %s\n",strerror(errno));
	return -1;
}

//setup
struct termios conf;
tcgetattr(device, &conf);
cfsetspeed(&conf, B115200);
cfmakeraw(&conf);
conf.c_cflag &= ~CSTOPB;
conf.c_cflag |= CLOCAL;
conf.c_cflag |= CREAD;
conf.c_cc[VTIME]=0;
conf.c_cc[VMIN]=0;
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

char initText[] = ".ATI0\r\n";
write(device, initText, strlen(initText)+1);

int count = 0;
int count2 = 0;

char received[100];
char in;
char out;

int i=0;

while(1){
	count = read(device, &in, 1);
	if(count!=0){
		write(STDOUT_FILENO, &in, 1);
		received[i]=in;
		i++;
		if(in==0) {continue;}
	};
};

close(device);
return (EXIT_SUCCESS);

}
