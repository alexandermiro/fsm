
#ifndef FSM_SRC_FSM_TRANSITION_TO_H
#define FSM_SRC_FSM_TRANSITION_TO_H

#include <variant>

namespace fsm::action {

struct DoNothing 
{
    template <typename Machine, typename State, typename Event>
	void execute(Machine*, State&, Event const&) {}
};

template <typename Action>
struct ByDefault
{
    template <typename Event>
    Action handle(Event const& p_event) const 
    {
        return {};
    }
};

using ByDefaultDoNothing = ByDefault<DoNothing>;

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
    OneOf(T&& p_arg) : m_options(std::forward<T>(p_arg)) {}

    template <typename Machine, typename State, typename Event>
    void execute(Machine* p_machine, State& p_prev_state, Event const& p_event)
    {
        std::visit([&] (auto& p_action) { p_action.execute(p_machine, p_prev_state, p_event); }, m_options);
    }
private:
    std::variant<Actions...> m_options;
};

template <typename... Actions>
struct Maybe : public OneOf<DoNothing, Actions...> 
{
    using OneOf<DoNothing, Actions...>::OneOf;
};


template <typename TargetState>
struct TransitionTo
{
    template <typename Machine, typename State, typename Event>
    void execute(Machine* p_machine, State& p_prev_state, Event const& p_event)
    {
        leave(p_prev_state, p_event);
        TargetState& new_state = p_machine->template transition_to<TargetState>();
        enter(new_state, p_event);
    }
private:
    void leave(...) {}

    template <typename State, typename Event>
    auto leave(State& p_state, Event const& p_event) -> decltype(p_state.on_leave(p_event))
    {

    }

    void enter(...) {}

    template <typename State, typename Event>
    auto enter(State& p_state, Event const& p_event) -> decltype(p_state.on_enter(p_event))
    {

    }
};

} // namespace fsm::action

#endif // FSM_SRC_FSM_TRANSITION_TO_H