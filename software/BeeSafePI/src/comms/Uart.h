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

class Uart
{

public:

    // Constructors & Destructors
    Uart();
    ~Uart();

public:

    // Generic getters and setters.
    bool isOpen();
    int getDevice();

    // For reading and writing from and to the device.
    ssize_t readBuffer(char *buffer, size_t bytesExpected, int timeoutMs);
    ssize_t readNext(char * resultBuffer, size_t resultBufferLen, int timeoutMs);

    ssize_t writeBuffer(const std::string &cmd);
    ssize_t writeBuffer(const char *cmdBuffer);

private:
    int conf();

private:

    // Device attributes.
    int device;

};

#endif //BEESAFEPI_UART_H



