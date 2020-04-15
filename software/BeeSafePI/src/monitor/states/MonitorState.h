/**
 * \class MonitorState
 *
 * \defgroup monitor
 * \ingroup monitor
 *
 * \brief Provide an example
 *
 * This class is meant as an example.  It is not useful by itself
 * rather its usefulness is only a function of how much it helps
 * the reader.  It is in a sense defined by the person who reads it
 * and otherwise does not exist in any real form.
 *
 * \author $Author: bk $
 *
 */

#ifndef BEESAFEPI_MONITORSTATE_H
#define BEESAFEPI_MONITORSTATE_H

#include "../../comms/Comms.h"
#include "../../device/Account.h"

#include <utility>


class MonitorState {

public:

    // Constructors and destructors.
    MonitorState(Comms *comms, Account *account);
    virtual ~MonitorState();

public:

    // Get the state name.
    virtual const char* getStateName() = 0;

    // Handle the new location of the device.
    virtual MonitorState *handleLatLng(std::pair<double, double> &latLng) = 0;

protected:

    // Communications and account attributes.
    Comms *comms;
    Account *account;

    // Override the interface responsible for handling the latitude and longitude.
    bool isInFence(std::pair<double, double> &latLng);

};


#endif //BEESAFEPI_MONITORSTATE_H
