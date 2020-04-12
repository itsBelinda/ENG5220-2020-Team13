#ifndef BEESAFEPI_COMMUNICATION_H
#define BEESAFEPI_COMMUNICATION_H

#include "UBlox.h"

#include "../contact/Contact.h"

// Include system libraries.
#include <string>
#include <utility>
#include <mutex>

/**
 * Comms class enabled the underlying device objects to be invoked in a thread-safe
 * manner, concurrently.
 *
 * Unless specified otherwise, functions within the class will
 * block access to prevent errors / side-effects from propegating in lower levels;
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

    // Methods for incoming / getting data from the device.
    bool getModelNumber(std::string &modelNumber);
    bool getIMEI(std::string &imeiNumber);
    bool getLocation(std::pair<double, double> &latLng);
    bool getLocation(double &lat, double &lng);

    // Methods for outgoing / sending data to the device (and beyond).
    bool sendMessage(Contact &contact, const std::string &message);
    bool sendMessage(const std::string &phoneNumber, const std::string &message);

private:

    // The u-Blox device that is being communicated with.
    UBlox uBlox;

    // The lock utilised to prevent concurrent access.
    std::mutex mtx;

};

#endif //BEESAFEPI_COMMUNICATION_H



