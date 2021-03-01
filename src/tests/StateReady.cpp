#include "StateReady.h"
#include "StateConnected.h"
#include "StateCharging.h"
#include "StatePaused.h"

fsm::action::DoNothing StateReady::handle(EventReadyInit const& p_init)
{
    std::cout << "from EventReadyInit - " << p_init.greetings << std::endl;
    return {};
}