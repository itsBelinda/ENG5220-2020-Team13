#include "Comms.h"

Comms::Comms() = default;

Comms::~Comms() = default;

bool Comms::init()
{
    bool initialised = uBlox.init();
    return initialised;
}

/**
 * Get the u-blox device that the comms interface utilises
 * for communication.
 *
 * @return An instance of the UBlox device utilised to communicate
 *      via the UART interface.
 */
const UBlox& Comms::getUBlox()
{
    return uBlox;
}

/**
 * Determines whether or not the GPRS is attached to the device.
 *
 * @param attached The bool reference into which the result of whether
 *      or not GPRS is attached will be stored.
 * @return True if the function successfully obtained the result, false
 *      otherwise.
 */
bool Comms::hasGPRS(bool &attached)
{
    // Lock the comms interface and check if GPRS is attached.
    bool rc = uBlox.hasGPRS(attached);
    return rc;
}

/**
 * Determines whether or not the PSD (internet) is connected.
 *
 * @param connected The bool reference into which the result of whether
 *      or not the PSD (internet) is connected will be stored.
 * @return True if the function successfully obtained the result, false
 *      otherwise.
 */
bool Comms::hasPSD(bool &connected)
{
    // Lock the comms interface and check if PSD connected.
    bool rc = uBlox.hasPSD(connected);
    return rc;
}
