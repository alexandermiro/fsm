
add_library(fsm-util INTERFACE)

target_sources(fsm-util
    INTERFACE
        common.h
        Anomaly.h	
        Logger.h
        LogSignal.h
        SyncStaticQueue.h
        for_each_tuple.h
        )

target_include_directories(
    fsm-util
    INTERFACE $<BUILD_INTERFACE:${fsm_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR})

target_compile_features(fsm-util INTERFACE cxx_std_17)

