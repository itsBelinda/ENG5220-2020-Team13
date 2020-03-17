#include "ActiveMonitorState.h"

/**
 * Constructor explicitly initialises the ActiveMonitorState class instance.
 *
 * @param comms The communications interface for sending messages etc.
 * @param account The account that the monitor state is monitoring.
 */
ActiveMonitorState::ActiveMonitorState(Comms * const comms, Account * const account)
        : MonitorState(comms, account)
{

    // TODO: Active monitor state code.

}

/**
 * Destructor is used to clean up any resources occupied by the ActiveMonitorState
 * instance.
 */
ActiveMonitorState::~ActiveMonitorState() = default;

/**
 * Responsible for invoking the appropriate functions that are responsible for
 * monitoring the location of the device.
 *
 * @param latLng The pair of latitude and longitude coordinates that are to be
 *      examined.
 * @return A pointer to the new state if this state is incapable of handling the
 *      locations, nullptr otherwise.
 */
MonitorState* ActiveMonitorState::handleLatLng(std::pair<double, double> &latLng)
{

    // TODO: Handle the checking of the latitude and longitude.

    return nullptr;
}
