
#ifndef BEESAFEPI_UBLOX_H
#define BEESAFEPI_UBLOX_H

#include "UArt.h"

#include <string>

// The length of the response buffer.
#define AT_CMD_BUFF_LEN 544

class UBlox
{

public:

    // Constructors and destructors.
    explicit UBlox();
    ~UBlox();

public:

    // Getters and setters.
    UArt &getUArtInterface();
    int getUArtDevice();
    bool isUArtOpen();

    // Connection specific functions.
    bool hasGPRS();
    bool hasPSD();

    // Methods for connecting / attaching GPRS / PSD.
    bool attachGPRS();
    bool connectPSD();

    // Methods for querying the U-Blox chip.
    bool getModelNumber(std::string &modelNumber);
    bool getIMEI(std::string &imei);
    bool getLocation(double &lat, double &lng);

    // Methods for outgoing commands.
    bool sendMessage(std::string &phoneNumber, std::string &message);
    bool sendLocation(std::string &phoneNumber, double lat, double lng);

private:

    // Configure the UBlox device.
    int conf();

    // Write and read via the uart interface.
    ssize_t writeNext(const char *command);
    ssize_t readNext(int timeout);

    // Buffer specific commands.
    void clearRx();
    char checkRxStatus();

private:

    // Attributes.
    UArt uart;

    char rxBuffer[AT_CMD_BUFF_LEN] = {'\0'};

};

#endif //BEESAFEPI_UBLOX_H



