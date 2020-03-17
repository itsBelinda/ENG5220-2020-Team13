#include "MonitorState.h"

/**
 * Constructor explicitly initialises the MonitorState base class.
 *
 * @param comms The communications interface used to obtain additional information
 *      or send messages.
 * @param account The device account instance that is being monitored.
 */
MonitorState::MonitorState(Comms * const comms, Account * const account)
{
    this->comms = comms;
    this->account = account;
}