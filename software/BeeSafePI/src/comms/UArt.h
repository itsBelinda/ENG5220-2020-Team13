#ifndef BEESAFEPI_UART_H
#define BEESAFEPI_UART_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <termios.h>

// Define device properties.
#define DEVICE_PATH "/dev/ttyS0"
#define DEVICE_BAUD_RATE 115200

class UArt
{

public:

    // Constructors & Destructors
    UArt();
    ~UArt();

public:

    // Invoked to (re-)configure the UART interface.
    bool configure();

    // Generic getters and setters for the device.
    bool hasDevice();
    int getDevice();

    // For reading commands from the device via uArt.
    ssize_t readExpected(char *buffer, size_t bytesExpected, int timeoutMs);
    ssize_t readNext(char *resultBuffer, size_t resultBufferLen, int timeoutMs);

    // For writing commands via uArt to the device.
    ssize_t writeNext(const std::string &cmd);
    ssize_t writeNext(const char *cmdBuffer);

private:

    // Device attributes.
    int device;

};

#endif //BEESAFEPI_UART_H



