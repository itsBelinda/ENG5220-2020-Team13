
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

void ActiveMonitorState::sendNotifications(bool ignoreSent, const char* msg)
{

}

MonitorState* ActiveMonitorState::handleLatLng(std::pair<double, double> &latLng)
{
    // Check whether the device is inside the fence or not.
    bool inside = isInFence(latLng);
    if (inside != perimeterCrossed) {
        perimeterCrossed = inside;
        sincePerimeterCrossed = std::chrono::steady_clock::now();
        setSentNotifications(false);
    }

    // Calculate the amount of time that has passed.
    auto now = std::chrono::steady_clock::now();
    long long int elapsedMs = std::chrono::duration_cast<std::chrono::seconds>
            (now - sincePerimeterCrossed).count();

    // Notify contacts device back inside.
    if (inside && elapsedMs > STEPPED_INSIDE_NOTIFICATION_DELAY_SEC) {
        sendNotifications(true, "");
    } else if (!inside && elapsedMs > STEPPED_OUTSIDE_NOTIFICATION_DELAY_SEC) {

        // Determine whether this is the first notification or reminder.


    }

    // If the device has reentered the fence and parents have been notified, switch.
    return inside && elapsedMs > STEPPED_INSIDE_NOTIFICATION_DELAY_SEC
           ? new PassiveMonitorState(comms, account)
           : nullptr;
}