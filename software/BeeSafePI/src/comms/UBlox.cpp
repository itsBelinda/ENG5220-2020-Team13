/**
 * \file UBlox.cpp
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

#include "UBlox.h"

// System inclusions.
#include <boost/compatibility/cpp_c_headers/cstring>
#include <iostream>
#include <cstring>
#include <string.h>

// Define generic u-blox commands for obtaining additional information.
#define AT_CMD_GET_MODEL_NUMBER "ATI\r"
#define AT_CMD_GET_IMEI "ATI5\r"

// GPRS (network) related commands.
#define AT_CMD_HAS_REGISTERED "AT+COPS?\r"
#define AT_CMD_START_AUTO_REGISTRATION "AT+COPS=0\r"
#define AT_CMD_GET_GPRS_ATTACHED "AT+CGATT?\r"
#define AT_CMD_GET_PSD_CONNECTED "AT+UPSND=0,8\r"
#define AT_CMD_SET_PSD_CONNECTION "AT+UPSDA=0,3\r"

// Location specific commands.
#define AT_CMD_GET_LOC "AT+ULOC=2,2,0,120,500\r"
#define AT_CMD_GET_LOC_SCAN_MODE "AT+ULOCCELL?\r"
#define AT_CMD_SET_LOC_SCAN_MODE_NORMAL "AT+ULOCCELL=0\r"
#define AT_CMD_SET_LOC_SCAN_MODE_DEEP "AT+ULOCCELL=1\r"

// Define the message modes for sending text messages.
#define AT_CMD_SEND_MSG_GET_MODE "AT+CMGF?\r"
#define AT_CMD_SEND_MSG_SET_MODE_TEXT "AT+CMGF=1\r"
#define AT_CMD_SEND_MSG_SET_MODE_PDU "AT+CMGF=0\r"
#define AT_CMD_SEND_MSG_SET_NUMBER "AT+CMGS=\"%s\"\r"
#define AT_CMD_SEND_MSG_SET_ESC "\x1B"
#define AT_CMD_SEND_MSG_SET_END "\x1A"

// Define expected responses from the device.
#define AT_CMD_RESPONSE_GPRS_IS_ATTACHED "+CGATT: 1"
#define AT_CMD_RESPONSE_GPRS_NOT_ATTACHED "+CGATT: 0"
#define AT_CMD_RESPONSE_PSD_IS_CONNECTED "+UPSND: 0,8,1"
#define AT_CMD_RESPONSE_PSD_NOT_CONNECTED "+UPSND: 0,8,0"

#define AT_CMD_RESPONSE_SEND_MSG_MODE_PDU "+CMGF: 0"
#define AT_CMD_RESPONSE_SEND_MSG_MODE_TEXT "+CMGF: 1"
#define AT_CMD_RESPONSE_LOC_SCAN_MODE_NORMAL "+ULOCCELL: 0,0,\"\",\"\",0,0"
#define AT_CMD_RESPONSE_LOC_SCAN_MODE_DEEP "+ULOCCELL: 1,0,\"\",\"\",0,0"

#define AT_CMD_RESPONSE_HAS_REGISTERED "+COPS: 0"
#define AT_CMD_RESPONSE_HAS_NOT_REGISTERED "+COPS: 1"

// Define the status codes returned by the device.
#define AT_CMD_STATUS_CODE_OK "OK"
#define AT_CMD_STATUS_CODE_ERROR "ERROR"
#define AT_CMD_STATUS_CODE_ABORTED "ABORTED"

// Define timeouts for the commands.
#define RX_TIMEOUT 10
#define RX_TIMEOUT_ECHO 1000
#define RX_TIMEOUT_STATUS 1000
#define RX_TIMEOUT_NETWORK 5000

#define RX_TIMEOUT_CMD_HAS_REGISTERED 1500
#define RX_TIMEOUT_CMD_GET_LOC_SCAN_MODE 1000
#define RX_TIMEOUT_CMD_GET_SEND_MSG_MODE 1000
#define RX_TIMEOUT_CMD_GET_LOCATION 120000
#define RX_TIMEOUT_CMD_GET_MODEL_NUMBER 1000
#define RX_TIMEOUT_CMD_GET_IMEI 1000
#define RX_TIMEOUT_CMD_GET_GPRS_ATTACHED 2000
#define RX_TIMEOUT_CMD_GET_PSD_CONNECTED 1000
#define RX_TIMEOUT_CMD_SET_PSD_CONNECTION 1000


/**
 * The UBlox constructor.
 */
UBlox::UBlox() = default;

/**
 * The UBlox destructor.
 */
UBlox::~UBlox() = default;

/**
 * The UBlox initialisation method sets up the complete communications channel from the UART level
 * communication enabling to verifying the settings and network connection of the u-blox module.
 *
 * @return True if the communications channel could be successfully initialised
 *      and the device is ready to operate as intended with full capacity
 */
bool UBlox::init()
{

    // Initialise the UART device and interface.
    std::cout << "Initialising UART interface..." << std::endl;
    if (!uArt.init()) {
        std::cerr << "Failed to initialise UART interface." << std::endl;
        return false;
    }
    std::cout << "... UART interface successfully initialised." << std::endl;

    // Check if the SIM has been registered.
    std::cout << "Starting SIM registration checks..." << std::endl;
    bool simRegistered = false;
    if (!hasRegistered(simRegistered)) {
        std::cerr << "Failed SIM registration checks." << std::endl;
        return false;
    } else if (!simRegistered) {
        std::cout << "Starting automatic SIM registration..." << std::endl;
        if (!startAutoRegistration(simRegistered)) {
            std::cerr << "Failed to start automatic SIM registration." << std::endl;
            return false;
        } else if (!simRegistered) {
            std::cerr << "Failed to register SIM." << std::endl;
            return false;
        }
        std::cout << "... SIM successfully registered." << std::endl;
    }
    std::cout << "... SIM registration checks finished." << std::endl;

    // Check if GPRS is attached.
    std::cout << "Starting GPRS checks..." << std::endl;
    bool gprsAttached = false;
    if (!hasGPRS(gprsAttached)) {
        std::cerr << "Failed GPRS checks." << std::endl;
        return false;
    }
    std::cout << "GPRS attached? " << gprsAttached << std::endl
              << "... GPRS checks finished." << std::endl;

    // Check if there is an internet connection.
    std::cout << "Starting PSD checks..." << std::endl;
    bool psdConnected = false;
    std::string psdUrc;
    if (!hasPSD(psdConnected)) {
        std::cerr << "Failed PSD checks." << std::endl;
        return false;
    } else if (!psdConnected || !gprsAttached) {
        std::cout << "Initialising PSD connection..." << std::endl;
        if (!connectPSD(psdConnected, psdUrc) || !psdConnected) {
            std::cerr << "Failed to establish PSD connection." << std::endl;
            return false;
        }
        std::cout << "... PSD connection successfully initialised." << std::endl;
    }
    std::cout << "... PSD checks finished." << std::endl;

    // Configure the sending of messages.
    std::cout << "Setting send message mode..." << std::endl;
    if (!setSendMessageMode(SEND_TEXT_MODE_TEXT)) {
        std::cerr << "Failed to set send message mode." << std::endl;
        return false;
    }
    std::cout << "... send message mode successfully set." << std::endl;

    // Configure the scan mode for obtaining the location.
    std::cout << "Setting location scan mode..." << std::endl;
    if (!setLocationScanMode(LOC_SCAN_MODE_DEEP)) {
        std::cerr << "Failed to set the location scan mode." << std::endl;
        return false;
    }
    std::cout << "... location scan mode successfully set." << std::endl;

    return true;
}

/**
 * Getter for the UART interface through which the commands are sent to the device.
 *
 * @return An instance of the UArt interface via which the commands are sent
 *      to the device.
 */
const UArt& UBlox::getUArt()
{
    return uArt;
}

/**
 * Invoked to determine whether or not the device SIM has been registered
 * with the GSM network.
 *
 * @param registered The bool reference into which the result is to be stored.
 * @return True if the SIM has been registered with the GSM network, false
 *      otherwise.
 */
bool UBlox::hasRegistered(bool &registered)
{
    // Write the command for checking if the user has registered.
    ssize_t rc = writeCommand(AT_CMD_HAS_REGISTERED);
    if (rc == -1) {
        return false;
    }

    // Read the raw response.
    rc = readRawResponse(RX_TIMEOUT_CMD_HAS_REGISTERED);
    if (rc == -1) {
        return false;
    }

    // Check that we have registered for the network.
    if (strncmp(buffer, AT_CMD_RESPONSE_HAS_REGISTERED,
                strlen(AT_CMD_RESPONSE_HAS_REGISTERED)) == 0) {
        registered = true;
    } else if (strncmp(buffer, AT_CMD_RESPONSE_HAS_NOT_REGISTERED,
                       strlen(AT_CMD_RESPONSE_HAS_NOT_REGISTERED)) == 0) {
        registered = false;
    } else {
        return false;
    }

    // Read the status command status response.
    return readStatusResponse(true) == AT_CMD_STATUS_CODE_OK;
}

/**
 * Getter for determining whether or not GPRS is attached.
 *
 * @param attached The bool reference into which the result is to be stored.
 * @return True if the result was successfully obtained, false otherwise.
 */
bool UBlox::hasGPRS(bool &attached)
{
    // Write the command to the device via uArt.
    ssize_t rc = writeCommand(AT_CMD_GET_GPRS_ATTACHED);
    if (rc == -1) {
        return false;
    }

    // Check whether the device has been attached.
    rc = readRawResponse(RX_TIMEOUT_CMD_GET_GPRS_ATTACHED);
    if (rc == -1) {
        return false;
    }

    // Determine whether the GPRS has been attached.
    if (strncmp(buffer, AT_CMD_RESPONSE_GPRS_IS_ATTACHED,
                strlen(AT_CMD_RESPONSE_GPRS_IS_ATTACHED)) == 0) {
        attached = true;
    } else if (strncmp(buffer, AT_CMD_RESPONSE_GPRS_NOT_ATTACHED,
                       strlen(AT_CMD_RESPONSE_GPRS_NOT_ATTACHED)) == 0) {
        attached = false;
    } else {
        return false;
    }

    // Finally, determine the status of the command.
    return readStatusResponse(true) == AT_CMD_STATUS_CODE_OK;
}

/**
 * Getter for determining whether or not PSD (internet) is connected.
 *
 * @param connected The bool reference into which the result is to be stored.
 * @return True if the result was successfully obtained, false otherwise.
 */
bool UBlox::hasPSD(bool &connected)
{
    // Write the command to the device via uArt.
    ssize_t rc = writeCommand(AT_CMD_GET_PSD_CONNECTED);
    if (rc == -1) {
        return false;
    }

    // Check whether the device has an internet connection.
    rc = readRawResponse(RX_TIMEOUT_CMD_GET_PSD_CONNECTED);
    if (rc == -1) {
        return false;
    }

    // Determine if PSD has been connected.
    if (strncmp(buffer, AT_CMD_RESPONSE_PSD_IS_CONNECTED,
                strlen(AT_CMD_RESPONSE_PSD_IS_CONNECTED)) == 0) {
        connected = true;
    } else if (strncmp(buffer, AT_CMD_RESPONSE_PSD_NOT_CONNECTED,
                       strlen(AT_CMD_RESPONSE_PSD_NOT_CONNECTED)) == 0) {
        connected = false;
    } else {
        return false;
    }

    // Finally, determine the status of the command.
    return readStatusResponse(true) == AT_CMD_STATUS_CODE_OK;
}

/**
 * Force start automatic registration of the network. Note, the function will
 * implicitly invoke the hasRegistered function to determine whether the SIM
 * has been registered.
 *
 * @param registered A boolean reference for storing the result.
 * @return True if the command was successfully processed, false otherwise.
 */
bool UBlox::startAutoRegistration(bool &registered)
{
    // Force start auto registration.
    ssize_t rc = writeCommand(AT_CMD_START_AUTO_REGISTRATION);
    if (rc == -1) {
        return false;
    }

    // Check whether the command executed successfully.
    if (readStatusResponse(false) != AT_CMD_STATUS_CODE_OK) {
        return false;
    }

    // Finally check whether the device has registered.
    return hasRegistered(registered);
}

/**
 * Attempt to connect the device to the internet. This should be performed
 * regularly if the location is to be obtained.
 *
 * @param connected The bool reference into which the result is to be stored i.e.
 *      whether or not the internet was successfully connected.
 * @param urc Additional data returned regarding the connection.
 * @return True if the result was successfully obtained, false otherwise.
 */
bool UBlox::connectPSD(bool &connected, std::string &urc)
{
    // Write the command to activate PSD.
    ssize_t rc = writeCommand(AT_CMD_SET_PSD_CONNECTION);
    if (rc == -1) {
        return false;
    }

    // Read the status of the connection.
    const char* status = readStatusResponse(false);
    if (status == AT_CMD_STATUS_CODE_OK) {
        connected = true;
    } else {
        return false;
    }

    // Read the URC response.
    rc = readRawResponse(RX_TIMEOUT_CMD_SET_PSD_CONNECTION);
    if (rc == -1) {
        return false;
    }
    urc.assign(buffer);

    // We have successfully connected the PSD.
    return true;
}

/**
 * Get the send message mode that the device is operating on.
 *
 * @param mode The char reference into which the mode utilised for sending of messages
 *      (SEND_TEXT_MODE_PDU or SEND_TEXT_MODE_TEXT) is to be stored.
 * @return True if the message mode was successfully obtained, false
 *      otherwise.
 */
bool UBlox::getSendMessageMode(char &mode)
{
    // Write the command to get the send message mode.
    ssize_t rc = writeCommand(AT_CMD_SEND_MSG_GET_MODE);
    if (rc == -1) {
        return false;
    }

    // Read the response from the device.
    rc = readRawResponse(RX_TIMEOUT_CMD_GET_SEND_MSG_MODE);
    if (rc == -1) {
        return false;
    }

    // Determine which of the modes has been utilised.
    if (strncmp(buffer, AT_CMD_RESPONSE_SEND_MSG_MODE_TEXT,
                strlen(AT_CMD_RESPONSE_SEND_MSG_MODE_TEXT)) == 0) {
        mode = SEND_TEXT_MODE_TEXT;
    } else if (strncmp(buffer, AT_CMD_RESPONSE_SEND_MSG_MODE_PDU,
                       strlen(AT_CMD_RESPONSE_SEND_MSG_MODE_PDU)) == 0) {
        mode = SEND_TEXT_MODE_PDU;
    } else {
        return false;
    }

    // Finally, determine the status of the command.
    return readStatusResponse(true) == AT_CMD_STATUS_CODE_OK;
}

/**
 * Function permits the sending of messages mode to be reconfigured.
 *
 * @param sendMessageMode The mode that's to be set; see SEND_TEXT_MODE_TEXT /
 *      SEND_TEXT_MODE_PDU.
 * @return True if the mode was successfully set, false otherwise.
 */
bool UBlox::setSendMessageMode(const char sendMessageMode)
{
    // Set the mode of the text messages.
    const char* const sendMsgModeCmd = sendMessageMode == SEND_TEXT_MODE_TEXT
                                   ? AT_CMD_SEND_MSG_SET_MODE_TEXT
                                   : AT_CMD_SEND_MSG_SET_MODE_PDU;
    ssize_t rc = writeCommand(sendMsgModeCmd);
    if (rc == -1) {
        return false;
    }

    // Check that the status was OK.
    return readStatusResponse(false) == AT_CMD_STATUS_CODE_OK;
}

/**
 * Get the scan mode utilised for obtaining the location of the device.
 *
 * @param scanMode The char reference into which the mode that is utilised
 *      (LOC_SCAN_MODE_NORMAL or LOC_SCAN_MODE_DEEP) is to be stored.
 * @return True if the location scan mode was obtained successfully, false
 *      otherwise.
 */
bool UBlox::getLocationScanMode(char &scanMode)
{
    // Write the command for obtaining the location scan mode.
    ssize_t rc = writeCommand(AT_CMD_GET_LOC_SCAN_MODE);
    if (rc == -1) {
        return false;
    }

    // Read the response obtained from the device.
    rc = readRawResponse(RX_TIMEOUT_CMD_GET_LOC_SCAN_MODE);
    if (rc == -1) {
        return false;
    }

    // Determine the mode utilised.
    if (strncmp(buffer, AT_CMD_RESPONSE_LOC_SCAN_MODE_NORMAL,
                strlen(AT_CMD_RESPONSE_LOC_SCAN_MODE_NORMAL)) == 0) {
        scanMode = LOC_SCAN_MODE_NORMAL;
    } else if (strncmp(buffer, AT_CMD_RESPONSE_LOC_SCAN_MODE_DEEP,
                       strlen(AT_CMD_RESPONSE_LOC_SCAN_MODE_DEEP)) == 0) {
        scanMode = LOC_SCAN_MODE_DEEP;
    } else {
        return false;
    }

    // Finally, read the status of the command.
    return readStatusResponse(true) == AT_CMD_STATUS_CODE_OK;
}

/**
 * Set the location scan mode for the device.
 *
 * @param scanMode The type of scan mode (LOCATION_SCAN_MODE_NORMAL or
 *      LOCATION_SCAN_MODE_DEEP) that's to be utilised for obtaining the location
 *      of the device.
 * @return True if the location scan mode was usccessfully set, false otherwise.
 */
bool UBlox::setLocationScanMode(const char scanMode)
{
    // Set the location scan mode.
    const char* const locScanModeCmd = scanMode == LOC_SCAN_MODE_NORMAL
                                       ? AT_CMD_SET_LOC_SCAN_MODE_NORMAL
                                       : AT_CMD_SET_LOC_SCAN_MODE_DEEP;
    ssize_t rc = writeCommand(locScanModeCmd);
    if (rc == -1) {
        return false;
    }

    // Check that the status is OK.
    return readStatusResponse(false) == AT_CMD_STATUS_CODE_OK;
}

/**
 * Get the model number of the u-blox device.
 *
 * @param modelNumber The string reference into which the model number
 *      is to be stored.
 * @return True if the model number was successfully returned, false otherwise.
 */
bool UBlox::getModelNumber(std::string &modelNumber)
{
    // Write the command to the device via uArt.
    ssize_t rc = writeCommand(AT_CMD_GET_MODEL_NUMBER);
    if (rc == -1) {
        return false;
    }

    // Read the response from the chip.
    rc = readRawResponse(RX_TIMEOUT_CMD_GET_MODEL_NUMBER);
    if (rc == -1) {
        return false;
    }
    modelNumber.assign(buffer, strlen(buffer) - 1);

    // Read the status of the command.
    return readStatusResponse(true) == AT_CMD_STATUS_CODE_OK;
}

/**
 * Get the IMEI of the u-blox device.
 *
 * @param imeiNumber The string reference into which the IMEI code is to be
 *      stored.
 * @return True if the IMEI was successfully returned, false otherwise.
 */
bool UBlox::getIMEI(std::string &imeiNumber)
{
    // Write a command and check that is has been successfully written.
    ssize_t rc = writeCommand(AT_CMD_GET_IMEI);
    if (rc == -1) {
        return false;
    }

    // Read the IMEI number from the chip.
    rc = readRawResponse(RX_TIMEOUT_CMD_GET_IMEI);
    if (rc == -1) {
        return false;
    }
    imeiNumber.assign(buffer, strlen(buffer) - 1);

    // Finally, return the status of the command.
    return readStatusResponse(true) == AT_CMD_STATUS_CODE_OK;
}

/**
 * Get the CellLocation location from the u-blox device. Note,
 * this function may take up to 3 minutes in order to return a location.
 *
 * @param lat The double reference into which the latitude is to be stored.
 * @param lng The double reference into which the longitude is to be stored.
 * @return True if the device was able to successfully get the latitude and longitude
 *      of the device, false otherwise.
 */
bool UBlox::getLocation(double &lat, double &lng)
{
    // Attempt to get the location from the device.
    ssize_t rc = writeCommand(AT_CMD_GET_LOC);
    if (rc == -1) {
        return false;
    }

    // Read the status of the command.
    const char* const status = readStatusResponse(false);
    if (status != AT_CMD_STATUS_CODE_OK) {
        return false;
    }

    // Read the first two bytes (\r\n).
    rc = readRawResponse(RX_TIMEOUT_CMD_GET_LOCATION);
    if (rc == -1) {
        return false;
    }

    // Read the actual response.
    rc = readRawResponse(RX_TIMEOUT_CMD_GET_LOCATION);
    if (rc == -1) {
        return false;
    }

    char* nextToken = buffer;

    // Discard the date and time fields of the response.
    char* token = strtok_r(buffer, ",", &nextToken);
    strtok_r(nullptr, ",", &nextToken);

    // Get the latitude.
    token = strtok_r(nullptr, ",", &nextToken);
    lat = std::strtod(token, nullptr);

    // Get the longitude.
    token = strtok_r(nullptr, ",", &nextToken);
    lng = std::strtod(token, nullptr);

    return true;
}

/**
 * Compose an outgoing message - sent via the UART interface to the device
 * which in turn is sent by the said UBlox device.
 *
 * @param phoneNumber The phone number to which the message is sent.
 * @param message The message that is to be sent.
 * @return True if the message was successfully sent, false otherwise.
 */
bool UBlox::sendMessage(const std::string &phoneNumber, const std::string &message)
{
    // Format the phone number command.
    size_t phoneNumberCmdLen = strlen(AT_CMD_SEND_MSG_SET_NUMBER) + phoneNumber.size() - 1;
    char phoneNumberCmd[phoneNumberCmdLen];
    memset(phoneNumberCmd, '\0', phoneNumberCmdLen);
    sprintf(phoneNumberCmd, AT_CMD_SEND_MSG_SET_NUMBER, phoneNumber.c_str());

    // Write the phone number command to the device.
    ssize_t rc = writeCommand(phoneNumberCmd);
    if (rc == -1) {
        return false;
    }

    // Write the message to the device.
    rc = uArt.writeNext(message);
    if (rc == -1) {
        uArt.writeNext(AT_CMD_SEND_MSG_SET_ESC);
        return false;
    }

    // Write the end message cmd to the device.
    rc = uArt.writeNext(AT_CMD_SEND_MSG_SET_END);
    if (rc == -1) {
        uArt.writeNext(AT_CMD_SEND_MSG_SET_ESC);
    }

    // Await the echo from the device.
    rc = uArt.readNext(buffer, AT_CMD_BUFF_LEN, RX_TIMEOUT_NETWORK);
    if (rc == -1 || strncmp(message.c_str(), buffer, message.size()) != 0) {
        return false;
    }

    // Read the initial response.
    rc = readRawResponse(RX_TIMEOUT_ECHO);
    if (rc == -1) {
        return false;
    }

    // Read the final status and check if its okay.
    return readStatusResponse(true) == AT_CMD_STATUS_CODE_OK;
}

bool UBlox::sendLocation(const std::string &phoneNumber, const double lat,
                         const double lng)
{
    // TODO: Implement the functionality that permits the location to be sent and displayed, live.
    return false;
}

/**
 * Writes the next command to u-blox via the uart interface.
 * However, it should be noted that the rx buffer is cleared prior
 * to reading the echo.
 *
 * @param command The command that is to be written via uart to the u-blox
 *      device. Note, must end in \r.
 * @return -1 if there was an error, n (representing length of the echo) otherwise.
 */
ssize_t UBlox::writeCommand(const char *command)
{
    // Write the command to the device.
    ssize_t rc = uArt.writeNext(command);
    if (rc == -1) {
        return -1;
    }

    // Read the raw echo response and check lengths to determine if echoed.
    rc = readRawResponse(RX_TIMEOUT_ECHO);
    if (rc == -1) {
        return -1;
    }
    return strlen(command) == rc;
}

/**
 * Read the next response from the uart interface into the rx buffer.
 * Note, the rx buffer is cleared (set to null terminators) prior to
 * reading the response from the said interface.
 *
 * @param timeout The timeout in ms prior to discarding the query.
 * @return -1 if there was an error, n (representing length) otherwise.
 */
ssize_t UBlox::readRawResponse(int timeoutMs)
{
    // Clear the buffer and read the response from the device.
    clearResponseBuff();
    return uArt.readNext(buffer, AT_CMD_BUFF_LEN, timeoutMs);
}

/**
 * Read the next response from the uart interface into the rx buffer.
 * Additionally, interpret the response as a status and return a pointer
 * to the C string representing the status of the command.
 *
 * @return nullptr if the response could not be interpreted, a pointer
 *      otherwise.
 */
const char* UBlox::readStatusResponse(bool crlf)
{
    // Occasionally there are preceding \r\n; read and discard.
    ssize_t rc = -1;
    if (crlf) {
        rc = readRawResponse(RX_TIMEOUT);
        if (rc != 2) {
            return nullptr;
        }
    }

    // Generically read the device response and attempt to resolve the status.
    rc = readRawResponse(RX_TIMEOUT_STATUS);
    if (rc == -1) {
        return nullptr;
    }
    return checkResponseBuffStatus();
}

/**
 * Attempt to determine the response status from the device that's
 * currently within the buffer - it may or may not be a status response.
 *
 * These functions should the treated as a state machine.
 *
 * @return A pointer to a resolved status code, nullptr otherwise.
 */
const char* const UBlox::checkResponseBuffStatus()
{
    // Match a status by comparing the strings.
    if (strncmp(buffer, AT_CMD_STATUS_CODE_OK,
                strlen(AT_CMD_STATUS_CODE_OK)) == 0) {
        return AT_CMD_STATUS_CODE_OK;
    } else if (strncmp(buffer, AT_CMD_STATUS_CODE_ERROR,
                       strlen(AT_CMD_STATUS_CODE_ERROR)) == 0) {
        return AT_CMD_STATUS_CODE_ERROR;
    } else if (strncmp(buffer, AT_CMD_STATUS_CODE_ABORTED,
                       strlen(AT_CMD_STATUS_CODE_ABORTED)) == 0) {
        return AT_CMD_STATUS_CODE_ABORTED;
    } else {
        return nullptr;
    }
}

/**
 * Clear the response buffer i.e. set all elements of the array
 * to null terminators.
 */
void UBlox::clearResponseBuff()
{
    std::memset(buffer, '\0', AT_CMD_BUFF_LEN);
}

