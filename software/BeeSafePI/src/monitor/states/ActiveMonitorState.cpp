#include "ActiveMonitorState.h"
#include "PassiveMonitorState.h"

/**
 * Constructor explicitly initialises the ActiveMonitorState class instance.
 *
 * @param comms The communications interface for sending messages etc.
 * @param account The account that the monitor state is monitoring.
 */
ActiveMonitorState::ActiveMonitorState(Comms *const comms, Account *const account)
        : MonitorState(comms, account) {
    this->notified = false;
    // TODO: Active monitor state code.

}

/**
 * Destructor is used to clean up any resources occupied by the ActiveMonitorState
 * instance.
 */
ActiveMonitorState::~ActiveMonitorState() = default;

/**
 * Handles active location monitoring.
 *
 * When in the active monitoring state, the account will be notified
 *
 * @param latLng The pair of latitude and longitude coordinates that are to be
 *      examined.
 * @return A pointer to the new state if this state is incapable of handling the
 *      locations, nullptr otherwise.
 */
MonitorState *ActiveMonitorState::handleLatLng(std::pair<double, double> &latLng) {

    // TODO: Handle the checking of the latitude and longitude.

    /// If fences are valid, go back to passive monitoring (can be immediately!)
    if (this->isInFence(latLng)) {
        if (this->notified) {
            this->notifyBackInFence();
        }
        // TODO: how to properly access comms / account? (currently: made
        //  them protected)
        PassiveMonitorState *passiveMonitorState = new PassiveMonitorState(comms, account);
        return passiveMonitorState;
    }

    // TODO: notify account about active monitoring
    // send location somewhere?
    // check if we are back in the
    if (!this->notified) { // TODO: add timer for hysteresis?
        //this->account->getContacts()
        this->notifyExitedFence();
        this->notified = true;
    }

    // TODO: dont do it every time, only in certain time interval?

    this->sendLocation();

    return nullptr;

}

void ActiveMonitorState::notifyExitedFence() {

}


void ActiveMonitorState::notifyBackInFence() {

}

void ActiveMonitorState::sendLocation() {

}
