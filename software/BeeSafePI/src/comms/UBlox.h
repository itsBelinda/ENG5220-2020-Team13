#ifndef BEESAFEPI_UBLOX_H
#define BEESAFEPI_UBLOX_H

#include "UArt.h"

#include <string>

// The length of the response buffer.
#define AT_CMD_BUFF_LEN 544

// Publicly available set send message mode commands.
#define AT_CMD_SEND_MSG_SET_MODE_TEXT "AT+CMGF=1\r"
#define AT_CMD_SEND_MSG_SET_MODE_PDU "AT+CMGF=0\r"

// Publicly available set location scan mode commands.
#define AT_CMD_SET_LOCATION_SCAN_MODE_NORMAL "AT+ULOCCELL=0\r"
#define AT_CMD_SET_LOCATION_SCAN_MODE_DEEP "AT+ULOCCELL=1\r"

class UBlox
{

public:

    // Constructors and destructors.
    explicit UBlox();
    ~UBlox();

public:

    // Initialise serial interface and UBlox device.
    bool init();

    // Get the UArt interface on which the UBlox interface sits.
    const UArt &getUArt();

    // Connection specific functions.
    bool hasGPRS(bool &attached);
    bool hasPSD(bool &connected);

    // Methods for connecting / attaching GPRS / PSD.
    bool attachGPRS();
    bool connectPSD(bool &connected, std::string &urc);

    // Methods for getting and setting the message mode.
    bool getSendMessageMode(const char *msgMode);
    bool setSendMessageMode(const char *msgMode);

    // Methods for querying the U-Blox device.
    bool getModelNumber(std::string &modelNumber);
    bool getIMEI(std::string &imeiNumber);
    bool getLocation(double &lat, double &lng);

    // Methods for outgoing commands.
    bool sendMessage(const std::string &phoneNumber, const std::string &message);
    bool sendLocation(const std::string &phoneNumber, double lat, double lng);

private:

    // Write a command to the device via the UART interface.
    ssize_t writeCommand(const char *command);

    // Read into the buffer a raw response or a status response.
    ssize_t readRawResponse(int timeoutMs);
    const char* readStatusResponse(bool crlf);

    // Resolve the status within the UBlox buffer.
    const char* const checkResponseBuffStatus();

    // Utilised for clearing the UBlox buffer.
    void clearResponseBuff();

private:

    // The uArt interface and the buffer into which responses are written.
    UArt uArt;
    char buffer[AT_CMD_BUFF_LEN] = {'\0'};

};

#endif //BEESAFEPI_UBLOX_H



