
#ifndef STATE_CHARGING_H
#define STATE_CHARGING_H

#include "state_common.h"
#include "State.h"
#include "EventReadyInit.h"

struct StateChargingTag {
    static inline char const name[] = "CHARGING";
};

class StateReady;
class StatePaused;

struct StateCharging : fsm::State<StateCharging
                                 , StateChargingTag::name
                                 , StateReady
                                 , StatePaused>
{
    fsm::action::DoNothing handle(EventReadyInit const& p_init);
};

#endif // STATE_CHARGING_H