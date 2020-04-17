#ifndef BEESAFEPI_ACTIVEMONITORSTATE_H
#define BEESAFEPI_ACTIVEMONITORSTATE_H

#include "MonitorState.h"

#include "../../contact/Contact.h"

// System inclusions.
#include <chrono>
#include <map>
#include <utility>

// The name of the state.
#define ACTIVE_STATE_NAME "Active"


//! Delay counter how many times the location has to be updated outside of the
//! fence before a notification is sent.
#define DELAY_NOTIFICATION_CNT  10
//! Defines how often the location update should be sent in a time interval defined in milliseconds (ms).
#define UPDATE_LOC_MS  1000

class ActiveMonitorState : public MonitorState
{

public:

    // Constructors and destructors.
    ActiveMonitorState(Comms* comms, Account* account);
    ~ActiveMonitorState() override;

public:

    // Get state / time since perimeter crossed crossed.
    bool isPerimeterCrossed();
    std::chrono::time_point<std::chrono::steady_clock> getSincePerimeterCrossed();

    // A mapping for the sent notifications.
    std::vector<std::pair<Contact*, bool>> &getSentNotifications();

    // Override the interface responsible for handling the latitude and longitude.
    MonitorState* handleLatLng(std::pair<double, double> &latLng) override;

private:

    // Notification related functions.
    bool allNotificationsSent();
    void setSentNotifications(bool sent);
    void sendMessageNotifications(bool forceAll, std::string &message);

private:

    // Whether the perimeter has been crossed.
    bool perimeterCrossed;
    std::chrono::time_point<std::chrono::steady_clock> sincePerimeterCrossed;

    // Whether or not the parents have been notified.
    std::vector<std::pair<Contact*, bool>> sentNotifications;

};


#endif //BEESAFEPI_ACTIVEMONITORSTATE_H
