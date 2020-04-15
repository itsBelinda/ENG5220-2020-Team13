#include "MonitorState.h"

/**
 * Constructor explicitly initialises the MonitorState base class.
 *
 * @param comms The communications interface used to obtain additional information
 *      or send messages.
 * @param account The device account instance that is being monitored.
 */
MonitorState::MonitorState(const char* const stateName, Comms *const comms, Account *const account)
{
    this->stateName = stateName;
    this->comms = comms;
    this->account = account;
}

MonitorState::~MonitorState() = default;

const char* const MonitorState::getStateName()
{
    return stateName;
}

/**
 * Function checks whether or not the latitude and longitudes are within the defined fences.
 *
 * If there are no fences that have been defined, function returns true. This is because by
 * definition the device cannot be in a fence, therefore it cannot be outside of the fence.
 *
 * @param latLng A reference to the pair containing the latitude (first) and longitude (second)
 *      for the device.
 * @return True if there are no fences or the user is within all fences, false otherwise.
 */
bool MonitorState::isInFence(std::pair<double, double> &latLng)
{
    std::vector<Fence *> vectorFences = account->getFences();

    // If there are no fences defined, the device cannot be inside, return true.
    if (vectorFences.empty()) {
        return true;
    }

    // Iterate the fences within the account class to determine if the device is inside.
    for (auto &&fence : vectorFences) {
        if (!fence->isInside(latLng)) {
            return false;
        }
    }

    return true;
}
