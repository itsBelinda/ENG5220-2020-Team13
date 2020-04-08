#include <boost/compatibility/cpp_c_headers/cstring>
#include "UBlox.h"

#define  MAX_CMD_LENGTH 544
// Define the AT commands that are used on the U-Blox device.
#define AT_COMMAND_GET_MODEL_NUMBER "ATI0\r"
#define AT_COMMAND_GET_IMEI "ATI5\r"
#define AT_COMMAND_GET_LOCATION "AT+ULOC=2,3,0,5,1\r"
#define AT_COMMAND_MSG_FORMAT "AT+CMGF1"
#define AT_COMMAND_SEND_MSG "AT+CMGF1"

// Define the states.
#define AT_STATUS_OK "OK"
#define AT_STATUS_ERROR "ERROR"
#define AT_STATUS_ABORTED "ABORTED"

// Define expected response sizes
#define SZ_RESPONSE_IMEI 30 //TODO: check
#define SZ_RESPONSE_STATUS 4 //TODO: check

// Define timeouts
#define ECHO_TIMEOUT 1000 // timeout in ms
#define CMD_TIMEOUT 1000 // timeout in ms
#define RX_TIMEOUT 1000 // timeout in ms


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

    printf("Command written %d\n", (int) rc);

    // Read the echo, imei and status back from the device.
    char modelNumberBuffer[40] = {'\0'};
    rc = uart.readNext(modelNumberBuffer, 40, 1000);
    printf("Rc: %d, Result: %s", (int) rc, modelNumberBuffer);
    rc = uart.readNext(modelNumberBuffer, 40, 1000);
    printf("Rc: %d, Result: %s", (int) rc, modelNumberBuffer);
    rc = uart.readNext(modelNumberBuffer, 40, 1000);
    rc = uart.readNext(modelNumberBuffer, 40, 1000);
    printf("Rc: %d, Result: %s", (int) rc, modelNumberBuffer);

    return rc;
}


int UBlox::getIMEI(std::string &imei)
{
    //TODO: processing of return needed?
    return processCmd(AT_COMMAND_GET_IMEI, imei);
}

int UBlox::getLocation(double &lat, double &lng)
{
    std::string location;
    //TODO: processing of return needed!
    // TODO: still needs testing (probaly setup)
    return processCmd(AT_COMMAND_GET_LOCATION, location);
}


int UBlox::sendMsg(std::string &nbr, std::string &message)
{
    //TODO: can this be configured as standard?
    if (processCmd(AT_COMMAND_MSG_FORMAT) == -1) {
        return -1;
    }

    //TODO: not implemented
    return 0;
}

//TODO: only have one function with option to get response or not.
int UBlox::processCmd(const char *const cmd)
{
    printf("Writing command \n");

    if (!sendCmd(cmd)) {
        return -1;// TODO: error codes? or handle here? why -1 and not true/false? @dan
    }

    if (!checkStatusOK()) {
        return -1;
    }

    return 0;

}

int UBlox::processCmd(const char *const cmd, std::string &response)
{
    printf("Writing command \n");

    if (!sendCmd(cmd)) {
        return -1;// TODO: error codes? or handle here? why -1 and not true/false? @dan
    }

    size_t nRx = uart.readNext(rxBuffer, MAX_BUFFER_LENGTH, ECHO_TIMEOUT); // TODO: need for length? @dan

    if (nRx <= 0) {
        printf("UART read error or timeout\n");
        return -1;
    }

    printf("answer read: %s\n", rxBuffer);

    if (!checkNoError(rxBuffer)) { // TODO: if we are sure that no valid answer can be 2 bytes, this can be
        // changed to also check for OK,
        return -1;
    }


    if (!checkStatusOK()) {
        return -1;
    }

    response = rxBuffer; // char array to string //todo: trailing zeros? @dan: the private buffer's data is not deleted
    // seems like too much work: is it possilbe to only send the data until
    // the first \n or \0 (if inserted in Uart) (I tried and failed)
    // or do we have to reset it every time?

    return 0;

}


bool UBlox::sendCmd(const char *const cmdBuffer)
{
    size_t nBytes;
    nBytes = uart.writeBuffer(cmdBuffer);
    if (nBytes == -1) {
        printf("UART write error\n");
        return false;
    }

    nBytes = uart.readNext(rxBuffer, MAX_BUFFER_LENGTH, ECHO_TIMEOUT);

    if (nBytes <= 0) {
        printf("UART read error or timeout\n");
        return false;
    }

    printf("echo read: %s\n", rxBuffer);
    if (!findCharArray(cmdBuffer, rxBuffer)) {
        printf("invalid echo\n");
        return false;
    }
    return true;
}

bool UBlox::checkStatusOK()
{
    // needs separate buffer to not overwrite the response.
    char checkBuffer[MAX_BUFFER_LENGTH];
    ssize_t nRx = uart.readNext(checkBuffer, MAX_BUFFER_LENGTH, RX_TIMEOUT);

    if (nRx <= 0) {
        printf("UART read error or timeout\n");
        return -1;
    } else {
        // if the result is OK, the device often sends \r\n first, if only two bytes are received, we
        // read again and check for OK
        if (nRx == 2) {
            nRx = uart.readNext(checkBuffer, MAX_BUFFER_LENGTH, RX_TIMEOUT);
        }
        if (findCharArray(AT_STATUS_OK, checkBuffer)) {
            printf("Status OK\n");
            //return 0;
        } else if (!checkNoError(checkBuffer)) {
            return -1;
        } else {
            printf("Status unknown: %s", checkBuffer);
            return -1;
        }
    }
}

bool UBlox::checkNoError(const char *const checkBuffer)
{
    if (findCharArray(AT_STATUS_ERROR, checkBuffer)) {
        printf("Status ERROR\n");
        // TODO: error status needs to be handled
        return false;
    } else if (findCharArray(AT_STATUS_ABORTED, checkBuffer)) {
        // TODO: aborted status needs to be handled
        printf("Status ABORTED\n");
        return false;
    }
    return true;
}

// todo helper
bool UBlox::findCharArray(const char *const needle, const char *const haystack)
{
    //printf("looking for %s in %s\n", needle , haystack);
    std::string hs(haystack);
    std::string nd(needle);
    return hs.find(nd) != std::string::npos;
}
