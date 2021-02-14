
#ifndef FSM_SRC_FSM_TRANSITION_TO_H
#define FSM_SRC_FSM_TRANSITION_TO_H

namespace fsm {

template <typename State>
struct TransitionTo
{
    template <typename Machine>
    void execute(Machine* p_machine)
    {
        machine.template transitionTo<State>();
    }
};

} // namespace fsm
#endif // FSM_SRC_FSM_TRANSITION_TO_H