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

#define ACCOUNT_PATH "account.json"

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

/**
 * Responsible for attempting to initialise the BeeSafeManager i.e. all
 * necessary components required in order to successfully run the device.
 *
 * Due to connectivity, the device may not initialise, first. Thus, this is done
 * up to three times.
 *
 * Note, successful initialisation may yield an account ptr == nullptr.
 * This occurs in the event the device does not possess a saved instance of
 * fences, contacts and timings.
 *
 * @return True if no errors were met, false otherwise.
 */
bool BeeSafeManager::init()
{
    // Prevents potential memory leaks.
    if (monitor != nullptr) {
        monitor->stop();
        delete monitor;
    }
    delete comms;
    delete account;

    // Attempt to initialise comms.
    comms = initComms();
    if (comms == nullptr) {
        std::cerr << "Failed to initialise comms." << std::endl;
        return false;
    }

    // Create the monitor instance without the account.
    monitor = new Monitor(comms);

    // Attempt to load the account file.
    account = initAccount();

    // We have successfully initialised the manager.
    return true;
}

/**
 * Function attempts to initialise the comms.
 *
 * @return True if the comms was successfully initialised, false
 *      otherwise.
 */
Comms* BeeSafeManager::initComms()
{
    // Create a new instance of comms.
    auto comms = new Comms();

    // Attempt to establish the comms interface.
    bool commsInit = false;
    short tries = 0;
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
        return nullptr;
    }

    return comms;
}

Account* BeeSafeManager::initAccount()
{
    // TODO:
    return nullptr;
}

bool BeeSafeManager::start()
{

    // If the account is not null, start the monitor thread.
    if (account != nullptr) {
        if (!monitor->start(account)) {
            std::cerr << "Failed to start the monitor thread." << std::endl;
            return false;
        }
    }

    // TODO: Get account from web API and try again.

    return true;
}

int main()
{
    // Create an instance of the manager.
    auto beeSafeManager = new BeeSafeManager();
    if (!beeSafeManager->init()) {
        std::cerr << "Failed to initialise manager." << std::endl;
        return EXIT_FAILURE;
    }

    // Start the manager loop.
    return beeSafeManager->start()
           ? EXIT_SUCCESS
           : EXIT_FAILURE;
}