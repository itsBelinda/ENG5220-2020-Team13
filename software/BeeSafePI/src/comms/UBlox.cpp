#include <boost/compatibility/cpp_c_headers/cstring>
#include "UBlox.h"

#define  MAX_CMD_LENGTH 544
// Define the AT commands that are used on the U-Blox device.
#define AT_COMMAND_GET_MODEL_NUMBER "ATI0\r\n"
#define AT_COMMAND_GET_IMEI "AT+GSN1\r\n"
#define AT_COMMAND_GET_LOCATION ""

// Define the states.
#define AT_STATUS_OK "OK"
#define AT_STATUS_ERROR "ERROR"
#define AT_STATUS_ABORTED "ABORTED"

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
        printf("Command successfully echoed\n");
    }



    // Read the echo, imei and status back from the device.
    char imeiBuffer[SZ_RESPONSE_IMEI] = {'\0'};
    nRx = uart.readBuffer(imeiBuffer, SZ_RESPONSE_IMEI, RX_TIMEOUT);

    //TODO: do in separate function
    if (nRx != SZ_RESPONSE_IMEI) {//TODO: check formatting of return first|| strcmp(AT_COMMAND_GET_IMEI,echoBuffer)) {
        printf("unexpected answer received: %s\n", imeiBuffer);
        return -1;
    } else {
        printf("Answer successfully received\n");
    }

    char statusBuffer[SZ_RESPONSE_STATUS] = {'\0'};
    nRx = uart.readBuffer(statusBuffer, SZ_RESPONSE_STATUS, RX_TIMEOUT);

    //TODO: do in separate function
    if (nRx != SZ_RESPONSE_IMEI) {//TODO: check formatting of return first|| strcmp(AT_COMMAND_GET_IMEI,echoBuffer)) {
        printf("unexpected answer received: %s\n", statusBuffer);
        return -1;
    } else {
        printf("Answer successfully received\n");
    }


    return 0;
    return -1;
}

int UBlox::getIMEI2(std::string &imei)
{
    bool result = false;
    char replyBuffer[MAX_CMD_LENGTH];
    printf("Writing command IMEI\n");

    // Write the at command via uart.
    result = sendCmd(AT_COMMAND_GET_IMEI);
    if (result != true) {
        return -1; // TODO: error codes? or handle here?
    }

    printf("Command written\n");

    result = getReply(replyBuffer, SZ_RESPONSE_IMEI);
    if (result != true) {
        return -1; // TODO: error codes? or handle here?
    }

    printf("Response received\n");

    result = checkStatus();
    if (result != true) {
        return -1; // TODO: error codes? or handle here?
    }

    imei = replyBuffer; // char array to string //todo: trailing zeros?
    printf("Status ok received\n");

    return 0;
}

int UBlox::getLocation(double &lat, double &lng)
{
    return -1;
}


bool UBlox::sendCmd(const char *const cmdBuffer)
{
    int expSize = sizeof(cmdBuffer);
    char echoBuffer[sizeof(cmdBuffer)] = {'\0'};

    // Write the at command via uart.
    ssize_t nTx = uart.writeBuffer(cmdBuffer);
    if (nTx == -1) {
        return false;
    }

    printf("Command written\n");
    ssize_t nRx = uart.readBuffer(echoBuffer, expSize, RX_TIMEOUT);

    if (nRx != nTx || strcmp(cmdBuffer, echoBuffer)) {
        printf("unexpected echo cmd: sent %s, received: %s\n", AT_COMMAND_GET_IMEI, echoBuffer);
        return false;
    } else {
        printf("Command was successfully echoed %s\n", echoBuffer);
    }

    return true;
}

// todo: possilby get rid of siez
bool UBlox::getReply(char *const replyBuffer, int expSize) // TODO: what happens if buffer size too small? fixed
// size?
// static?
{
    ssize_t nRx = uart.readBuffer(replyBuffer, expSize, RX_TIMEOUT);
    if (nRx != expSize) {//TODO:
        printf("unexpected answer received: %s\n", replyBuffer);
        return false;
    } else {
        printf("Answer successfully received\n");
    }

    return true;
}

bool UBlox::checkStatus()
{
    char statusBuffer[SZ_RESPONSE_STATUS] = {'\0'};
    ssize_t nRx = uart.readBuffer(statusBuffer, SZ_RESPONSE_STATUS, RX_TIMEOUT);

    if (nRx != SZ_RESPONSE_STATUS) {
        printf("unexpected answer received: %s\n", statusBuffer);
        return false;
    } else {
        std::string reply = statusBuffer;
        if (findCharArray(AT_STATUS_OK, statusBuffer) == true) {
            printf("Status OK\n");
            return true;
        } else if (findCharArray(AT_STATUS_ERROR, statusBuffer) == true) {
            printf("Status ERROR\n"); // TODO: handle here or error code?
            // TODO: error status needs to be handled
            return false;
        } else if (findCharArray(AT_STATUS_OK, statusBuffer) == true) {
            // TODO: aborted status needs to be handled
            printf("Status ABORTED\n"); // TODO: handle here or error code?
            return false;
        }

    }
}

// todo helper
bool UBlox::findCharArray(const char *const needle, const char *const haystack)
{
    const char *result = NULL;
    //std::string::find()
    result = std::strstr(needle, haystack);
    if (result != NULL) {
        return true;
    }
    return false;
}