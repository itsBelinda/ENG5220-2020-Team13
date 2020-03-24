
#ifndef BEESAFEPI_UBLOX_H
#define BEESAFEPI_UBLOX_H

#include <string>
#include <vector>
#include "Uart.h"


class Ublox{
public:
    //Constructors and destructor
    Ublox();
    ~Ublox();

public:
    std::string config(std::string& params);

    std::string sendText(std::string &params);
    std::string getLocation(std::string &params);
    std::string sendLocation(std::string &params);
    std::string getIMEI(std::string &params);

private:
    Uart *uart;
    std::string response;

    int checkConnection();
};
#endif //BEESAFEPI_UBLOX_H



