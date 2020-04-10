#include <boost/compatibility/cpp_c_headers/cstring>
#include <iostream>

#include "string.h"

#include "UBlox.h"

// Define generic u-blox commands for obtaining additional information.
#define AT_CMD_GET_MODEL_NUMBER "ATI\r"
#define AT_CMD_GET_IMEI "ATI5\r"

// Commands for checking if the necessary components are present / enabled.
#define AT_CMD_GPRS_ATTACHED "AT+CGATT?\r"
#define AT_CMD_ATTACH_GPRS ""
#define AT_CMD_PSD_CONNECTED "AT+UPSND=0,8\r"
#define AT_CMD_CONNECT_PSD "AT+UPSDA=0,3\r"

// Location specific commands.
#define AT_CMD_SET_LOCATION_SCAN_MODE_DEEP "AT+ULOCCELL=1\r"
#define AT_CMD_GET_LOCATION "AT+ULOC=2,2,0,120,500\r"

// Define the response states (from u-blox).
#define AT_RESPONSE_STATUS_OK "OK"
#define AT_RESPONSE_STATUS_ERROR "ERROR"
#define AT_RESPONSE_STATUS_ABORTED "ABORTED"

// Define responses to commands.
#define AT_RESPONSE_PSD_CONNECTED ""
#define AT_RESPONSE_PSD_NOT_CONNECTED ""

// Define timeouts for the commands.
#define RX_TIMEOUT 10
#define RX_TIMEOUT_ECHO 1000
#define RX_TIMEOUT_STATUS 1000

#define RX_TIMEOUT_CMD_GET_MODEL_NUMBER 1000
#define RX_TIMEOUT_CMD_GET_IMEI 1000
#define RX_TIMEOUT_CMD_IS_GPRS_ATTACHED 1000
#define RX_TIMEOUT_CMD_IS_PSD_CONNECTED 1000

UBlox::UBlox()
{
    configure();
}

UBlox::~UBlox() = default;

int UBlox::configure()
{
    return -1;
}

const UArt& UBlox::getUArt()
{
    return uart;
}

bool UBlox::hasGPRS()
{
    return false;
}

bool UBlox::hasPSD()
{
    return false;
}

bool UBlox::attachGPRS()
{
    return false;
}

bool UBlox::connectPSD()
{
    return false;
}

bool UBlox::getModelNumber(std::string &modelNumber)
{
    // Write the command to the device via uart.
    ssize_t rc = writeCommand(AT_CMD_GET_MODEL_NUMBER);
    if (rc == -1) {
        return false;
    }

    // Read the response from the chip.
    rc = readResponse(RX_TIMEOUT_CMD_GET_MODEL_NUMBER);
    if (rc == -1) {
        return false;
    }
    modelNumber.assign(buffer, strlen(buffer));
    std::cout << modelNumber << std::endl;

    // Read the status of the command.
    return readResponseStatus(true) == AT_RESPONSE_STATUS_OK;
}

bool UBlox::getIMEI(std::string &imei)
{
    // Write a command and check that is has been successfully written.
    ssize_t rc = writeCommand(AT_CMD_GET_IMEI);
    if (rc == -1) {
        return false;
    }

    // Read the IMEI number from the chip.
    rc = readResponse(RX_TIMEOUT_CMD_GET_IMEI);
    if (rc == -1) {
        return false;
    }
    imei.assign(buffer, strlen(buffer));
    std::cout << imei << std::endl;

    // Finally, return the status of the command.
    return readResponseStatus(true) == AT_RESPONSE_STATUS_OK;
}

bool UBlox::getLocation(double &lat, double &lng)
{
    return true;
}

bool UBlox::sendMessage(std::string &phoneNumber, std::string &message)
{
    return false;
}

bool UBlox::sendLocation(std::string &phoneNumber, double lat, double lng)
{
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
    ssize_t rc = uart.writeNext(command);
    if (rc == -1) {
        return -1;
    }

    // Read the response i.e. echo. If present, command successful.
    rc = readResponse(RX_TIMEOUT_ECHO);
    if (rc == -1) {
        return -1;
    }

    // If the lengths match, command successfully echoed.
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
ssize_t UBlox::readResponse(const int timeoutMs)
{
    // Clear the buffer and read the response from the device.
    clearResponseBuff();
    return uart.readNext(buffer, AT_CMD_BUFF_LEN, timeoutMs);
}

/**
 * Read the next response from the uart interface into the rx buffer.
 * Additionally, interpret the response as a status and return a pointer
 * to the C string representing the status of the command.
 *
 * @return nullptr if the response could not be interpreted, a pointer
 *      otherwise.
 */
const char* UBlox::readResponseStatus(const bool crlf)
{
    // An additional \r\n is expected before the status.
    ssize_t rc = -1;
    if (crlf) {
        rc = readResponse(RX_TIMEOUT);
        if (rc != 2) {
            return nullptr;
        }
    }

    // Read the response as a generic response.
    rc = readResponse(RX_TIMEOUT_STATUS);
    if (rc == -1) {
        return nullptr;
    }

    // Attempt to resolve the response status within the buffer.
    return resolveResponseBuffStatus();
}

/**
 * Function attempts to interpret the response within the buffer as a
 * status response i.e. OK, ERROR, ABORTED.
 *
 * @return The status within the buffer, nullptr otherwise.
 */
const char* const UBlox::resolveResponseBuffStatus()
{
    // Compare the response against known status responses.
    if (strncmp(buffer, AT_RESPONSE_STATUS_OK, 2) == 0) {
        return AT_RESPONSE_STATUS_OK;
    } else if (strncmp(buffer, AT_RESPONSE_STATUS_ERROR, 5) == 0) {
        return AT_RESPONSE_STATUS_ERROR;
    } else if (strncmp(buffer, AT_RESPONSE_STATUS_ABORTED, 7) == 0) {
        return AT_RESPONSE_STATUS_ABORTED;
    } else {
        return nullptr;
    }
}

/**
 * Clear the RX buffer; usually done prior to reading the next
 * response from the uart interface.
 */
void UBlox::clearResponseBuff()
{
    // Iterate the buffer, resetting the chars to null terminators.
    for (auto& byte : buffer) {
        byte = '\0';
    }
}

