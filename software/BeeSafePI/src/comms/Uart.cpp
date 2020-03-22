#include <stdexcept>
#include "Uart.h"

//--------Constructors and destructor--------

Uart::Uart(const std::string &devName, BaudRate baud){
    deviceName = devName;
    baudRate = baud;
    readDataSize = max_bufferSize;
    readData.reserve(readDataSize);
}

//Empty constructor
Uart::Uart() {}

//Destructor
Uart::~Uart(){
    try { closePort(); }
    catch(...){ }
}

//-----getters and setters------
void Uart::setDevice(const std::string &devName){
    deviceName = devName;
    if(portState == PortState::OPEN){
        conf();
    }
}

void Uart::setBaudRate(BaudRate baud){
    baudRate = baud;
    if(portState == PortState::OPEN){
        conf();
    }
}

//--------other methods-----------
void Uart::openPort(){
    device = open(deviceName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (device == -1){
        printf("Failed to open device\r\n");
        printf("Error: %s\n",strerror(errno));
    }
    conf();
    printf("Port open.");
    portState = PortState::OPEN;
}

void Uart::closePort(){
    if(device != -1){
        close(device);
        device = -1;
    }
    printf("Port closed.");
    portState = PortState::CLOSED;
}

void Uart::read(std::string& readInData){

}

void Uart::write(std::string& writeOutData){

}

void Uart::conf(){

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

    switch(baudRate) {
        case BaudRate::BR9600:
            cfsetspeed(&configuration, B9600);
            break;
        case BaudRate::BR115200:
            cfsetspeed(&configuration, B115200);
            break;
        default:
            throw std::runtime_error(std::string() + "baudRate cannot be set, choose one of the options");
    }

    cfmakeraw(&configuration);

    if(tcsetattr(device, TCSANOW,&configuration) < 0) {
        printf("Failed to configure the device\r\n");
        printf("Error: %s\n",strerror(errno));
    }

    if(tcsetattr(device, TCSAFLUSH, &configuration) < 0) {
        printf("Failed to configure device\r\n");
        printf("Error: %s\n",strerror(errno));
    }
}

