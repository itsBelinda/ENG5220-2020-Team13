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
 * Function will block concurrent access to the comms interface to prevent
 * errors / side effects from occurring. Handling locking on lower levels becomes
 * too complex / messy given the interoperability of the underlying functions,
 * hence here.
 *
 * @param attached The bool reference into which the result of whether
 *      or not GPRS is attached will be stored.
 * @return True if the function successfully obtained the result, false
 *      otherwise.
 */
bool Comms::hasGPRS(bool &attached)
{
    // Lock the comms interface and check if GPRS is attached.
    mtx.lock();
    bool rc = uBlox.hasGPRS(attached);
    mtx.unlock();

    // Return the result of the function.
    return rc;
}

/**
 * Determines whether or not the PSD (internet) is connected.
 *
 * Function will block concurrent access to the comms interface to prevent
 * errors / side effects from occurring. Handling locking on lower levels becomes
 * too complex / messy given the interoperability of the underlying functions,
 * hence here.
 *
 * @param connected The bool reference into which the result of whether
 *      or not the PSD (internet) is connected will be stored.
 * @return True if the function successfully obtained the result, false
 *      otherwise.
 */
bool Comms::hasPSD(bool &connected)
{
    // Lock the comms interface and check if PSD connected.
    mtx.lock();
    bool rc = uBlox.hasPSD(connected);
    mtx.unlock();

    // Return the result of the function.
    return rc;
}

/**
 * Get the model number of the u-Blox device.
 *
 * Function will block concurrent access to the comms interface to prevent
 * errors / side effects from occurring. Handling locking on lower levels becomes
 * too complex / messy given the interoperability of the underlying functions,
 * hence here.
 *
 * @param modelNumber The string reference into which the result i.e.
 *      model number will be stored.
 * @return True if the function successfully obtained the result, false
 *      otherwise.
 */
bool Comms::getModelNumber(std::string &modelNumber)
{
    // Locks the comms interface and get the model number.
    mtx.lock();
    bool rc = uBlox.getModelNumber(modelNumber);
    mtx.unlock();

    // Return the result of the function.
    return rc;
}

/**
 * Get the IMEI code associated with the device.
 *
 * @param imei The string reference into which the result i.e. imei will
 *      be stored.
 * @return  True if the function successfully obtained the result, false
 *      otherwise.
 */
bool Comms::getIMEI(std::string &imei)
{
    // Locks the comms interface and get the imei number.
    bool rc = uBlox.getIMEI(imei);
    return rc;
}

