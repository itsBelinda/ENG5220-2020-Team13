/**
 * \file BeeSafe.h
 * \class BeeSafeManager
 *
 * \defgroup BeeSafePi BeeSafePi
 * \brief The package incorporating the entire BeeSafe Project
 * \ingroup BeeSafePi
 *
 * \brief The BeeSafe class initiates the complete program
 *
 * The BeeSafe class creates the instances of classes required to communicate with the hardware, the monitors overseeing
 * that, and the account information to whom the system must reach out in case of an emergency. Containing the main() function,
 * this is the root of the BeeSafe API.
 *
 * \author BeeSafe Team, Team 13
 *
 * \version v1.0
 *
 * \date 2020/04/20
 *
 * Contact: beesafe.uofg@gmail.com
 *
 * Licence: MIT
 */


#ifndef BEESAFEPI_MAIN_H
#define BEESAFEPI_MAIN_H

#include "comms/Comms.h"
#include "device/Account.h"
#include "monitor/Monitor.h"

class BeeSafeManager
{

public:

    // Constructor and destructor.
    BeeSafeManager();
    ~BeeSafeManager();

public:

    // Get the comminications interface and monitor thread.
    Comms* const getComms();
    Monitor* const getMonitor();

    // Get the account being managed / monitored.
    Account* const getAccount();

    // Initialise and start the manager.
    bool init();
    bool start();

private:

    // Initialise comms and monitor.
    Comms* initComms();
    Monitor *initMonitor(Comms *comms);

    // Initialise the account instance.
    Account* initAccount(const char* path);

private:

    // The device comms and monitor thread.
    Comms *comms;
    Monitor *monitor;

    // The account being monitored.
    Account *account;
};

#endif //BEESAFEPI_MAIN_H
