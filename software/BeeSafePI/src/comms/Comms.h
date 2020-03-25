#ifndef BEESAFEPI_COMMUNICATION_H
#define BEESAFEPI_COMMUNICATION_H

#include "UBlox.h"

#include <string>
#include <vector>

class Comms
{

public:

    // Constructors and destructors.
    Comms();
    ~Comms();

public:

    // TODO: Define the behaviour for the comms interface.

private:

    // Comms attributes.
    UBlox uBlox;

};

class CommsX
{

    /*
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
    std::string params;

*/

};

#endif //BEESAFEPI_COMMUNICATION_H



