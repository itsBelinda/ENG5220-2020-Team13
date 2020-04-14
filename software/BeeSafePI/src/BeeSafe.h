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

    //
    bool init();
    bool start();

private:

    // The device comms and monitor thread.
    Comms *comms;
    Monitor *monitor;

    // The account being monitored.
    Account *account;
};

#endif //BEESAFEPI_MAIN_H
