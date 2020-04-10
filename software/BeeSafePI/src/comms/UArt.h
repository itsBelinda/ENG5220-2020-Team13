#ifndef BEESAFEPI_UART_H
#define BEESAFEPI_UART_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <time.h>
#include <termios.h>

// Defines various device properties.
#define DEVICE_PATH "/dev/ttyS0"
#define DEVICE_BAUD_RATE 115200
#define DEVICE_MAX_BUFFER_SIZE 1024

class UArt
{

public:

    // Constructors & Destructors
    UArt();
    ~UArt();

public:

    // Generic getters and setters.
    bool isDeviceOpen();
    int getDevice();

    // For reading commands from the device via uart.
    ssize_t readExpected(char *buffer, size_t bytesExpected, int timeoutMs);
    ssize_t readNext(char *resultBuffer, size_t resultBufferLen, int timeoutMs);

    // For writing commands via uart to the device.
    ssize_t writeNext(const std::string &cmd);
    ssize_t writeNext(const char *cmdBuffer);

private:
    int conf();

private:

    // Device attributes.
    int device;

};

#endif //BEESAFEPI_UART_H



