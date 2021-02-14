
#ifndef FSM_SRC_FSM_STATE_H
#define FSM_SRC_FSM_STATE_H

#include <string_view>
#include "fsm_types.h"

namespace fsm {

template <typename Derived>
class State 
{
public:
    constexpr State() noexcept
    {
        types::StateID state_id = Derived::id();
    }

private:
    /// @brief Convenience member to access the derived type
    Derived* derived() { return static_cast<Derived*>(this); }
};

}  // namespace fsm

// #define DECLARE_STATE_CLASS(class_name, class_label) \
//     class class_name : public State<class_name> {

#endif  // FSM_SRC_FSM_STATE_H