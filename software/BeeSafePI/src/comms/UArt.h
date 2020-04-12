#ifndef BEESAFEPI_UART_H
#define BEESAFEPI_UART_H

// Include system libraries.
#include <string>

class UArt
{

public:

    // Constructors and destructors
    UArt();
    ~UArt();

public:

    // Initialise the serial interface.
    bool init();

    // Checking and getting the device.
    bool hasDevice();
    int getDevice();

    // Reading bytes (responses) from the linux buffer / device.
    ssize_t readExpected(char *buffer, size_t bytesExpected, int timeoutMs);
    ssize_t readNext(char *resultBuffer, size_t resultBufferLen, int timeoutMs);

    // Writing bytes (commands) to the linux buffer / device.
    ssize_t writeNext(const std::string &command);
    ssize_t writeNext(const char *command);

private:

    // The actual device.
    int device;

};

#endif //BEESAFEPI_UART_H



