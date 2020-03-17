#ifndef BEESAFEPI_MONITORSTATE_H
#define BEESAFEPI_MONITORSTATE_H

#include "../../comms/Comms.h"
#include "../../device/Account.h"

#include <utility>

class MonitorState
{

protected:

    // Constructors and destructors.
    MonitorState(Comms* comms, Account* account);

public:

    // Handle the new location of the device.
    virtual MonitorState* handleLatLng(std::pair<double, double> &latLng) = 0;

private:

    // Communications and account attributes.
    Comms* comms;
    Account* account;

};


#endif //BEESAFEPI_MONITORSTATE_H
