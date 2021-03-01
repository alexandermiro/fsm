
#ifndef FSM_SRC_LIB_FSM_TYPES_H
#define FSM_SRC_LIB_FSM_TYPES_H

#include <cstdint>
#include <forward_list>

#include "static_hash.h"

namespace fsm::types {

/**
 * @brief
 */
struct EventID
{
    char const* const label;
    util::hash_t hash;
};

using state_hash_t = util::hash_t;


/**
 * @brief Trivial type to identify a state
 */
struct StateTag final 
{
    char const* name;
    types::state_hash_t hash{0u};
};

} // namespace fsm::types


#endif // FSM_SRC_LIB_FSM_TYPES_H