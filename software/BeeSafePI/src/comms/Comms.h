#ifndef BEESAFEPI_COMMUNICATION_H
#define BEESAFEPI_COMMUNICATION_H

#include "UBlox.h"

#include <string>
#include <utility>
#include <mutex>

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

    // Methods for querying the U-Blox device.
    bool getModelNumber(std::string &modelNumber);
    bool getIMEI(std::string &imei);

private:

    // The u-Blox device that is being communicated with.
    UBlox uBlox;

    // The lock utilised to prevent concurrent access.
    std::mutex mtx;

};

#endif //BEESAFEPI_COMMUNICATION_H



