#ifndef BEESAFEPI_PASSIVEMONITORSTATE_H
#define BEESAFEPI_PASSIVEMONITORSTATE_H


#include "MonitorState.h"

class PassiveMonitorState : public MonitorState
{

public:

    // Constructors and destructors.
    PassiveMonitorState(Comms* comms, Account* account);

public:

    // Override the interface responsible for handling the latitude and longitude.
    MonitorState* handleLatLng(std::pair<double, double> &latLng) override;

};


#endif //BEESAFEPI_PASSIVEMONITORSTATE_H
