#ifndef FSM_SRC_TESTS_GTEST_FINITE_STATE_MACHINE_H
#define FSM_SRC_TESTS_GTEST_FINITE_STATE_MACHINE_H

#include "FiniteStateMachine.h"
#include "StateCharging.h"
#include "StateConnected.h"
#include "StatePaused.h"
#include "StateReady.h"
#include "StateScheduled.h"
#include "EventReadyInit.h"
#include "EventEVGunConnected.h"
#include "fsm_types.h"

#include <iostream>

using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::TypedEq;
using ::testing::_;
using ::testing::StrictMock;


TEST(FiniteStateMachine_event, GIVEN_wrong_order_event_THEN_do_not_change_state)
{
    fsm::FiniteStateMachine<StateReady, StateConnected, StatePaused, StateCharging> fsm;

    EventEVGunConnected evt_ev_gun_connected;
    fsm.handle(evt_ev_gun_connected);

    EventReadyInit evt_ready_init{"Hi dear user"};
    fsm.handle(evt_ready_init);

    auto const state_tag = fsm.current();

    EXPECT_STREQ(state_tag.name, StateReadyTag::name);
}

TEST(FiniteStateMachine_event, GIVEN_correct_order_event_THEN_change_state)
{
    fsm::FiniteStateMachine<StateReady, StateConnected, StatePaused, StateCharging> fsm;

    EventReadyInit evt_ready_init{"Hi dear user"};
    fsm.handle(evt_ready_init);

    EventEVGunConnected evt_ev_gun_connected;
    fsm.handle(evt_ev_gun_connected);

    auto const state_tag = fsm.current();

    EXPECT_STREQ(state_tag.name, StateConnectedTag::name);
}

/**
 * @brief
 */
class ChargerFsmTest: public ::testing::Test
{
public:
    void SetUp()
    {
    }

    void TearDown()
    {
    }

    struct UnknownEvent { };
    
    // fsm::FiniteStateMachine<StateReady, StateConnected, StatePaused, StateCharging> m_fsm;
    fsm::FiniteStateMachine<StateReady> m_fsm;
};


TEST_F(ChargerFsmTest, GIVEN_eventNotSupported_THEN_doesNothing)
{
    m_fsm.handle(UnknownEvent{});
}

#endif  // FSM_SRC_TESTS_GTEST_FINITE_STATE_MACHINE_H