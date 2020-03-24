#ifndef BEESAFEPI_COMMUNICATION_H
#define BEESAFEPI_COMMUNICATION_H

#include <string>
#include "Ublox.h"
#include <vector>


class Comms
{
public:
    //Constructors and destructor

    Comms();
    ~Comms();

public:
    std::pair<double, double> getLocation();
    int sendText(std::string number, std::string message);    bool isAlive();
    std::string getIMEI();
    int sendLocation();

private:
    Ublox *ublox;
    std::string params;



};

#endif //BEESAFEPI_COMMUNICATION_H



