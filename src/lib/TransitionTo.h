
#ifndef FSM_SRC_FSM_TRANSITION_TO_H
#define FSM_SRC_FSM_TRANSITION_TO_H

#include <variant>

namespace fsm::action {

struct DoNothing 
{
    template <typename Machine, typename State, typename Event>
	void execute(Machine&, State&, Event const&) { }
};

/**
 * @brief
 */
template <typename Event, typename Action>
struct On
{
    Action handle(Event const& p_event) const 
    {
        return {};
    }
};

template <typename... Actions>
class OneOf 
{
public:
    template <typename T>
    OneOf(T&& p_arg)
        : m_options(std::forward<T>(p_arg))
    {
    }

    template <typename Machine>
    void execute(Machine& p_machine)
    {
        std::visit([&p_machine](auto& p_action) { p_action.execute(p_machine); }, m_options);
    }
private:
    std::variant<Actions...> m_options;
};

template <typename Action>
struct Maybe : public OneOf<Action, DoNothing> 
{
    using OneOf<Action, DoNothing>::OneOf;
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

} // namespace fsm::action
#endif // FSM_SRC_FSM_TRANSITION_TO_H