#ifndef BEESAFEPI_COMMUNICATION_H
#define BEESAFEPI_COMMUNICATION_H

#include "UBlox.h"

#include <string>
#include <utility>

class Comms
{

public:

    // Constructor and destructor.
    explicit Comms();
    ~Comms();

public:

    // Invoked to configure / re-configure the comms.
    bool configure();

    // Get the uBlox device that's being communicated to.
    const UBlox &getUBlox();

    // Incoming / getters from the device.
    bool getModelNumber(std::string &modelNumber);
    bool getIMEI(std::string &imei);
    bool getLocation(std::pair<double, double> &latLng);
    bool getLocation(double &lat, double &lng);

    // Outgoing / setters to the device.
    bool sendMessage(const std::string &phoneNumber, const std::string &message);
    bool sendLocation(const std::string &phoneNumber, const std::pair<double, double> &latLng);
    bool sendLocation(const std::string &phoneNumber, double lat, double lng);

private:

    // The u-blox that's used as a means of communication.
    UBlox uBlox;

};

#endif //BEESAFEPI_COMMUNICATION_H



