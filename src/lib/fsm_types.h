
#ifndef FSM_SRC_LIB_FSM_TYPES_H
#define FSM_SRC_LIB_FSM_TYPES_H

#include <cstdint>
#include <forward_list>

#include "static_hash.h"

namespace fsm::types {

/**
 * @brief Trivial type to identify a state
 */
struct StateID 
{
    char const* label{"NONE"};
    std::size_t idx{0};
    util::hash_t hash{0};
};

/**
 * @brief
 */
struct EventID
{
    char const* const label;
    util::hash_t hash;
};

using state_hash_t = util::hash_t;


} // namespace fsm::types

#define DECLARE_STATE(state_name)                                                                       \
    static constexpr fsm::types::StateID id() {                                                         \
        return {.label = state_name, .hash = fsm::util::hash(state_name) }; };


#define DECLARE_EVENT(event_name) \
    static constexpr fsm::types::EventID id() { \
        return {.label = event_name, .hash = fsm::util::hash(event_name) }; };

#define TRANSITION_TO(_NextState)                                                                       \
    [[nodiscard]] fsm::action::TransitionTo<_NextState> 


#endif // FSM_SRC_LIB_FSM_TYPES_H