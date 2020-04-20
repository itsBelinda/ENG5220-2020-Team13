/**
 * \file UBlox.h
 * \class UBlox
 *
 * \ingroup Comms
 *
 * \brief The class containing module specific functionality for the u-blox GSM module
 *
 * The UBlox class contains hardware and manufacturer-specific method calls, configuration options and some higher level wrappers
 * to facilitate the communication requirements of this project via the u-blox SARA-G350 GSM module. This includes the universal
 * AT commands used for mobile communications on the GSM network, as well as their specific implementation and configuration
 * pertaining not only the u-blox company, but the specific make and model of the GSM module. It is a mid-level communications
 * class making the UART I/O human-readable, grouping together commands related to the high-level communication features of Comms.
 *
 * \author BeeSafe Team, Team 13
 *
 * \version v1.0
 *
 * \date 2020/04/20
 *
 * Contact: beesafe.uofg@gmail.com
 *
 * Licence: MIT
 */

#ifndef BEESAFEPI_UBLOX_H
#define BEESAFEPI_UBLOX_H

#include "UArt.h"

#include <string>

// The length of the response buffer.
#define AT_CMD_BUFF_LEN 544

// Define the possible send message modes.
#define SEND_TEXT_MODE_PDU 0
#define SEND_TEXT_MODE_TEXT 1

// Define the possible scan location modes.
#define LOC_SCAN_MODE_NORMAL 0
#define LOC_SCAN_MODE_DEEP 1

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
    bool hasRegistered(bool &registered);
    bool hasGPRS(bool &attached);
    bool hasPSD(bool &connected);

    // Methods for connecting / attaching GPRS / PSD.
    bool startAutoRegistration(bool &registered);
    bool connectPSD(bool &connected, std::string &urc);

    // Methods for getting and setting the message mode.
    bool getSendMessageMode(char &mode);
    bool setSendMessageMode(char sendMessageMode);

    // Methods for getting and setting the location scan mode.
    bool getLocationScanMode(char &scanMode);
    bool setLocationScanMode(char scanMode);

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



