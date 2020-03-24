#include "Comms.h"
/*
//--------Constructors and destructor--------

//Empty constructor
Comms::Comms() {

}

//Destructor
Comms::~Comms() {}

//--------Getters--------

//--------Setters--------

//--------Other methods--------
std::pair<double, double> Comms::getLocation(){
    isAlive();

    std::pair<double, double> loc;
    std::string response = ublox->getLocation(params);
    std::vector<std::string> responses;

    //assuming response is space-separated - to update
    for (auto i:response) {
        if (i==' ') {
            responses.push_back(response);
            response = "";
        } else {
            response = response + i;
        }
    }
    responses.push_back(response);

    //filter out lat and lon and add to pair to return

    return loc;
}

int Comms::sendText(std::string number, std::string message){
    isAlive();
    params = params+number+message;


    //error checking: filter success for any error messages

    return 0;
}

int Comms::sendLocation(){
    isAlive();
    getLocation();


    return 0;
}

std::string Comms::getIMEI(){
    isAlive();

    return "";
}

bool Comms::isAlive(){
    //check if connection still intact

    return true;
}

*/