#ifndef BEESAFEPI_PASSIVEMONITORSTATE_H
#define BEESAFEPI_PASSIVEMONITORSTATE_H


#include "MonitorState.h"

class PassiveMonitorState : public MonitorState
{

public:
    PassiveMonitorState(Communication* communication, Account* account);

};


#endif //BEESAFEPI_PASSIVEMONITORSTATE_H
