
#ifndef FSM_SRC_LIB_STATE_METADATA_H
#define FSM_SRC_LIB_STATE_METADATA_H

#include <forward_list>
#include "fsm_types.h"

namespace fsm {

struct StateMetadata;
using transitions_flist_t = std::forward_list<StateMetadata>;

/**
 * @brief
 */
struct StateID final
{
    char const* name;
    types::state_hash_t hash{0u};
};

struct StateIDTransitions {
    std::forward_list<StateID> transitions_to;

    void add(StateID p_next_state) 
    {
        transitions_to.emplace_front(std::move(p_next_state));
    };
};

}  // namespace fsm

#define DECLARE_STATE_METADATA(_StateName) \
    static constexpr fsm::StateID Id{ .name = _StateName, .hash = fsm::util::hash(_StateName) }; \
    static inline fsm::StateIDTransitions IdTxn;


#endif  // FSM_SRC_LIB_STATE_METADATA_H