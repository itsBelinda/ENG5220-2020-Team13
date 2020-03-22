
#ifndef BEESAFEPI_UBLOX_H
#define BEESAFEPI_UBLOX_H

#include <string>
#include <vector>
#include "Uart.h"

#define BAUD_RATE 115200

class Ublox{
public:
    //Constructors and destructor
    //Ublox();

    Ublox();
    ~Ublox();

public:
    std::string config(std::string& params);
    std::string sendCommand(std::string& type, std::string& params);

private:
    Uart *uart;
    std::string response;

    std::string sendText(std::string &params);
    std::string fetchLocation(std::string &params);
    std::string getIMEI(std::string &params);
};
#endif //BEESAFEPI_UBLOX_H


