#ifndef BEESAFEPI_ACTIVEMONITORSTATE_H
#define BEESAFEPI_ACTIVEMONITORSTATE_H

#include "MonitorState.h"

class ActiveMonitorState : public MonitorState
{

public:
    ActiveMonitorState(Communication* communication, Account* account);

};


#endif //BEESAFEPI_ACTIVEMONITORSTATE_H
