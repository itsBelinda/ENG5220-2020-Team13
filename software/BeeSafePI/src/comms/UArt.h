/**
 * \file UArt.h
 * \class UArt
 *
 * \ingroup Comms
 *
 * \brief The lowest level communication handler between the Pi and the PCB
 *
 * The UArt class handles the low-level communication between the Raspberry Pi and the PCB (specifically the Ublox GSM module on it),
 * implementing the Universal Asynchronous Receiver/Transmitter protocol. This class opens and closes the tty devices on the Pi so
 * that it can communicate with the PCB via GPIO pins using the UART protocol. The class contains methods and parameters that
 * pertain to byte-level communication via the protocol, including the specific configuration options necessary.
 *
 * \author BeeSafe Team, Team 13
 *
 * \version v1.0
 *
 * \date 2020/04/20
 *
 * Contact: beesafe.uofg@gmail.com
 *
 * Licence: MIT
 */

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

    // The tty device in use.
    int device;

};

#endif //BEESAFEPI_UART_H



