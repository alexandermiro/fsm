
#include "StatePaused.h"

#include <iostream>

#include "StateCharging.h"

fsm::action::DoNothing StatePaused::handle(EventReadyInit const& p_init)
{
    std::cout << "from EventReadyInit - " << p_init.greetings << std::endl;
    return {};
}