#include "BeeSafe.h"

#include "comms/UBlox.h"
#include "device/AccountBuilder.h"
#include "device/Account.h"
#include "contact/Contact.h"
#include "geo/RoundFence.h"
#include "geo/PolyFence.h"
#include "monitor/Monitor.h"

#include <utility>
#include <iostream>

#define INIT_COMMS_TRIES 3

BeeSafeManager::BeeSafeManager()
{
    comms = nullptr;
    account = nullptr;
    monitor = nullptr;
}

BeeSafeManager::~BeeSafeManager()
{
    // Delete monitor first to illegal memory access.
    if (monitor != nullptr) {
        monitor->stop();
        delete monitor;
    }

    // Finally, delete the comms and account.
    delete comms;
    delete account;
}

bool BeeSafeManager::init()
{
    // Prevents potential memory leaks.
    if (monitor != nullptr) {
        monitor->stop();
        delete monitor;
    }
    delete comms;
    delete account;

    comms = new Comms();

    // Attempt to establish the comms interface.
    bool commsInit = false;
    char tries = 0;
    do {
        tries++;
        std::cout << "Attempting to initialise comms... " << tries << "/" << INIT_COMMS_TRIES << "." << std::endl;
        commsInit = comms->init();
        if (!commsInit) {
            std::cerr << "Attempt " << tries << "/" << INIT_COMMS_TRIES << " to initialise comms failed." << std::endl;
        } else {
            std::cout << "Attempt " << tries << "/" << INIT_COMMS_TRIES << " successfully initialised comms." << std::endl;
            break;
        }
    } while (tries < INIT_COMMS_TRIES);

    // If the comms was not initialised, return.
    if (!commsInit) {
        std::cerr << "Failed to initialise comms." << std::endl;
        return false;
    }

    // Create the monitor instance without the account.
    monitor = new Monitor(comms);

    // TODO: Check whether a user account is present.

    return true;
}

bool BeeSafeManager::start()
{

    printf("HERE\n");

    // If the account is not null, start the monitor thread.
    if (account != nullptr) {
        if (!monitor->start(account)) {
            std::cerr << "Failed to start the monitor thread." << std::endl;
            return false;
        }
    }

    printf("HERE 2\n");

    // TODO: Get account from web API.

    return true;
}

int main()
{
    // Create an instance of the manager.
    BeeSafeManager beeSafeManager;
    if (!beeSafeManager.init()) {
        std::cerr << "Failed to initialise manager." << std::endl;
        return EXIT_FAILURE;
    }

    // Start the manager loop.
    return beeSafeManager.start()
           ? EXIT_SUCCESS
           : EXIT_FAILURE;
}