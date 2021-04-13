
#include "StateConnected.h"
#include "StateReady.h"

#include <iostream>

#include "StatePaused.h"

fsm::action::DoNothing StateConnected::handle(EventReadyInit const& p_init)
{
    std::cout << "from EventReadyInit - " << p_init.greetings << std::endl;
    return {};
}