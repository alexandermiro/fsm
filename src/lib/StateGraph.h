
#ifndef FSM_SRC_LIB_STATE_GRAPH_H
#define FSM_SRC_LIB_STATE_GRAPH_H

#include <forward_list>
#include <unordered_map>

#include "fsm_types.h"
#include "static_hash.h"
#include "StateMetadata.h"


namespace fsm {


using transition_flist_t = std::forward_list<types::state_hash_t>;

/**
 * @brief
 */
class StateGraph
{
public:
    void add_vertex(StateID&& p_vertex_from, StateID&& p_vertex_to) 
    {
        auto [it_from, ret_from] = m_vertices.try_emplace(p_vertex_from.hash, std::move(p_vertex_from));
        auto [it_to, ret_to] = m_vertices.try_emplace(p_vertex_from.hash, std::move(p_vertex_from));

        auto const id_from = it_from->first;
        auto const id_to = it_to->first;

        if (auto [it, inserted] = m_transitions.emplace(id_from, transition_flist_t{id_to}); !inserted)
        {
            m_transitions[id_from].emplace_front(id_to);
        }
        m_transitions.try_emplace(id_to, transition_flist_t{});
    }

private:
    std::unordered_map<types::state_hash_t, StateID> m_vertices;    
    std::unordered_map<types::state_hash_t, transition_flist_t> m_transitions;    
};

} // namespace fsm

#endif // FSM_SRC_LIB_STATE_GRAPH_H