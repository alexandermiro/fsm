
#ifndef STATE_CONNECTED_H
#define STATE_CONNECTED_H

#include "state_common.h"
#include "State.h"
#include "EventReadyInit.h"

class StateReady;
class StatePaused;

STATE_CLASS(StateConnected, CONNECTED, StatePaused, StateReady)
{
public:
    using ByDefault::handle;

    fsm::action::DoNothing handle(EventReadyInit const& p_init);

};

#endif // STATE_CONNECTED_H