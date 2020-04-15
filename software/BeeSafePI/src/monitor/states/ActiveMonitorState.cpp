
#include "ActiveMonitorState.h"
#include "PassiveMonitorState.h"

#define ACTIVE_STATE_NAME "Active"

/**
 * Constructor explicitly initialises the ActiveMonitorState class instance.
 *
 * @param comms The communications interface for sending messages etc.
 * @param account The account that the monitor state is monitoring.
 */
ActiveMonitorState::ActiveMonitorState(Comms *const comms, Account *const account)
        : MonitorState(comms, account)
{
    notified = false;
    delayNotification = 0;
    lastLocTime = 0;
    // TODO: Active monitor state code.

}

/**
 * Destructor is used to clean up any resources occupied by the ActiveMonitorState
 * instance.
 */
ActiveMonitorState::~ActiveMonitorState() = default;

const char* ActiveMonitorState::getStateName()
{
    return ACTIVE_STATE_NAME;
}

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
MonitorState *ActiveMonitorState::handleLatLng(std::pair<double, double> &latLng)
{
    /// If fences are valid, go back to passive monitoring (can be immediately!)
    if (isInFence(latLng)) {
        if (notified) {
            notifyBackInFence();
        }
        // TODO: how to properly access comms / account? (currently: made
        //  them protected)
        PassiveMonitorState *passiveMonitorState = new PassiveMonitorState(comms, account);
        return passiveMonitorState;
    }

    // TODO: notify account about active monitoring
    // send location somewhere?
    // check if we are back in the
    if (!notified) {

        if (delayNotification < DELAY_NOTIFICATION_CNT) {// use cnt for now, might be updated to time like location?
            delayNotification++;
        } else {
            //account->getContacts()?
            notifyExitedFence();
            notified = true; // TODO: save notified for each contact individually (not done now, because
            // it's not clear if it can be properly identified if notification was successful
            // Additionally: is it feasible to have multiple contacts? How many are allowed?
            // How how long does it take to send it to one?
            lastLocTime = getSysTimeMS();
            // Do not send the location until exit notification has been sent.
            return nullptr;
        }
    }

    double currentTime = getSysTimeMS();

    if ((lastLocTime - currentTime) > UPDATE_LOC_MS) {
        sendLocation();
        lastLocTime = currentTime;
    }

    return nullptr;
}

/**
 * Handles notification that the device has exited the valid fence(s).
 *
 * When in the active monitoring state, a notification is sent to the contact(s) in account through the comms
 * interface, that the device is now longer in a valid fence.
 *
 */
void ActiveMonitorState::notifyExitedFence()
{
    // comms.
}

/**
 * Handles notification that the device has re-entered the valid fence(s).
 *
 * When in the active monitoring state, a notification is sent to the contact(s) in account through the comms
 * interface, that the device is back in a valid fence.
 *
 */
void ActiveMonitorState::notifyBackInFence()
{


}

/**
 * Sends the current location of the device to (where?)
 *
 * TODO: add more description
 *
 */
void ActiveMonitorState::sendLocation()
{

}

// maybe put thin in "helper functions":
/**
 * Gets the system time in ms and returns it as a double.
 *
 * @return double
 *   The system time in ms as a double.
 */
double ActiveMonitorState::getSysTimeMS()
{

    // TODO: check with DAN
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(now);;
}
