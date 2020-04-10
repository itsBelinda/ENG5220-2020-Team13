
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

    // Get the UArt interface.
    const UArt &getUArt();

    // Connection specific functions.
    bool hasGPRS(bool &attached);
    bool hasPSD(bool &connected);

    // Methods for connecting / attaching GPRS / PSD.
    bool attachGPRS();
    bool connectPSD(bool &connected, std::string &urc);

    // Methods for querying the U-Blox chip.
    bool getModelNumber(std::string &modelNumber);
    bool getIMEI(std::string &imei);
    bool getLocation(double &lat, double &lng);

    // Methods for outgoing commands.
    bool sendMessage(std::string &phoneNumber, std::string &message);
    bool sendLocation(std::string &phoneNumber, double lat, double lng);

private:

    // Configure the UBlox device.
    int configure();

    // For writing to and reading from the device.
    ssize_t writeCommand(const char *command);
    ssize_t readResponse(int timeoutMs);
    const char* readResponseStatus(bool crlf);

    // Resolve the response status within the buffer.
    const char* const resolveResponseBuffStatus();

    // Clear the response buffer.
    void clearResponseBuff();

private:

    // The uart interface and the buffer into which responses are written.
    UArt uart;
    char buffer[AT_CMD_BUFF_LEN] = {'\0'};

};

#endif //BEESAFEPI_UBLOX_H



