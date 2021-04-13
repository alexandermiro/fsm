
#include "StateCharging.h"
#include "StateReady.h"
#include "StatePaused.h"

DoNothing StateCharging::handle(EventReadyInit const& p_init)
{
    std::cout << "from EventReadyInit - " << p_init.greetings << std::endl;
    return {};
}

TransitionTo<StateReady> StateCharging::handle(EventEVGunDisconnected const& p_ev_gun_disconnected)
{
    return TransitionTo<StateReady>{};
}

Maybe<TransitionTo<StatePaused>
      , TransitionTo<StateReady>> StateCharging::handle(EventUserAction const& p_ev_user_action)
{
    using ua = EventUserAction::Type;

    switch (p_ev_user_action.type)
    {
    case ua::Pause:
        return TransitionTo<StatePaused>{};
    case ua::Cancel:
        return TransitionTo<StateReady>{};
    case ua::None:
    case ua::ChargerInfo:
    case ua::StartCharge:
        return DoNothing{};
    }
    return DoNothing{};
}