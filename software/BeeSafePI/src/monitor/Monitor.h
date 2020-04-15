#ifndef BEESAFEPI_MONITOR_H
#define BEESAFEPI_MONITOR_H

#include "../comms/Comms.h"
#include "../device/Account.h"

#include "states/MonitorState.h"
#include "states/ActiveMonitorState.h"
#include "states/PassiveMonitorState.h"

#include <thread>
#include <utility>

class Monitor
{

public:

    // Constructors and destructors.
    Monitor(Comms* comms, Account* account);
    explicit Monitor(Comms* comms);

public:

    // Starting the thead.
    bool start();
    bool start(Account* account);

    // Stopping and joining the thread.
    void stop();
    void join();

private:

    // The monitor thread loop.
    void run();

private:

    // Communications and account pointers.
    Comms* comms;
    Account* account;

    // The thread and its status.
    bool monitorThreadRunning;
    std::thread* monitorThread;

    // The state of the monitor thread.

    MonitorState *monitorState;

    // Runs the monitor thread.
    static void execute(Monitor* const monitor)
    {
        monitor->run();
    }
};


#endif //BEESAFEPI_MONITOR_H
