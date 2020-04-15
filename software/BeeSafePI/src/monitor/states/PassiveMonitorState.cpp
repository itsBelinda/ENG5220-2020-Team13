#include "PassiveMonitorState.h"
#include "ActiveMonitorState.h"

#define PASSIVE_STATE_NAME "Passive"

/**
 * Constructor explicitly initialises the PassiveMonitorState class
 * instance.
 *
 * @param comms The communications interface for the underling hardware.
 * @param account The account against which the coordinates are compared.
 */
PassiveMonitorState::PassiveMonitorState(Comms *const comms, Account *const account)
        : MonitorState(comms, account) {}

/**
 * Destructor is used to clean up any resources occupied by the PassiveMonitorState
 * instance.
 */
PassiveMonitorState::~PassiveMonitorState() = default;

/**
 * Getter for the passive state name.
 *
 * @return A c-string pointer to the state name.
 */
const char* PassiveMonitorState::getStateName()
{
    return PASSIVE_STATE_NAME;
}

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
    if (isInFence(latLng)) {
        return nullptr;
    }

    // A fence has been breached. Begin monitoring in an active state.
    return new ActiveMonitorState(comms, account);
}
