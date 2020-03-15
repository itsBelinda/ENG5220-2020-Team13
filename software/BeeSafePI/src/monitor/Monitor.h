#ifndef BEESAFEPI_MONITOR_H
#define BEESAFEPI_MONITOR_H

#include "../comms/Communication.h"
#include "../device/Account.h"

#include "states/MonitorState.h"
#include "states/ActiveMonitorState.h"
#include "states/PassiveMonitorState.h"

#include <thread>

class Monitor
{

public:

    // Constructors, copy constructors, and destructors.
    Monitor(Communication* communication, Account* account);
    explicit Monitor(Communication* communication);

public:

    // Starting and stopping the thread.
    bool start();
    bool start(Account* account);
    void stop();

private:

    // Monitor thread main loop.
    void run();

private:

    // Attributes for communication and the account.
    Communication* communication;
    Account* account;

    // Attributes for threading.
    bool monitorThreadRunning;
    std::thread* monitorThread;

    // The monitor thread state and the location of the child.
    MonitorState *monitorState;

    // Static function executes the monitor run thread.
    static void execute(Monitor* const monitor)
    {
        monitor->run();
    }
};


#endif //BEESAFEPI_MONITOR_H
