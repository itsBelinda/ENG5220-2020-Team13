#ifndef BEESAFEPI_MONITOR_H
#define BEESAFEPI_MONITOR_H

#include "states/MonitorState.h"
#include "states/ActiveMonitorState.h"
#include "states/PassiveMonitorState.h"

#include <thread>

class Monitor
{

public:

    // Constructors, copy constructors, and destructors.
    Monitor();

public:

    // Starting and stopping the thread.
    bool start();
    void stop();

private:

    // Behaviour.
    void run();

private:

    // Attributes for threading.
    bool monitorThreadRunning;
    std::thread* monitorThread;

    // Attributes for the location.


    // Static function executes the monitor run thread.
    static void execute(Monitor* const monitor)
    {
        monitor->run();
    }
};


#endif //BEESAFEPI_MONITOR_H
