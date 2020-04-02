
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
    int requestLocation();
    int getLocation(double *const lat, double *const lng);
    int sendMsg(const std::string &nbr, const std::string &message);

    bool tempGetLoc(double *const lat, double *const lng);
    bool activatePSD();
    bool checkConnections();
    bool checkPSD();
private:

    // Configure the UBlox device.
    int conf();
    int processCmd(const char *const cmd);
    int processCmd(const char *const cmd, int timeout, std::string &response);
    bool sendCmd(const char * const cmdBuffer);
    bool checkStatusOK();
    static bool findCharArray(const char *const needle, const char *const haystack);
    static bool checkNoError(const char *const checkBuffer);
    static double getSysTimeMS();

private:

    // Attributes.
    Uart uart;
    bool locationRequested;
    char rxBuffer[MAX_BUFFER_LENGTH] = {'\0'};
};

#endif //BEESAFEPI_UBLOX_H



