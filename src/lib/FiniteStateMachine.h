
#ifndef FSM_SRC_FSM_FINITE_STATE_MACHINE_H
#define FSM_SRC_FSM_FINITE_STATE_MACHINE_H

#include <unordered_map>
#include <memory>
#include <tuple>
#include <variant>
#include <cassert>

#include "fsm_types.h"
#include "AnyState.h"
#include "for_each_tuple.h"
#include "State.h"



namespace fsm {
    
/**
 * @brief
 */
template <typename ... TStates> 
class FiniteStateMachine 
{
public:
    /**
     * @brief
     */
    constexpr std::size_t num_states() const { return sizeof...(TStates); }

    /**
     * @brief Ctor
     */
    FiniteStateMachine() noexcept
    {
        util::algo::for_each_tuple(m_states, [this] (auto& p_state, std::size_t p_idx) {
            if (auto const state_code = p_state.id().code; m_state_umap.count(state_code) == 0) {
                m_state_umap.try_emplace(state_code, p_idx);
            }
            else {
                assert(false);   // Duplicated state
            }
        });
    }

    FiniteStateMachine(FiniteStateMachine const&) = delete;
    FiniteStateMachine& operator=(FiniteStateMachine const&) = delete;

    /**
     * @brief
     */
    template <typename Event>
    void handle(Event&& p_event) 
    {
        auto handle_event = [this, p_event] (auto p_state) {
            p_state->handle(p_event); //.execute(*this);
        };
        std::visit(handle_event, m_current_state);
    }
    /**
     * @brief
     */
    template <typename State>
    void transition_to()
    {
        // m_current_state = &std::get<State>(m_states);
    }
    /**
     * @brief
     */
    // constexpr std::size_t num_states() const { return sizeof...(TStates); }

private:

    //
    // ========================================================== Attributes
    //

    std::unordered_map<types::StateCode, std::size_t> m_state_umap;

    /// @brief Contains all the states 
    std::tuple<TStates...> m_states;

    /// @brief The current state. The first state template parameter will be the very first state.

    std::variant<TStates*...> m_current_state{&std::get<0>(m_states)};
};


} // namespace fsm

#endif  //FSM_SRC_FSM_FINITE_STATE_MACHINE_H
