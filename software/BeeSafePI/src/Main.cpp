
#include "comms/UBlox.h"
#include "device/AccountBuilder.h"
#include "device/Account.h"
#include "contact/Contact.h"
#include "geo/RoundFence.h"
#include "geo/PolyFence.h"
#include "monitor/Monitor.h"

#include <iostream>

int main()
{

    // Attempt to instantiate the comms interface.
    Comms comms;
    if (!comms.init()) {
        fprintf(std::stderr, "Failed to initialise the comms interface.");
        return -1;
    }

    // Check if GPRS is attached.
    bool gprsAttached = false;
    bool rc = comms.hasGPRS(gprsAttached);
    std::cout << "GPRS Attached, success: " << rc << ", attached: " << gprsAttached << std::endl;

    // Check that the internet connection has been established.
    bool psdConnected = false;
    rc = comms.hasPSD(psdConnected);
    std::cout << "PSD Connected, success: " << rc << ". connected: " << psdConnected << std::endl;

    return 0;
}