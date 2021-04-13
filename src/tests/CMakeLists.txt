
cmake_minimum_required(VERSION 3.16.3)

find_package(GTest REQUIRED)
include(GoogleTest)

set(subproj fsm_tests)
set(CMAKE_BUILD_TYPE Debug)


add_executable(${subproj}
    main.cpp
    gtest_FiniteStateMachine.h
    gtest_for_each_tuple.h
    StatePaused.h
    StatePaused.cpp
    StateConnected.h
    StateConnected.cpp
    StateReady.h
    StateReady.cpp
    StateCharging.h
    StateCharging.cpp
)

set_property(TARGET ${subproj} PROPERTY CXX_STANDARD 17)

target_include_directories(${subproj}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_HOME_DIRECTORY}/src/lib>
        $<BUILD_INTERFACE:${CMAKE_HOME_DIRECTORY}/src/util>
)

add_compile_definitions(DISABLE_LOG_SYSTEM)        

target_link_libraries(${subproj}
   PUBLIC
       GTest::GTest 
       GTest::Main
       pthread
       )

# target_link_libraries(${subproj} PUBLIC gtest gmock pthread)

target_compile_options(${subproj} PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall>
     $<$<CXX_COMPILER_ID:MSVC>:/W4>)

set_target_properties(${subproj}
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )


enable_testing()
gtest_add_tests(TARGET      ${subproj}
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests)

add_test(NAME monolithic COMMAND ${subproj})
