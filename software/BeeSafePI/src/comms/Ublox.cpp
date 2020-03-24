#include <chrono>
#include "UBlox.h"

UBlox::UBlox()
{

}

UBlox::~UBlox()
{

}

Uart& UBlox::getUart()
{
    return uart;
}

int UBlox::getDevice()
{
    return uart.getDevice();
}

bool UBlox::isOpen()
{
    return uart.isOpen();
}

int UBlox::getModelNumber(std::string &modelNumber)
{
    return -1;
}

int UBlox::getIMEI(std::string &imei)
{
    return -1;
}

int UBlox::getLocation(double &lat, double &lng)
{
    return -1;
}
