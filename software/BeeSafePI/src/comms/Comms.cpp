/**
 * \file Comms.cpp
 * \class Comms
 *
 * \ingroup Comms
 *
 * \brief A high-level wrapper for the communication features
 *
 * The Comms class acts as a high-level wrapper over the UBlox class, ensuring concurrency-safe access to the hardware. By calling the UBlox
 * commands describing higher level communications functions such as getting a location and sending a text message, and "wrapping it" in a mutex lock
 * it ensures the safe interactions with and operations of the hardware connected to the Raspberry Pi.
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

#include "Comms.h"

/**
 * The Comms class constructor.
 */
Comms::Comms() = default;

/**
 * The Comms class destructor.
 */
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
    mtx.unlock();

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
 * Checks whether or not the device sim is registered with a network.
 *
 * @param registered The bool reference into which the result is to be stored.
 * @return True if the device successfully returned the result, false
 *      otherwise.
 */
bool Comms::hasRegistered(bool &registered)
{
    // Lock comms and check if the device is registered to the network.
    mtx.lock();
    bool rc = uBlox.hasRegistered(registered);
    mtx.unlock();

    // Return the state of the command.
    return rc;
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
 * Starts the auto registration process if the sim has not already been
 * registered,
 *
 * @param registered The bool reference into which the auto registration
 *      result is to be stored.
 * @return True if the command was successfully run, false otherwise.
 */
bool Comms::startAutoRegistration(bool &registered)
{
    // Lock comms and start the auto registration.
    mtx.lock();
    bool rc = uBlox.startAutoRegistration(registered);
    mtx.unlock();

    // Return the state of the command.
    return rc;
}

/**
 * Attempt to connect to the internet.
 *
 * @param connected A bool reference into which the result is to be stored i.e.
 *      whether a connection was successfully established.
 * @param urc A reference into which additional information is to be stored.
 * @return True if the command was successfully executed, false otherwise.
 */
bool Comms::connectPSD(bool &connected, std::string &urc)
{
    // Lock the comms interface and connect.
    mtx.lock();
    bool rc = uBlox.connectPSD(connected, urc);
    mtx.unlock();

    // Return the state of the command.
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
 * Get the send message mode utilised by the device to send text messages.
 *
 * @param sendMsgMode The char reference into which the mode (SEND_TEXT_MODE_PDU
 *      or SEND_TEXT_MODE_TEXT) is to be stored.
 * @return True if the function successfully obtained the send message mode
 *      false otherwise.
 */
bool Comms::getSendMessageMode(char &sendMsgMode)
{
    // Lock the comms interface and get the message mode.
    mtx.lock();
    bool rc = uBlox.getSendMessageMode(sendMsgMode);
    mtx.unlock();

    // Return the state of the function.
    return rc;
}

/**
 * Setting the send message mode that is to be used for sending text messages
 * to a mobile device.
 *
 * @param sendMsgMode The mode that's to be utilised (SEND_TEXT_MODE_PDU
 *      or SEND_TEXT_MODE_TEXT).
 * @return True if the mode was successfully set, false otherwise.
 */
bool Comms::setSendMessageMode(const char sendMsgMode)
{
    // Lock the comms interface and set the message mode.
    mtx.lock();
    bool rc = uBlox.setSendMessageMode(sendMsgMode);
    mtx.unlock();

    // Return the state of the function.
    return rc;
}

/**
 * Get the CellLocate location scan mode that's utilised for obtaining the
 * latitude and longitude coordinates.
 *
 * @param locScanMode The char reference into which the mode (LOC_SCAN_MODE_NORMAL
 *      or LOC_SCAN_MODE_DEEP) is to be stored.
 * @return True if the function successfully returned a result, false otherwise.
 */
bool Comms::getLocationScanMode(char &locScanMode)
{
    // Lock the comms and get the location scan mode.
    mtx.lock();
    bool rc = uBlox.getLocationScanMode(locScanMode);
    mtx.unlock();

    // Return the state of the function.
    return rc;
}

/**
 * Set the CellLocate location scan mode that is to be used for obtaining
 * the latitude and longitude coordinates.
 *
 * @param locScanMode The mode (LOC_SCAN_MODE_NORMAL or LOC_SCAN_MODE_DEEP) that is
 *      to be used for obtaining the location.
 * @return True if the function successfully set the location scan mode, false
 *      otherwise.
 */
bool Comms::setLocationScanMode(const char locScanMode)
{
    // Lock comms and set the location scan mode.
    mtx.lock();
    bool rc = uBlox.setLocationScanMode(locScanMode);
    mtx.unlock();

    // Return the state of the function.
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