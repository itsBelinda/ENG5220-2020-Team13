/**
 * \file PassiveMonitorState.h
 * \class PassiveMonitorState
 *
 * \ingroup Monitor
 *
 * \brief The passive monitor state child class
 *
 * The child class to MonitorState. It handles the functionality of what to do when the device is inside a fence,
 * by dormantly monitoring the coordinates at which the device is at, so that in case of an event it can hand over
 * to the ActiveMonitorState to handle notifications and further monitoring.
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


#ifndef BEESAFEPI_PASSIVEMONITORSTATE_H
#define BEESAFEPI_PASSIVEMONITORSTATE_H

#include "MonitorState.h"

// The name of the state
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
