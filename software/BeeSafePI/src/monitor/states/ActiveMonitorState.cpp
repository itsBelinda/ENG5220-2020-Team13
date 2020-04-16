
#include "ActiveMonitorState.h"

#include "PassiveMonitorState.h"

// System inclusions

// Time periods after which the notifications should be send; validation periods.
#define STEPPED_OUTSIDE_NOTIFICATION_DELAY_SEC 5
#define STEPPED_INSIDE_NOTIFICATION_DELAY_SEC 20

ActiveMonitorState::ActiveMonitorState(Comms *const comms, Account *const account)
        : MonitorState(ACTIVE_STATE_NAME, comms, account)
{

    // By definition the device just crossed the perimeter.
    perimeterCrossed = true;
    sincePerimeterCrossed = std::chrono::steady_clock::now();

    // Populate the notifications.
    for (Contact *contact : account->getContacts()) {
        sentNotifications.emplace_back(std::make_pair(contact, false));
    }
}


ActiveMonitorState::~ActiveMonitorState() = default;

bool ActiveMonitorState::isPerimeterCrossed()
{
    return perimeterCrossed;
}

std::chrono::time_point<std::chrono::steady_clock> ActiveMonitorState::getSincePerimeterCrossed()
{
    return sincePerimeterCrossed;
}

std::vector<std::pair<Contact*, bool>>& ActiveMonitorState::getSentNotifications()
{
    return sentNotifications;
}

bool ActiveMonitorState::allNotificationsSent()
{
    for (auto& notifiableContact : sentNotifications) {
        if (!notifiableContact.second) {
            return false;
        }
    }
    return true;
}

void ActiveMonitorState::setSentNotifications(bool sent)
{
    for (auto& notifiableContact : sentNotifications) {
        notifiableContact.second = sent;
    }
}

void ActiveMonitorState::sendMessageNotifications(bool forceAll, std::string &message)
{
    for (auto notifiableContact : sentNotifications) {
        if (!notifiableContact.second || forceAll) {
            notifiableContact.second = comms->sendMessage(*notifiableContact.first, message);
        }
    }
}

MonitorState* ActiveMonitorState::handleLatLng(std::pair<double, double> &latLng)
{
    // Get the fence the device has left.
    Fence* crossedFence = getCrossedFence(latLng);
    bool inside = crossedFence == nullptr;

    // If the fence has been crossed, update the monitor states.
    if (inside != perimeterCrossed) {
        perimeterCrossed = inside;
        sincePerimeterCrossed = std::chrono::steady_clock::now();
        setSentNotifications(false);
    }

    // Elapsed seconds since monitor (perimeterCrossed) state change.
    auto now = std::chrono::steady_clock::now();
    long long int elapsedMs = std::chrono::duration_cast<std::chrono::seconds>
            (now - sincePerimeterCrossed).count();

    // Handle notifications.
    if (inside && elapsedMs > STEPPED_INSIDE_NOTIFICATION_DELAY_SEC) {

        // Send a message informing the contacts that the device has returned.
        std::string message = "Device " + account->getName() + " has returned to the fence.";
        sendMessageNotifications(true, message);

    } else if (!inside && elapsedMs > STEPPED_OUTSIDE_NOTIFICATION_DELAY_SEC) {

        // Send a message informing the contacts that the device has left the fence.
        std::string message = "Device " + account->getName()
                              + " has left fence " + crossedFence->getName() + ".";
        sendMessageNotifications(false, message);

    }

    // If the device has reentered the fence and parents have been notified, switch.
    return inside && elapsedMs > STEPPED_INSIDE_NOTIFICATION_DELAY_SEC
           ? new PassiveMonitorState(comms, account)
           : nullptr;
}