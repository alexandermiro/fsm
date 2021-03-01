
#include "StateCharging.h"
#include "StateReady.h"
#include "StatePaused.h"

fsm::action::DoNothing StateCharging::handle(EventReadyInit const& p_init)
{
    std::cout << "from EventReadyInit - " << p_init.greetings << std::endl;
    return {};
}