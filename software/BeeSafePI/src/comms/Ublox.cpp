#include "Ublox.h"

//--------Constructors and destructor--------

//Constructor for
//Ublox::Ublox() {}

//Empty constructor
Ublox::Ublox() {
    uart = new Uart();
}

//Destructor
Ublox::~Ublox() {}

//--------Getters--------


//--------Setters--------


//--------Other methods--------

std::string Ublox::config(std::string& parameters){


    return response;
}

std::string Ublox::sendCommand(std::string& type, std::string& params){
    if (type == "send_text"){
        //modify parameter string to add any additional parameters

        response = Ublox::sendText(params);
    } else if (type == "get_location") {
        //modify parameter string to add any additional parameters

        response = Ublox::fetchLocation(params);
    }

    return response;
}

std::string Ublox::sendText(std::string& params){
    //do all the calls necessary to set up the text functionality

    return response;
}

std::string Ublox::fetchLocation(std::string& params){
    //do all the calls necessary to set up the fetch locations functionality

    return response;
}

std::string Ublox::getIMEI(std::string& params){
    //do all the calls necessary to fetch the IMEI

    return response;
}
