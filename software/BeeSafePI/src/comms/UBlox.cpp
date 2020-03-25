#include "UBlox.h"

// Define the AT commands that are used on the U-Blox device.
#define AT_COMMAND_GET_MODEL_NUMBER "ATI0\r\n"
#define AT_COMMAND_GET_IMEI "AT+GSN1"
#define AT_COMMAND_GET_LOCATION ""

// Define the responses.
#define AT_RESPONSE_OK "OK"
#define AT_RESPONSE_ERROR "ERROR"

UBlox::UBlox()
{
    conf();
}

UBlox::~UBlox() = default;

int UBlox::conf()
{
    return -1;
}

Uart& UBlox::getUart()
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
    char modelNumberBuffer[31] = {'\0'};


    rc = uart.readBuffer(modelNumberBuffer, 31, 1000);

    printf("Command successfully echoed\n");

    printf("Read, %d Echo: %s\n", (int) rc, modelNumberBuffer);

    // Figure out the format.
    for (int i = 0; i < 31; ++i) {
        if (modelNumberBuffer[i] == '\n') printf("/n");
        else if (modelNumberBuffer[i] == '\r') printf("/r");
        else printf("%c", modelNumberBuffer[i]);
    }
    printf("\n");

    return rc;
}

int UBlox::getIMEI(std::string &imei)
{
    return -1;
}

int UBlox::getLocation(double &lat, double &lng)
{
    return -1;
}
