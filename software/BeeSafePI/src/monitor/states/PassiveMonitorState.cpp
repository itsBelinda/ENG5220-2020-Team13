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
        : MonitorState(comms, account) {

    // TODO: Passive monitor state code.

}

/**
 * Destructor is used to clean up any resources occupied by the PassiveMonitorState
 * instance.
 */
PassiveMonitorState::~PassiveMonitorState() = default;

/**
 * Responsible for invoking the appropriate functions that are responsible for
 * monitoring the location of the device.
 *
 * @param latLng The pair of latitude and longitude coordinates that are to be
 *      examined.
 * @return A pointer to the new state if this state is incapable of handling the
 *      locations, nullptr otherwise.
 */
MonitorState *PassiveMonitorState::handleLatLng(std::pair<double, double> &latLng) {

    if (!this->isInFence(latLng)) {

        // TODO: how to properly access comms / account? (currently: made
        //  them protected)
        ActiveMonitorState *activeMonitorState = new ActiveMonitorState(comms, account);
        return activeMonitorState;
    }
    return nullptr;
}
