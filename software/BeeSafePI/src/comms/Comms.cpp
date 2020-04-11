#include "Comms.h"

Comms::Comms()
{
}

Comms::~Comms() = default;

bool Comms::configure()
{
    return true;
}

const UBlox& Comms::getUBlox()
{
    return uBlox;
}

bool Comms::getModelNumber(std::string &modelNumber)
{
    return uBlox.getModelNumber(modelNumber);
}

bool Comms::getIMEI(std::string &imei)
{
    return uBlox.getIMEI(imei);
}

bool Comms::getLocation(std::pair<double, double> &latLng)
{
    return getLocation(latLng.first, latLng.second);
}

bool Comms::getLocation(double &lat, double &lng)
{
    return uBlox.getLocation(lat, lng);
}

bool Comms::sendMessage(const std::string &phoneNumber, const std::string &message)
{
    return uBlox.sendMessage(phoneNumber, message);
}

