#ifndef STATE_PAUSED_H
#define STATE_PAUSED_H

#include "state_common.h"
#include "State.h"
#include "EventReadyInit.h"

struct StatePausedTag {
    static inline char const name[] = "PAUSED";
};


class StateCharging;

struct StatePaused : fsm::State<StatePaused, StatePausedTag::name, StateCharging>
{
    fsm::action::DoNothing handle(EventReadyInit const& p_init);
};

#endif // STATE_PAUSED_H