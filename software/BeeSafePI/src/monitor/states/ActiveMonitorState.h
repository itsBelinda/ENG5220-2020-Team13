#ifndef BEESAFEPI_ACTIVEMONITORSTATE_H
#define BEESAFEPI_ACTIVEMONITORSTATE_H

#include "MonitorState.h"

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
    void notifyExitedFence();
    void notifyBackInFence();
    void sendLocation();
};


#endif //BEESAFEPI_ACTIVEMONITORSTATE_H
