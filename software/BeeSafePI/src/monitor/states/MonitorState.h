/**
 * \file MonitorState.h
 * \class MonitorState
 *
 * \ingroup Monitor
 *
 * \brief The parent monitor state class
 *
 * The parent class to ActiveMonitorState and PassiveMonitorState. It provides generic functionality
 * by defining how to handle when the device crosses a fence boundary.
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

#ifndef BEESAFEPI_MONITORSTATE_H
#define BEESAFEPI_MONITORSTATE_H

#include "../../comms/Comms.h"
#include "../../device/Account.h"

#include <utility>


class MonitorState {

public:

    // Constructors and destructors.
    MonitorState(const char* stateName, Comms *comms, Account *account);
    virtual ~MonitorState();

public:

    // Get the name of the state.
    const char* const getStateName();

    // Handle the new location of the device.
    virtual MonitorState *handleLatLng(std::pair<double, double> &latLng) = 0;

protected:

    // The name of the state.
    const char *stateName;

    // Communications and account attributes.
    Comms *comms;
    Account *account;

    // Override the interface responsible for handling the latitude and longitude.
    Fence* getCrossedFence(std::pair<double, double> &latLng);

};


#endif //BEESAFEPI_MONITORSTATE_H
