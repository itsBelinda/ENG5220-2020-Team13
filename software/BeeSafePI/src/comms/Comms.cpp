#include "Comms.h"
//--------Constructors and destructor--------
//Constructor with contact specified
Comms::Comms(Contact * contact) {
    this->contact = contact;
    ublox = new Ublox();
}

//Empty constructor
Comms::Comms() {}

//Destructor
Comms::~Comms() {}

//--------Getters--------
Contact* Comms::getContact(){
    return contact;
}

//--------Setters--------
void Comms::setContact(Contact &contact){}

//--------Other methods--------
std::pair<double, double> Comms::fetchLocation(){
    std::pair<double, double> loc;
    std::string response = ublox->sendCommand((std::string &)"get_location",params);
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

int Comms::sendText(std::string message){
    std::string number = contact->getNumber();
    params = params+number+message;
    std::string success = ublox->sendCommand((std::string &)"send_text",params);

    //error checking: filter success for any error messages

    return 0;
 }

bool isAlive(){
    //check if connection still intact

    return true;
}
