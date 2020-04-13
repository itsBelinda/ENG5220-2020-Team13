#ifndef BEESAFEPI_COMMUNICATION_H
#define BEESAFEPI_COMMUNICATION_H

#include "UBlox.h"

#include "../contact/Contact.h"

// Include system libraries.
#include <string>
#include <utility>
#include <mutex>

/**
 * Comms class enables the underlying device objects to be invoked in a thread-safe
 * manner, concurrently.
 *
 * Unless specified otherwise, functions within the class will
 * block access to prevent errors / side-effects from propagating in lower levels;
 * this is difficult to manage given the interoperability of functions.
 */
class Comms
{

public:

    // Constructor and destructor.
    explicit Comms();
    ~Comms();

public:

    // Initialise the comms interface.
    bool init();

    // Get the uBlox device that's being communicated to.
    const UBlox &getUBlox();

    // Connection specific functions.
    bool hasGPRS(bool &attached);
    bool hasPSD(bool &connected);

    // Get information about the device.
    bool getModelNumber(std::string &modelNumber);
    bool getIMEI(std::string &imeiNumber);

    // Getting and setting the message mode.
    bool getSendMessageMode(char &sendMsgMode);
    bool setSendMessageMode(char sendMsgMode);

    // Getting and setting the location scan mode.
    bool getLocationScanMode(char &locScanMode);
    bool setLocationScanMode(char locScanMode);

    // Getting the CellLocate location from the device.
    bool getLocation(std::pair<double, double> &latLng);
    bool getLocation(double &lat, double &lng);


    bool sendMessage(Contact &contact, const std::string &message);
    bool sendMessage(const std::string &phoneNumber, const std::string &message);


private:

    // The u-Blox device that is being communicated with.
    UBlox uBlox;

    // The lock utilised to prevent concurrent access.
    std::mutex mtx;

};

#endif //BEESAFEPI_COMMUNICATION_H



