#ifndef STATE_PAUSED_H
#define STATE_PAUSED_H

#include "StateID.h"

struct StatePaused
{
    DECLARE_STATE_ID_FUNC(fsm::States::PAUSED, "PAUSED")

};

#endif // STATE_PAUSED_H