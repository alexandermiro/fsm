
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
    static constexpr types::StateTag tag() { return { .name = TagName, .hash = util::hash(TagName) }; }

    Derived& self() { return static_cast<Derived &>(*this); }
    Derived const& self() const { return static_cast<Derived const&>(*this); }

    static constexpr std::array<types::StateTag, sizeof...(Transitions)> transitions() {
        return { Transitions::tag()... };
    }

protected:
    State() = default;

private:

};

} // namespace fsm
#endif  // FSM_SRC_LIB_STATE_H