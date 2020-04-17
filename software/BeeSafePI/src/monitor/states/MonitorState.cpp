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

Fence* MonitorState::getCrossedFence(std::pair<double, double> &latLng)
{
    std::vector<Fence *> vectorFences = account->getFences();

    // If there are no fences, then return nullptr i.e. no fence to be outside of.
    if (vectorFences.empty()) {
        return nullptr;
    }

    // Iterate the fences within the account class to determine if the device is inside.
    for (auto &&fence : vectorFences) {
        if (!fence->isInside(latLng)) {
            return fence;
        }
    }

    return nullptr;
}
