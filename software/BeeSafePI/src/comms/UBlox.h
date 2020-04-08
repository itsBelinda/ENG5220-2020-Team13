
#ifndef BEESAFEPI_UBLOX_H
#define BEESAFEPI_UBLOX_H

#include "Uart.h"

#include <string>

#define  MAX_BUFFER_LENGTH 544

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
    int getLocation(double &lat, double &lng);
    int sendMsg(std::string &nbr, std::string &message);

private:

    // Configure the UBlox device.
    int conf();
    int processCmd(const char *const cmd);
    int processCmd(const char *const cmd, std::string &response);
    bool sendCmd(const char * const cmdBuffer);
    bool checkStatusOK();
    static bool findCharArray(const char *const needle, const char *const haystack);
    static bool checkNoError(const char *const checkBuffer);

private:

    // Attributes.
    Uart uart;
    char rxBuffer[MAX_BUFFER_LENGTH] = {'\0'};

};

#endif //BEESAFEPI_UBLOX_H



