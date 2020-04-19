#include <iostream>
using namespace std;
#include "../src/comms/UArt.cpp"

int main ()
{
    std::string failures = "";
    
    UArt uart;

    //-----testing init()-----
    bool initSuccess = uart.init();
    //---testing if the init takes care of repeated device initialisations
    // and ensures that the system doesn't just run out of file 
    // descriptors (called device in the UArt class), crashing the system--- 
    try{ uart.init();}
    catch(...){failures+="initFD;";}

    //---testing if init succeeded---
    if(initSuccess==false){failures+="initSuccess;";}

    //---testing if init succeeded---
    int dev = uart.getDevice();
    if(dev==-1){failures+="initSuccess2;";}
    
    //---testing if init succeeded---
    bool has = uart.hasDevice();
    if(has==false){failures+="initSuccess3;";}
    
    //-----test writenext(string)-----
    std::string command = "ATI\r";
    ssize_t wNextSize = uart.writeNext(command);
    if (wNextSize == -1 || wNextSize != 5){failures+="writeNextString;";}
    
    //-----test writenext(char arr)-----
    char commandCharArr[command.size()+1];
    strcpy(commandCharArr,command.c_str());
    ssize_t wNextArrSize = uart.writeNext(commandCharArr);
    if(wNextArrSize == -1 || wNextArrSize != 5){failures+="writeNextCharArr;";}
    
    //-----test readnext-----
    char buffer[544]={'\0'};
    std::memset(buffer,'\0',544);
    ssize_t responseSize = uart.readNext(buffer,544,1000);
    
    if(responseSize == -1){failures+="readNext;";}
    //should it check for correct response size past non-'-1'?
    if(failures != ""){
        cout << "Test failed" << endl;
        cout << failures <<endl;
    }
    else{ cout << "Test passed"; }
}
