
#ifndef FSM_SRC_FSM_FINITE_STATE_MACHINE_H
#define FSM_SRC_FSM_FINITE_STATE_MACHINE_H

#include <unordered_map>
#include <memory>
#include <tuple>
#include <variant>
#include <cassert>
#include <string_view>

#include "fsm_types.h"
#include "AnyState.h"
#include "for_each_tuple.h"
#include "StateGraph.h"
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
        (add_to_graph(TStates::tag()), ...);
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
    State& transition_to()
    {
        State& state = &std::get<State>(m_states);
        m_current_state = &state;
        return state;
    }
    /**
     * @brief
     */

private:
    void generate_graph()
    {
        // util::algo::for_each_tuple(m_states, [this] (auto& p_state, std::size_t p_idx) {

            // auto const & transitions = p_state.transitions_to;

            // std::for_each(transitions.begin(), transitions.end()
            //              , [this] (Vertex const& p_vertex_to) {
            //                     m_graph.add_vertex(Vertex{p_state.id()}, p_vertex_to);
            // });

            // if (auto const state_hash = p_state.id().hash; m_state_umap.count(state_hash) == 0) {
            //     m_state_umap.try_emplace(state_hash, p_idx);
            // }
            // else {
            //     assert(false);   // Duplicated state
            // }
        // });


    }

    void add_to_graph(StateTag p_tag)
    {

    }

    //
    // ========================================================== Attributes
    //

    std::unordered_map<util::hash_t, std::size_t> m_state_umap;

    /// @brief Contains all the states 
    std::tuple<TStates...> m_states;

    /// @brief The current state. The first state template parameter will be the very first state.
    std::variant<TStates*...> m_current_state{&std::get<0>(m_states)};

    StateGraph m_graph;
};


} // namespace fsm

#endif  //FSM_SRC_FSM_FINITE_STATE_MACHINE_H
