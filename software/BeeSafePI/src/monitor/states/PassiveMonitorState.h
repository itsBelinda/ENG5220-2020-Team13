#ifndef BEESAFEPI_PASSIVEMONITORSTATE_H
#define BEESAFEPI_PASSIVEMONITORSTATE_H

#include "MonitorState.h"

#define PASSIVE_STATE_NAME "Passive"

class PassiveMonitorState : public MonitorState
{

public:

    // Constructors and destructors.
    PassiveMonitorState(Comms* comms, Account* account);
    ~PassiveMonitorState() override;

public:

    // Override the interface responsible for handling the latitude and longitude.
    MonitorState* handleLatLng(std::pair<double, double> &latLng) override;

};

#endif //BEESAFEPI_PASSIVEMONITORSTATE_H
