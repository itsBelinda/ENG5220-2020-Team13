/**
 * \file Monitor.h
 * \class Monitor
 *
 * \defgroup Monitor Monitor
 * \brief The package handling the constant real-time monitoring of the device location, overall and in relation to fences.
 * \ingroup Monitor
 *
 * \brief The Monitor class, handling the threads of monitor state classes and connects the device and communication functionalities.
 *
 * The Monitor class maintains the threads for Active and Passive monitoring state, serving the account information that needs to be
 * contacted in case of emergency, and facilitates the real-time connection between the communication module(package) and the
 * administrative, device-side features.
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
