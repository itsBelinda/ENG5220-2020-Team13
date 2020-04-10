#include <boost/compatibility/cpp_c_headers/cstring>
#include <iostream>

#include "string.h"

#include "UBlox.h"

// Define generic u-blox commands for obtaining additional information.
#define AT_CMD_GET_MODEL_NUMBER "ATI\r"
#define AT_CMD_GET_IMEI "ATI5\r"

// GPRS specific commands.
#define AT_CMD_GET_GPRS_ATTACHED "AT+CGATT?\r"

// Internet specific commands.
#define AT_CMD_GET_PSD_CONNECTED "AT+UPSND=0,8\r"
#define AT_CMD_SET_PSD_CONNECTION "AT+UPSDA=0,3\r"

// Location specific commands.
#define AT_CMD_SET_LOCATION_SCAN_MODE_NORMAL "AT+ULOCCELL=0\r"
#define AT_CMD_SET_LOCATION_SCAN_MODE_DEEP "AT+ULOCCELL=1\r"
#define AT_CMD_GET_LOCATION "AT+ULOC=2,2,0,120,500\r"

// Define the message modes for sending text messages.
#define AT_MSG_MODE_TEXT "AT+CMGF=1\r"
#define AT_MSG_MODE_PDU "AT+CMGF=0\r"

// Define the commands and formats for sending messages.
#define AT_CMD_SEND_MSG_NUMBER "AT+CMGS=\"%s\"\r"
#define AT_CMD_SEND_MSG_ESC "\x1B"
#define AT_CMD_SEND_MSG_END "\x1A"

// Define expected responses from the device.
#define AT_CMD_RESPONSE_GPRS_IS_ATTACHED "+CGATT: 1"
#define AT_CMD_RESPONSE_GPRS_NOT_ATTACHED "+CGATT: 0"
#define AT_CMD_RESPONSE_PSD_IS_CONNECTED "+UPSND: 0,8,1"
#define AT_CMD_RESPONSE_PSD_NOT_CONNECTED "+UPSND: 0,8,0"

// Define the status codes returned by the device.
#define AT_CMD_STATUS_CODE_OK "OK"
#define AT_CMD_STATUS_CODE_ERROR "ERROR"
#define AT_CMD_STATUS_CODE_ABORTED "ABORTED"

// Define timeouts for the commands.
#define RX_TIMEOUT 10
#define RX_TIMEOUT_ECHO 1000
#define RX_TIMEOUT_STATUS 1000
#define RX_TIMEOUT_NETWORK 10000

#define RX_TIMEOUT_CMD_GET_LOCATION 200000
#define RX_TIMEOUT_CMD_GET_MODEL_NUMBER 1000
#define RX_TIMEOUT_CMD_GET_IMEI 1000
#define RX_TIMEOUT_CMD_GET_GPRS_ATTACHED 1000
#define RX_TIMEOUT_CMD_GET_PSD_CONNECTED 1000
#define RX_TIMEOUT_CMD_SET_PSD_CONNECTION 1000

UBlox::UBlox()
{
    configure();
}

UBlox::~UBlox() = default;

int UBlox::configure()
{
    // TODO: Check if GPRS is present,
    // TODO: Check if PSD is present,
    // TODO: Attach PSD if not,
    // TODO: Configure the sending of messagesm
    // TODO: Configure the scanning of location.
    return -1;
}

const UArt& UBlox::getUArt()
{
    return uart;
}

bool UBlox::hasGPRS(bool &attached)
{
    // Write the command to the device via uart.
    ssize_t rc = writeCommand(AT_CMD_GET_GPRS_ATTACHED);
    if (rc == -1) {
        return false;
    }

    // Check whether the device has been attached.
    rc = readResponse(RX_TIMEOUT_CMD_GET_GPRS_ATTACHED);
    if (rc == -1) {
        return false;
    }

    // Determine whether the GPRS has been attached.
    if (strncmp(buffer, AT_CMD_RESPONSE_GPRS_IS_ATTACHED, 9) == 0) {
        attached = true;
    } else if (strncmp(buffer, AT_CMD_RESPONSE_GPRS_NOT_ATTACHED, 9) == 0) {
        attached = false;
    } else {
        return false;
    }

    // Finally, determine the status of the command.
    return readResponseStatus(true) == AT_CMD_STATUS_CODE_OK;
}

bool UBlox::hasPSD(bool &connected)
{
    // Write the command to the device via uart.
    ssize_t rc = writeCommand(AT_CMD_GET_PSD_CONNECTED);
    if (rc == -1) {
        return false;
    }

    // Check whether the device has an internet connection.
    rc = readResponse(RX_TIMEOUT_CMD_GET_PSD_CONNECTED);
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
    return readResponseStatus(true) == AT_CMD_STATUS_CODE_OK;
}

bool UBlox::attachGPRS()
{
    return false;
}

bool UBlox::connectPSD(bool &connected, std::string &urc)
{
    // Write the command to activate PSD.
    ssize_t rc = writeCommand(AT_CMD_SET_PSD_CONNECTION);
    if (rc == -1) {
        return false;
    }

    // Read the status of the connection.
    const char* status = readResponseStatus(false);
    if (status == AT_CMD_STATUS_CODE_OK) {
        connected = true;
    } else {
        return false;
    }

    // Read the URC response.
    rc = readResponse(RX_TIMEOUT_CMD_SET_PSD_CONNECTION);
    if (rc == -1) {
        return false;
    }
    urc.assign(buffer);

    // We have successfully connected the PSD.
    return true;
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
    return readResponseStatus(true) == AT_CMD_STATUS_CODE_OK;
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
    return readResponseStatus(true) == AT_CMD_STATUS_CODE_OK;
}

bool UBlox::getLocation(double &lat, double &lng)
{
    // Attempt to get the location from the device.
    ssize_t rc = writeCommand(AT_CMD_GET_LOCATION);
    if (rc == -1) {
        return false;
    }

    // Read the status of the command.
    const char* const status = readResponseStatus(false);
    printf("Buffer: %s, status: %s\n", buffer, status);
    if (status != AT_CMD_STATUS_CODE_OK) {
        return false;
    }

    // Read the raw location from the device.
    rc = readResponse(RX_TIMEOUT_CMD_GET_LOCATION);
    printf("Response: (%d) %s\n", (int) rc, buffer);
    if (rc == -1) {
        printf("Failed to read the location! %s\n", buffer);
        return false;
    }

    // Print the response to the screen.
    printf("Response: %s\n", buffer);

    return true;
}

bool UBlox::sendMessage(const std::string &phoneNumber, const std::string &message)
{

    // TODO: START: Move this into its own method!
    // Set the text message type.
    ssize_t ct = writeCommand(AT_MSG_MODE_TEXT);
    if (ct == -1) {
        return false;
    }
    // TODO: END: Move this into its own method!


    // Format the command and write it to the device.
    char phoneNumberCmd[strlen(AT_CMD_SEND_MSG_NUMBER) + phoneNumber.size() - 1] = {'\0'};
    sprintf(phoneNumberCmd, AT_CMD_SEND_MSG_NUMBER, phoneNumber.c_str());
    ssize_t rc = writeCommand(phoneNumberCmd);
    if (rc == -1) {
        return false;
    }

    // Write the message to the device.
    rc = uart.writeNext(message);
    if (rc == -1) {
        uart.writeNext(AT_CMD_SEND_MSG_ESC);
        return false;
    }

    // Write the end message cmd to the device.
    rc = uart.writeNext(AT_CMD_SEND_MSG_END);
    if (rc == -1) {
        uart.writeNext(AT_CMD_SEND_MSG_ESC);
    }

    // Await the echo from the device.
    rc = uart.readNext(buffer, AT_CMD_BUFF_LEN, RX_TIMEOUT_NETWORK);
    if (rc == -1 || strncmp(message.c_str(), buffer, message.size()) != 0) {
        return false;
    }

    // Read the status of the command.
    return readResponseStatus(false) == AT_CMD_STATUS_CODE_OK;
}

bool UBlox::sendLocation(const std::string &phoneNumber, const double lat,
                         const double lng)
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
    if (strncmp(buffer, AT_CMD_STATUS_CODE_OK, 2) == 0) {
        return AT_CMD_STATUS_CODE_OK;
    } else if (strncmp(buffer, AT_CMD_STATUS_CODE_ERROR, 5) == 0) {
        return AT_CMD_STATUS_CODE_ERROR;
    } else if (strncmp(buffer, AT_CMD_STATUS_CODE_ABORTED, 7) == 0) {
        return AT_CMD_STATUS_CODE_ABORTED;
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

