#ifndef BEESAFEPI_ACTIVEMONITORSTATE_H
#define BEESAFEPI_ACTIVEMONITORSTATE_H

// System inclusions.
#include <chrono>

#include "MonitorState.h"

//! Delay counter how many times the location has to be updated outside of the
//! fence before a notification is sent.
#define DELAY_NOTIFICATION_CNT  10
//! Defines how often the location update should be sent in a time interval defined in milliseconds (ms).
#define UPDATE_LOC_MS  1000

class ActiveMonitorState : public MonitorState
{

public:

    // Constructors and destructors.
    ActiveMonitorState(Comms* comms, Account* account);
    ~ActiveMonitorState() override;

public:

    // Override the interface responsible for handling the latitude and longitude.
    MonitorState* handleLatLng(std::pair<double, double> &latLng) override;
private:
    bool notified;
    short delayNotification;
    double lastLocTime;
    void notifyExitedFence();
    void notifyBackInFence();
    void sendLocation();
    static double getSysTimeMS();
};


#endif //BEESAFEPI_ACTIVEMONITORSTATE_H
