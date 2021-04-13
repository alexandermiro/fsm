
#ifndef STATE_CHARGING_H
#define STATE_CHARGING_H

#include "state_common.h"
#include "State.h"
#include "EventReadyInit.h"
#include "EventEVGunDisconnected.h"
#include "EventUserAction.h"


class StateReady;
class StatePaused;


STATE_CLASS(StateCharging, CHARGING, StateReady, StatePaused)
{
public:
    using ByDefault::handle;

    DoNothing handle(EventReadyInit const& p_init);

    TransitionTo<StateReady> handle(EventEVGunDisconnected const& p_ev_gun_disconnected);

    Maybe<TransitionTo<StatePaused>
         , TransitionTo<StateReady>> handle(EventUserAction const& p_ev_user_action);
};

#endif // STATE_CHARGING_H