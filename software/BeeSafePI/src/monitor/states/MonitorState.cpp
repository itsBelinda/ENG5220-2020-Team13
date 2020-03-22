#include "MonitorState.h"

/**
 * Constructor explicitly initialises the MonitorState base class.
 *
 * @param comms The communications interface used to obtain additional information
 *      or send messages.
 * @param account The device account instance that is being monitored.
 */
MonitorState::MonitorState(Comms *const comms, Account *const account) {
    this->comms = comms;
    this->account = account;
}

/**
 * Destructor is used to release / free any resources occupied by the
 * MonitorState base class instance. Note, no resources are used, thus
 * the default destructor is utilised.
 */
MonitorState::~MonitorState() = default;

//TODO: do this here or have function in account? (Account::isInside(latLng))
// IMO it makes more sense to be here... but not 100% sure.
/**
 * Checks coordinates if they are in the fence.
 *
 * Loops through all the fences in Account and checks if they are fulfilled.
 * If there is no fence, false is returned. If the fence has a time associated,
 * it needs to be in the fence during the time.
 *
 * @param std::pair<double, double> latLng
 *  The pair of latitude and longitude coordinates that are to be
 *  examined.
 * @return bool
 *  True if the coordinates are within the specified fences, false if
 *  not or if there are no fences specified.
 */
bool MonitorState::isInFence(std::pair<double, double> &latLng) {
    std::vector<Fence *> vectorFences = this->account->getFences();

    /// If there are no fences defined, the device cannot be inside, return false
    if (vectorFences.size() == 0) {
        return false;
    }

    //for (std::vector<Fence *>::iterator fence = vectorFences.begin() ; fence != vectorFences.end(); ++fence)
    /// Iterate through all the fences in account and check if position is inside.

    //TODO: probably needs to be rewritten #6
    // check with team for definition
    for (auto &&fence : vectorFences) {
        if (!fence->isInside(latLng)) {
            return false;
        }

    }

    /// If the code ran until the end, all fences are fine.
    return true;
}
