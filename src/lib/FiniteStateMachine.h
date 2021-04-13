
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
        (add_to_graph(TStates::tag(), TStates::transitions()), ...);
    }

    FiniteStateMachine(FiniteStateMachine const&) = delete;
    FiniteStateMachine& operator=(FiniteStateMachine const&) = delete;

    /**
     * @brief
     */
    template <typename Event>
    void handle(Event&& p_event) 
    {
        auto handle_event = [this, event = std::move(p_event)] (auto p_state) {
            auto action = p_state->impl().handle(event); 
            action.execute(this, p_state, event);
        };
        std::visit(handle_event, m_current_state);
    }
    /**
     * @brief
     */
    template <typename State>
    State& transition_to()
    {
        auto state = &std::get<State>(m_states);
        m_current_state = state;
        return *state;
    }
    /**
     * @brief Returns the current state information
     * @return The current state tag
     */
    types::StateTag current() const 
    {
        types::StateTag curr_state_tag;
        auto tag = [&] (auto p_state) { curr_state_tag = std::move(p_state->tag()); };
        std::visit(tag, m_current_state);
        return curr_state_tag;
    }
    /**
     * @brief
     */
    types::StateTag previous() const 
    {
        types::StateTag prev_state_tag;
        auto tag = [&] (auto p_state) { prev_state_tag = std::move(p_state->tag()); };
        std::visit(tag, m_current_state);
        return prev_state_tag;
    }

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

    template <size_t N>
    constexpr void add_to_graph(types::StateTag const& p_from, std::array<types::StateTag, N> const& p_tos)
    {
        std::for_each(p_tos.begin(), p_tos.end(), [this, &p_from] (types::StateTag const& p_to) {
            m_graph.add_vertex(p_from, p_to);
        });
    }

    //
    // ========================================================== Attributes
    //

    std::unordered_map<util::hash_t, std::size_t> m_state_umap;

    /// @brief Contains all the states 
    std::tuple<TStates...> m_states;

    /// @brief The current state. The first state template parameter will be the very first state.
    std::variant<TStates*...> m_current_state{&std::get<0>(m_states)};

    std::variant<TStates*...> m_previous_state{&std::get<0>(m_states)};

    /// @brief Contains the state machine graph
    StateGraph m_graph;
};


} // namespace fsm

#endif  //FSM_SRC_FSM_FINITE_STATE_MACHINE_H
