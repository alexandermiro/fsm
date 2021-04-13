#ifndef STATE_PAUSED_H
#define STATE_PAUSED_H

#include "state_common.h"
#include "State.h"
#include "EventReadyInit.h"


class StateReady;
class StateCharging;

STATE_CLASS(StatePaused, PAUSED, StateCharging, StateReady)
{
public:
    using ByDefault::handle;

    fsm::action::DoNothing handle(EventReadyInit const& p_init);
};

#endif // STATE_PAUSED_H