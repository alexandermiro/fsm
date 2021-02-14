
#ifndef STATE_READY_H
#define STATE_READY_H

#include <iostream>

#include "fsm_types.h"
#include "EventReadyInit.h"
#include "EventEVGunConnected.h"
#include "State.h"


struct StateReady : public fsm::State<StateReady>
{
    DECLARE_STATE_ID_FUNC(fsm::types::StateCode::READY, "READY")

    void handle(EventReadyInit const& p_init) {
        std::cout << "from EventReadyInit - " << p_init.greetings << std::endl;
    }

    // void handle(EventEVGunConnected const& p_gun) {
    //     std::cout << "from EventEVGunConnected - " << p_gun.x << " " << p_gun.y << std::endl;
    // }
};

// BEGIN_DECLARE_STATE_CLASS(StateReady, READY) 

//     HANDLE_EVENT_NO_TRANSITION(EventReadyInit) {

//     }

// END_DECLARE_STATE_CLASS()

#endif  // STATE_READY_H
