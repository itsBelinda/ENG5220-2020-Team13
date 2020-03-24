#include <chrono>
#include "Ublox.h"

//--------Constructors and destructor--------

//Constructor for
//Ublox::Ublox() {}

//Empty constructor
Ublox::Ublox() {
}

//Destructor
Ublox::~Ublox() {}

//--------Getters--------


//--------Setters--------


//--------Other methods--------

std::string Ublox::config(std::string& parameters){


    return response;
}

std::string Ublox::sendText(std::string& params){
    //do all the calls necessary to set up the text functionality

    return response;
}

std::string Ublox::getLocation(std::string& params){
    //do all the calls necessary to set up the fetch locations functionality
    //deal with the time comparison first
    auto currentTime = std::chrono::system_clock::now();


    return response;
}

std::string Ublox::sendLocation(std::string& params){
    //do all the calls necessary to set up the send location to server functionality

    return response;
}

std::string Ublox::getIMEI(std::string& params){
    //do all the calls necessary to fetch the IMEI

    return response;
}

int Ublox::checkConnection(){
    uart->writeBuffer((std::string &) "OK");
    //std::string response = uart->getReadBuffer();
    if(response == "any errors really"){
        return 1;
    }
    return 0;
}
