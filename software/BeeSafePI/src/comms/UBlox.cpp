#include "UBlox.h"

// Define the AT commands that are used on the U-Blox device.
#define AT_COMMAND_GET_MODEL_NUMBER "ATI0\r\n"
#define AT_COMMAND_GET_IMEI "AT+GSN1\r\n"
#define AT_COMMAND_GET_LOCATION ""

// Define the responses.
#define AT_RESPONSE_OK "OK"
#define AT_RESPONSE_ERROR "ERROR"

// Define expected response sizes
#define SZ_RESPONSE_IMEI 17 //TODO: check
#define SZ_RESPONSE_STATUS 4 //TODO: check

// Define timeouts
#define RX_TIMEOUT 4000 // timeout in ms


UBlox::UBlox()
{
    conf();
}

UBlox::~UBlox() = default;

int UBlox::conf()
{
    return -1;
}

Uart &UBlox::getUart()
{
    return uart;
}

int UBlox::getDevice()
{
    return uart.getDevice();
}

bool UBlox::isOpen()
{
    return uart.isOpen();
}

int UBlox::getModelNumber(std::string &modelNumber)
{

    printf("Writing command\n");

    // Write the at command via uart.
    ssize_t rc = uart.writeBuffer(AT_COMMAND_GET_MODEL_NUMBER);
    if (rc == -1) {
        return -1;
    }

    printf("Command written\n");

    // Read the echo, model number and status back from the device.
    char modelNumberBuffer[7] = {'\0'};
    rc = uart.readBuffer(modelNumberBuffer, 7, 4000);

    printf("Command successfully echoed");

    printf("Read, %d Echo: %s\n", (int) rc, modelNumberBuffer);

    return rc;
}

int UBlox::getIMEI(std::string &imei)
{

    printf("Writing command IMEI\n");

    // Write the at command via uart.
    ssize_t nTx = uart.writeBuffer(AT_COMMAND_GET_IMEI);
    if (nTx == -1) {
        return -1;
    }

    printf("Command written\n");

    char echoBuffer[sizeof(AT_COMMAND_GET_IMEI)] = {'\0'}; // TODO: doesn't let me use nTx
    ssize_t nRx = uart.readBuffer(echoBuffer, sizeof(AT_COMMAND_GET_IMEI), RX_TIMEOUT);

    //TODO: do in separate function
    if (nRx != nTx) {//TODO: check formatting of return first|| strcmp(AT_COMMAND_GET_IMEI,echoBuffer)) {
        printf("unexpected echo cmd: sent %s, received: %s\n", AT_COMMAND_GET_IMEI, echoBuffer);
        return -1;
    } else {
        printf("Command successfully echoed");
    }



    // Read the echo, imei and status back from the device.
    char imeiBuffer[SZ_RESPONSE_IMEI] = {'\0'};
    nRx = uart.readBuffer(imeiBuffer, SZ_RESPONSE_IMEI, RX_TIMEOUT);

    //TODO: do in separate function
    if (nRx != SZ_RESPONSE_IMEI) {//TODO: check formatting of return first|| strcmp(AT_COMMAND_GET_IMEI,echoBuffer)) {
        printf("unexpected answer received: %s\n",  imeiBuffer);
        return -1;
    } else {
        printf("Answer successfully received");
    }

    char statusBuffer[SZ_RESPONSE_STATUS] = {'\0'};
    nRx = uart.readBuffer(statusBuffer, SZ_RESPONSE_STATUS, RX_TIMEOUT);

    //TODO: do in separate function
    if (nRx != SZ_RESPONSE_IMEI) {//TODO: check formatting of return first|| strcmp(AT_COMMAND_GET_IMEI,echoBuffer)) {
        printf("unexpected answer received: %s\n",  statusBuffer);
        return -1;
    } else {
        printf("Answer successfully received");
    }


    return 0;
    return -1;
}

int UBlox::getLocation(double &lat, double &lng)
{
    return -1;
}
