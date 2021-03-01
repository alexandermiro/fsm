
#ifndef STATE_CONNECTED_H
#define STATE_CONNECTED_H

#include "state_common.h"
#include "State.h"
#include "EventReadyInit.h"

class StatePaused;

struct StateConnectedTag {
    static inline char const name[] = "CONNECTED";
};

struct StateConnected : fsm::State<StateConnected, StateConnectedTag::name, StatePaused>
{

    fsm::action::DoNothing handle(EventReadyInit const& p_init);

};

#endif // STATE_CONNECTED_H