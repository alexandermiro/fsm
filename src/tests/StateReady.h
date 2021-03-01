
#ifndef STATE_READY_H
#define STATE_READY_H

#include <iostream>

#include "state_common.h"
#include "EventReadyInit.h"
#include "EventEVGunConnected.h"
#include "State.h"


struct StateReadyTag {
    static inline char const name[] = "READY";
};

class StateCharging;
class StateConnected;
class StatePaused;

struct StateReady  : fsm::State<StateReady
                               , StateReadyTag::name
                               , StateConnected
                               , StateCharging
                               , StatePaused>
{


    // TRANSITION_TO(StateConnected) handle(EventEVGunConnected const& p_ev_gun_connected) 
    // {
    //     return fsm::action::TransitionTo<StateConnected>{};
    // }

    fsm::action::DoNothing handle(EventReadyInit const& p_init);
};

#endif  // STATE_READY_H
