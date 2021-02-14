
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gtest_FiniteStateMachine.h"
#include "gtest_for_each_tuple.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}