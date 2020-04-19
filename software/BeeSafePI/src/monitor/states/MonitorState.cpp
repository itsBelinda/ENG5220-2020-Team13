/**
 * \file MonitorState.cpp
 * \class MonitorState
 *
 * \ingroup Monitor
 *
 * \brief The parent monitor state class
 *
 * The parent class to ActiveMonitorState and PassiveMonitorState. It provides generic functionality
 * by defining how to handle when the device crosses a fence boundary.
 *
 * \author BeeSafe Team, Team 13
 *
 * \version v1.0
 *
 * \date 2020/04/20
 *
 * Contact: beesafe.uofg@gmail.com
 *
 * Licence: MIT
 */

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
