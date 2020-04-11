
#ifndef BEESAFEPI_UBLOX_H
#define BEESAFEPI_UBLOX_H

#include "UArt.h"

// System inclusions.
#include <string>

// The length of the response buffer.
#define AT_CMD_BUFF_LEN 544

// Commands that enable the user to define the message mode.
#define AT_CMD_SEND_MSG_SET_MODE_TEXT "AT+CMGF=1\r"
#define AT_CMD_SEND_MSG_SET_MODE_PDU "AT+CMGF=0\r"

class UBlox
{

public:

    // Constructors and destructors.
    explicit UBlox();
    ~UBlox();

public:

    // Invoked to configure / re-configure the uBlox device.
    int configure();

    // Get the UArt interface.
    const UArt &getUArt();

    // Connection specific functions.
    bool hasGPRS(bool &attached);
    bool hasPSD(bool &connected);

    // Methods for connecting / attaching GPRS / PSD.
    bool attachGPRS();
    bool connectPSD(bool &connected, std::string &urc);

    // Methods for getting and setting the message mode.
    bool getSendMessageMode(const char *mode);
    bool setSendMessageMode(const char *mode);

    // Methods for querying the U-Blox chip.
    bool getModelNumber(std::string &modelNumber);
    bool getIMEI(std::string &imei);
    bool getLocation(double &lat, double &lng);

    // Methods for outgoing commands.
    bool sendMessage(const std::string &phoneNumber, const std::string &message);
    bool sendLocation(const std::string &phoneNumber, double lat, double lng);

private:

    // For writing to and reading from the device.
    ssize_t writeCommand(const char *command);
    ssize_t readResponse(int timeoutMs);
    const char* readResponseStatus(bool crlf);

    // Resolve the response status within the buffer.
    const char* const resolveResponseBuffStatus();

    // Clear the response buffer.
    void clearResponseBuff();

private:

    // The uArt interface and the buffer into which responses are written.
    UArt uArt;
    char buffer[AT_CMD_BUFF_LEN] = {'\0'};

};

#endif //BEESAFEPI_UBLOX_H



