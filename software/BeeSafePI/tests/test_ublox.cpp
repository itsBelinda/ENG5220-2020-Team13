#include <iostream>
using namespace std;
#include "../src/comms/UBlox.cpp"
#include "../src/comms/UArt.cpp"


int main ()
{
    std::string failures = "";
    
    UBlox ublox;
    
    //-----testing init-----
    //init sometimes needs to be done twice, as on the first attempt it often
    //doesn't pass the PSD checks
    ublox.init();
    bool success = ublox.init();
    if(success != true){failures+="init;";}

    bool tempStatus = false;

    //-----testing hasregistered-----
    ublox.hasRegistered(tempStatus);
    if(tempStatus != true){failures+="hasRegistered;";}

    tempStatus = false;
    //-----testing hasGPRS-----
    ublox.hasGPRS(tempStatus);
    if(tempStatus != true){failures+="hasGPRS;";}

    tempStatus = false;
    //-----testing hasPSD-----
    ublox.hasPSD(tempStatus);
    if(tempStatus != true){failures+="hasPSD;";}

    tempStatus = false;
    //-----testing hasregistered-----
//    ublox.startAutoRegistration(tempStatus);
//    if(tempStatus != true){failures+="startAutoReg;";}

    //-----testing connectPSD-----
    

//    std::string tempResponse = "";
    //-----testing getSendMsgMode-----
    


    //-----testing setSendMsgMode-----



    //-----testing getLocScanMode-----



    //-----testing setLocScanMode-----


//    tempResponse = "";
    //-----testing getModelNumber-----
//    ublox.getModelNumber(tempResponse);
//    if(tempResponse != "SARA-G350-02X-01"){failures+="modelNo;";}

//    tempResponse = "";
    //-----testing getIMEI-----
//    ublox.getIMEI(tempResponse);
//    if(tempResponse != "355989070341461"){failures+="imei;";}

//    tempResponse = "";
    //-----testing getLocation-----



    //-----testing sendMessage-----


    //-----testing sendLocation-----
    //TODO when method implemented

    if(failures != ""){
        cout << "Test failed" << endl;
        cout << failures << endl;
    }
    else{cout << "Test passed";}
}
