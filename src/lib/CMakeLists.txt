
add_library(fsm-lib INTERFACE)

target_sources(fsm-lib 
    INTERFACE
    	AnyState.h
        FiniteStateMachine.h
        fsm_types.h
        TransitionTo.h
        StateGraph.h
        State.h
        state_common.h
        )

target_include_directories(fsm-lib
    INTERFACE $<BUILD_INTERFACE:${fsm_SOURCE_DIR}/include>
              $<BUILD_INTERFACE:${fsm_SOURCE_DIR}/util>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR})

target_compile_features(fsm-lib INTERFACE cxx_std_17)

