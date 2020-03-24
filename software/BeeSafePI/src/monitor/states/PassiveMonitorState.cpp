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
        : MonitorState(comms, account)
{

    // TODO: Passive monitor state code.

}

/**
 * Destructor is used to clean up any resources occupied by the PassiveMonitorState
 * instance.
 */
PassiveMonitorState::~PassiveMonitorState() = default;

/**
 * Handles passive location monitoring.
 *
 * When in the passive monitoring state, the current location is checked against
 * the fences that are sored in account. If the current location is not a valid
 * one for any of the fences, the state is switched to the active monitoring state.
 *
 * @param latLng The pair of latitude and longitude coordinates that are to be
 *      examined.
 * @return A pointer to the new state if this state is incapable of handling the
 *      locations, nullptr otherwise.
 */
MonitorState *PassiveMonitorState::handleLatLng(std::pair<double, double> &latLng)
{
    /// Check all fences if they are valid (or should only one be valid?)
    if (!isInFence(latLng)) {
        // TODO: how to properly access comms / account? (currently: made
        //  them protected)
        ActiveMonitorState *activeMonitorState = new ActiveMonitorState(comms, account);
        return activeMonitorState;
    }
    return nullptr;
}
