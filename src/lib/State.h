
#ifndef FSM_SRC_LIB_STATE_H
#define FSM_SRC_LIB_STATE_H

#include <algorithm>
#include <type_traits>
#include <array>

#include "fsm_types.h"

namespace fsm {

/**
 * @brief
 */
struct StateTag final 
{
    char const* name;
    types::state_hash_t hash{0u};
};

template <typename... T>
struct All : std::true_type {};

/**
 * @brief
 */
template <typename Derived
         , char const* TagName
         , typename... Transitions>
class State 
{
public:
    static constexpr StateTag tag() { return { .name = TagName, .hash = util::hash(TagName) }; }

    Derived& self() { return static_cast<Derived &>(*this); }
    Derived const& self() const { return static_cast<Derived const&>(*this); }

    constexpr std::array<StateTag, sizeof...(Transitions)> transitions() {
        return { Transitions::tag()... };
    }

private:
    // constexpr char const* const name() const { return TagName; }
    // constexpr util::hash_t hash() const { return util::hash(TagName); }

};

} // namespace fsm
#endif  // FSM_SRC_LIB_STATE_H