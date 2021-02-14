
#ifndef STATE_CHARGING_H
#define STATE_CHARGING_H

#include "StateID.h"

struct StateCharging
{
    DECLARE_STATE_ID_FUNC(fsm::States::CHARGING, "CHARGING")
};

#endif // STATE_CHARGING_H