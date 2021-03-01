
#ifndef FSM_SRC_FSM_TRANSITION_TO_H
#define FSM_SRC_FSM_TRANSITION_TO_H

namespace fsm::action {

struct DoNothing 
{
    template <typename Machine, typename State, typename Event>
	void execute(Machine&, State&, Event const&)
	{
	}
};

template <typename State>
struct TransitionTo
{
    template <typename Machine>
    void execute(Machine* p_machine)
    {
        p_machine.template transitionTo<State>();
    }
};

} // namespace fsm::state
#endif // FSM_SRC_FSM_TRANSITION_TO_H