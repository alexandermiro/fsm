
#ifndef FSM_SRC_LIB_FSM_TYPES_H
#define FSM_SRC_LIB_FSM_TYPES_H

#include <cstdint>

namespace fsm::types {

/**
 * @brief Contains all possible states codes
 * @note Put here every available state
 */
enum class StateCode : std::uint8_t
{
    NONE                    = 0 
    , READY                 = 1
    , PAUSED                = 2
    , CHARGING              = 3
};

/**
 * @brief Trivial type to identify a state
 */
struct StateID 
{
    StateCode code;
    char const* const label;
    std::size_t idx;
};


enum class EventCode : std::uint32_t
{
    NONE                    = 0
};
   
/**
 * @brief
 */
struct EventID
{
    EventCode code;
    char const* const label;
};

} // namespace fsm::types

#define DECLARE_STATE_ID_FUNC(state_code, state_name) \
    static constexpr fsm::types::StateID id() { return {.code = state_code, .label = state_name}; };

#define DECLARE_EVENT_ID_FUNC(event_code, event_name) \
    static constexpr fsm::types::EventID id() { return {.code = event_code, .label = event_name}; };

#endif // FSM_SRC_LIB_FSM_TYPES_H