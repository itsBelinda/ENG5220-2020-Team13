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
