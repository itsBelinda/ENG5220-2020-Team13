
#include "comms/UBlox.h"
#include "device/AccountBuilder.h"
#include "device/Account.h"
#include "contact/Contact.h"
#include "geo/RoundFence.h"
#include "geo/PolyFence.h"
#include "monitor/Monitor.h"

#include <utility>
#include <iostream>

int main()
{

    // Attempt to instantiate the comms interface.
    Comms comms;
    if (!comms.init()) {
        printf("Failed to initialise the comms interface.");
        return -1;
    }

    // Check if GPRS is attached.
    bool gprsAttached = false;
    bool rc = comms.hasGPRS(gprsAttached);
    std::cout << "GPRS Attached, success: " << rc << ", attached: " << gprsAttached << std::endl;

    // Check that the internet connection has been established.
    bool psdConnected = false;
    rc = comms.hasPSD(psdConnected);
    std::cout << "PSD Connected, success: " << rc << ", connected: " << psdConnected << std::endl;

    // Check that comms is able to get the model number.
    std::string modelNumber;
    rc = comms.getModelNumber(modelNumber);
    std::cout << "Model Number, success: " << rc << ", number: " << modelNumber << std::endl;

    // Check that the comms is able to get the IMEI number.
    std::string imei;
    rc = comms.getIMEI(imei);
    std::cout << "IMEI, success: " << rc << ", imei: " << imei << std::endl;

    // Check the sending of messages.
    std::string phoneNumber = "+447455787051";
    //std::string phoneNumber = "+447939652867";
    std::string message = "THERE IS A BEAST IN YOUR AREA! GET OUT!";
    rc = comms.sendMessage(phoneNumber, message);
    std::cout << "Sending message, success: " << rc << std::endl;

    // Test the getting of the location.
    std::pair<double, double> latLng;
    rc = comms.getLocation(latLng);
    std::cout << "Location, success: " << rc << ", lat: " << latLng.first << ", lng: " << latLng.second << std::endl;

    return 0;
}