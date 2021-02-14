#ifndef FSM_SRC_TESTS_GTEST_FINITE_STATE_MACHINE_H
#define FSM_SRC_TESTS_GTEST_FINITE_STATE_MACHINE_H

#include "FiniteStateMachine.h"
// #include "StateCharging.h"
#include "StateConnected.h"
// #include "StatePaused.h"
#include "StateReady.h"
#include "StateScheduled.h"
#include "EventReadyInit.h"
#include "EventEVGunConnected.h"
#include "fsm_types.h"


using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::TypedEq;
using ::testing::_;
using ::testing::StrictMock;


TEST(FiniteStateMachine_creation, GIVEN_aCertainNumberOfStates_THEN_numStatesMustBeCorrect)
{
    {
        // fsm::FiniteStateMachine<StateReady> fsm;
        // EXPECT_EQ(fsm.num_states(), 1);
    }
    {
        // fsm::FiniteStateMachine<StateReady, StateCharging, StatePaused> fsm;
        // EXPECT_EQ(fsm.num_states(), 3);
    }
}

TEST(FiniteStateMachine_stateID, GIVEN_states_THEN_returnCorrectStateIdentification)
{
    // fsm::FiniteStateMachine<StateReady, StateCharging, StatePaused> fsm;

    // EXPECT_TRUE(fsm.has(StateReady::id()));
    // EXPECT_TRUE(fsm.has(StateCharging::id()));
    // EXPECT_TRUE(fsm.has(StatePaused::id()));
}

TEST(FiniteStateMachine_event, GIVEN_event_THEN_handleIt)
{
    fsm::FiniteStateMachine<StateReady> fsm;

    EventReadyInit event{"Hi dear user"};
    fsm.handle(event);
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

    
};

#endif  // FSM_SRC_TESTS_GTEST_FINITE_STATE_MACHINE_H