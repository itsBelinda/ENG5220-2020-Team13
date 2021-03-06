/**
 * \file PassiveMonitorState.cpp
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


#include "PassiveMonitorState.h"
#include "ActiveMonitorState.h"

/**
 * Constructor explicitly initialises the PassiveMonitorState class
 * instance.
 *
 * @param comms The communications interface for the underling hardware.
 * @param account The account against which the coordinates are compared.
 */
PassiveMonitorState::PassiveMonitorState(Comms *const comms, Account *const account)
        : MonitorState(PASSIVE_STATE_NAME, comms, account) {}

/**
 * Destructor is used to clean up any resources occupied by the PassiveMonitorState
 * instance.
 */
PassiveMonitorState::~PassiveMonitorState() = default;


/**
 * Handles passive location monitoring; passively handled monitoring i.e. no notifications /
 * storage.
 *
 * When in the positive monitoring state, the current location is checked against the fences
 * that are stored in account. If the device is out of a fence, the state is switched to the
 * active monitoring state.
 *
 * @param latLng The pair containing the latitude (first) and longitude (second) coordinates
 *      of the device.
 * @return Nullptr if state remains in passive, a pointer to ActiveMonitorState if the
 *      device should be monitored.
 */
MonitorState *PassiveMonitorState::handleLatLng(std::pair<double, double> &latLng)
{
    // If the device is within the account fences, return (keep passive state).
    if (getCrossedFence(latLng) == nullptr) {
        return nullptr;
    }

    // A fence has been breached. Begin monitoring in an active state.
    return new ActiveMonitorState(comms, account);
}
