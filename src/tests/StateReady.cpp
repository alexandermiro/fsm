#include "StateReady.h"
#include "StateConnected.h"
#include "StateCharging.h"
#include "StatePaused.h"

DoNothing StateReady::handle(EventReadyInit const& p_init)
{
    std::cout << "StateReady received EventReadyInit\n";
    std::cout << "    from EventReadyInit - " << p_init.greetings << std::endl;
    m_initialized = true;
    return {};
}

Maybe<TransitionTo<StateConnected>> StateReady::handle(EventEVGunConnected const& p_ev_gun_connected)
{
    std::cout << "StateReady received EventEVGunConnected\n";
    if (m_initialized) 
    {
        m_initialized = true;
        return TransitionTo<StateConnected>{};
    }
    std::cout << "StateReady not initialized - must receive EventReadyInit first\n";
    return DoNothing{};
}
