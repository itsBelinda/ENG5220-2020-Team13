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

    // Initialise and start the manager.
    bool init();
    bool start();

private:

    // Attempt to initialise the account.
    Comms* initComms();
    Account* initAccount();

private:

    // The device comms and monitor thread.
    Comms *comms;
    Monitor *monitor;

    // The account being monitored.
    Account *account;
};

#endif //BEESAFEPI_MAIN_H
