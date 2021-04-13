
#ifndef STATE_READY_H
#define STATE_READY_H

#include <iostream>

#include "state_common.h"
#include "EventReadyInit.h"
#include "EventEVGunConnected.h"
#include "State.h"


// struct StateReadyTag {
//     static inline char const name[] = "READY";
// };

class StateCharging;
class StateConnected;
class StatePaused;

using namespace fsm::action;


STATE_CLASS(StateReady, READY, StateConnected, StateCharging, StatePaused)
{
public:
    using ByDefault::handle;        // TODO: eliminate this injecting the handler automatically

    Maybe<TransitionTo<StateConnected>> handle(EventEVGunConnected const& p_ev_gun_connected);

    DoNothing handle(EventReadyInit const& p_init);

private:
    bool m_initialized{false};
};

#endif  // STATE_READY_H
