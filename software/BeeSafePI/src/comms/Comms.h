#ifndef BEESAFEPI_COMMUNICATION_H
#define BEESAFEPI_COMMUNICATION_H

#include <string>
#include "Ublox.h"
#include <vector>
#include "../contact/Contact.h"


class Comms
{
public:
    //Constructors and destructor
    Comms(Contact * contact);
    
    Comms();
    ~Comms();

public:
    std::pair<double, double> fetchLocation();
    int sendText(std::string message);
    Contact* getContact();
    void setContact(Contact &contact);
    
private:
    Ublox *ublox;

    Contact * contact;
    std::string params;

};

#endif //BEESAFEPI_COMMUNICATION_H


