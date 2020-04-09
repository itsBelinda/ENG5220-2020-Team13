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
    conf();
}

UBlox::~UBlox() = default;

int UBlox::conf()
{
    return -1;
}

UArt &UBlox::getUArtInterface()
{
    return uart;
}

int UBlox::getUArtDevice()
{
    return uart.getDevice();
}

bool UBlox::isUArtOpen()
{
    return uart.isDeviceOpen();
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

    // Write the command to via uart, check if successfully written.
    ssize_t rc = writeNext(AT_CMD_GET_MODEL_NUMBER);
    if (rc == -1) {
        return false;
    }

    // Read the model number from the chip.
    rc = readNext(RX_TIMEOUT_CMD_GET_MODEL_NUMBER);
    if (rc == -1) {
        return false;
    }
    modelNumber.assign(rxBuffer, strlen(rxBuffer));

    // Print the model number...
    std::cout << modelNumber << std::endl;

    // Get the status from the chip.
    if ((rc = readNext(RX_TIMEOUT)) != 2) {
        return false;
    } else if ((rc = readNext(RX_TIMEOUT_STATUS)) != 4) {
        return false;
    }

    return true;
}

bool UBlox::getIMEI(std::string &imei)
{
    // Write a command and check that is has been successfully written.
    ssize_t rc = writeNext(AT_CMD_GET_IMEI);
    if (rc == -1) {
        return false;
    }

    // Read the IMEI number from the chip.
    rc = readNext(RX_TIMEOUT_CMD_GET_IMEI);
    if (rc == -1) {
        return false;
    }
    imei.assign(rxBuffer, strlen(rxBuffer));

    std::cout << imei << std::endl;

    // Get the status of the command.
    if ((rc = readNext(RX_TIMEOUT)) != 2) {
        return false;
    } else if ((rc = readNext(RX_TIMEOUT_STATUS)) != 4) {
        return false;
    }

    printf("%s\n", rxBuffer);

    return true;
}

bool UBlox::getLocation(double &lat, double &lng)
{
    // Check connections, these should be enabled.
    // Activate any if necessary.
    // Send the message for getting the location, await response.
    // If we timeout, do we abort?
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
ssize_t UBlox::writeNext(const char * const command)
{

    // Attempt to write the command.
    ssize_t rc = uart.writeNext(command);
    if (rc == -1) {
        return -1;
    }

    // Commands always echo, read it.
    rc = readNext(RX_TIMEOUT_ECHO);
    if (rc == -1) {
        return -1;
    }

    // If the echo is the same length, command successfully written.
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
ssize_t UBlox::readNext(int timeout)
{
    clearRx();
    return uart.readNext(rxBuffer, AT_CMD_BUFF_LEN, timeout);
}

/**
 * Clear the RX buffer; usually done prior to reading the next
 * response from the uart interface.
 */
void UBlox::clearRx()
{
    for (auto& byte : rxBuffer) {
        byte = '\0';
    }
}

char UBlox::checkRxStatus()
{
    return -1;
}



