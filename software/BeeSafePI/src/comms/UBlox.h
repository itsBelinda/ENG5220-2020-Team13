
#ifndef BEESAFEPI_UBLOX_H
#define BEESAFEPI_UBLOX_H

#include "Uart.h"

#include <string>

class UBlox
{

public:

    // Constructors and destructors.
    UBlox();
    ~UBlox();

public:

    // Getters and setters.
    Uart &getUart();
    int getDevice();
    bool isOpen();

    // Methods for querying the U-Blox chip.
    int getModelNumber(std::string &modelNumber);
    int getIMEI(std::string &imei);
    int getIMEI2(std::string &imei);
    int getLocation(double &lat, double &lng);

private:

    // Configure the UBlox device.
    int conf();
    bool sendCmd(const char * const cmdBuffer);
    bool getReply(char *const replyBuffer, int expSize);
    bool checkStatus();
    bool findCharArray(const char *const needle, const char *const haystack);

private:

    // Attributes.
    Uart uart;

};

#endif //BEESAFEPI_UBLOX_H



