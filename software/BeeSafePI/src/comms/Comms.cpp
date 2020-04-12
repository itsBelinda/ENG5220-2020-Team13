#include "Comms.h"

Comms::Comms() = default;

Comms::~Comms() = default;

/**
 * Initialise or re-initialise the comms interface. Any prior non-default
 * configurations will be reset.
 *
 * @return True if the comms interface was successfully initialised, false
 *      otherwise.
 */
bool Comms::init()
{
    // Lock and initialise the comms interface.
    mtx.lock();
    bool initialised = uBlox.init();
    mtx.lock();

    // Whether or not the comms was successfully initialised.
    return initialised;
}

/**
 * Get the u-blox device that the comms interface utilises
 * for communication.
 *
 * Invocation of the function does not lock the comms interface.
 * Moreover, there are no locks associated with underlying structures.
 * Thus, this function (and the returned instance of UBlox) should be
 * used with extreme care.
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
    mtx.lock();
    bool rc = uBlox.hasGPRS(attached);
    mtx.unlock();

    // Return the result of the function.
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
    mtx.lock();
    bool rc = uBlox.hasPSD(connected);
    mtx.unlock();

    // Return the result of the function.
    return rc;
}

/**
 * Get the model number of the u-Blox device.
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
 * Get the IMEI number associated with the device.
 *
 * @param imeiNumber The string reference into which the result i.e. imei will
 *      be stored.
 * @return  True if the function successfully obtained the result, false
 *      otherwise.
 */
bool Comms::getIMEI(std::string &imeiNumber)
{
    // Locks the comms interface and get the imeiNumber number.
    mtx.lock();
    bool rc = uBlox.getIMEI(imeiNumber);
    mtx.unlock();

    // Return the result of the function.
    return rc;
}

/**
 * Get the location of the device on which the code is being run.
 *
 * Note, the function delegates the locking to the
 * getLocation(double &lat, double &lng).
 *
 * @param latLng The pair structure into which the result is to be placed.
 *      'first' represents the latitude whereas 'second' represents the
 *      latitude.
 * @return True if the function returned successfully, false otherwise.
 */
bool Comms::getLocation(std::pair<double, double> &latLng)
{
    return getLocation(latLng.first, latLng.second);
}

/**
 * Get the location of the device on which the code is being run.
 *
 * @param lat The geo-location latitude.
 * @param lng The geo-location longitude.
 * @return True if the function returned successfully, false otherwise.
 */
bool Comms::getLocation(double &lat, double &lng)
{
    // Lock the comms interface and get the latitude and longitude.
    mtx.lock();
    bool rc = uBlox.getLocation(lat, lng);
    mtx.unlock();

    // Return the result of the operation.
    return rc;
}

/**
 * Send a message to a contacts phone number via the UBlox device.
 *
 * Note, the function delegates the locking to the
 * sendMessage(string &phoneNumber, string &message) function.
 *
 * @param contact The contact (phone number) to which the message will be sent.
 * @param message The message contents that are to be sent.
 * @return True if the message was successfully sent, false otherwise.
 */
bool Comms::sendMessage(Contact &contact, const std::string &message)
{
   return sendMessage(contact.getNumber(), message);
}

/**
 * Send a message to a mobile phone via the UBlox device.
 *
 * @param phoneNumber The phone number to which the message will be sent.
 * @param message The message contents that are to be sent.
 * @return True if the message was sent successfully, false otherwise.
 */
bool Comms::sendMessage(const std::string &phoneNumber, const std::string &message)
{
    // Lock the comms interface and set a message.
    mtx.lock();
    bool rc = uBlox.sendMessage(phoneNumber, message);
    mtx.unlock();

    // Return the result of the operation.
    return rc;
}