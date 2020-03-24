#include <stdexcept>
#include "Uart.h"

//--------Constructors and destructor--------

Uart::Uart(){
    readDataSize = max_bufferSize;
    readData.reserve(readDataSize);
}

//Destructor
Uart::~Uart(){
    try {
        if(device != -1){
            close(device);
            device = -1;
        }
    }
    catch(...){ }
}

//-----getters and setters------

//--------other methods-----------

void Uart::read(std::string& readInData){
    readInData.clear();
    if(device == 0){
        printf("Device not open\r\n");
        printf("Error: %s\n",strerror(errno));
    }
    //add to readData
    //ssize_t n = read(device);

}

void Uart::write(std::string& writeOutData){

}

void Uart::conf(){
    device = open(DEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (device == -1){
        printf("Failed to open device\r\n");
        printf("Error: %s\n",strerror(errno));
    }

    struct termios configuration;
    tcgetattr(device, &configuration);
    configuration.c_cflag &= ~CSTOPB;
    configuration.c_cflag |= CLOCAL;
    configuration.c_cflag |= CREAD;
    configuration.c_cc[VTIME]=0;
    configuration.c_cc[VMIN]=0;
    configuration.c_iflag = 0;
    configuration.c_oflag = 0;
    configuration.c_lflag = 0;
    cfsetspeed(&configuration, B115200);

    cfmakeraw(&configuration);

    if(tcsetattr(device, TCSANOW,&configuration) < 0) {
        printf("Failed to configure the device\r\n");
        printf("Error: %s\n",strerror(errno));
    }

    if(tcsetattr(device, TCSAFLUSH, &configuration) < 0) {
        printf("Failed to configure device\r\n");
        printf("Error: %s\n",strerror(errno));
    }

    printf("Port open.");
    portState = PortState::OPEN;
}

std::string Uart::getReadBuffer() {
    std::string result(readData.begin(), readData.end());
    return result;
}


