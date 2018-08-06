cmake_minimum_required (VERSION 3.11)

add_library (redux INTERFACE)

target_sources (redux INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/redux.h)
target_sources (redux INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/redux/detail/get.h)
target_sources (redux INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/redux/detail/overloaded.h)
target_sources (redux INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/redux/eventloop.h)
target_sources (redux INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/redux/reducer.h)
target_sources (redux INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/redux/store.h)

target_include_directories(redux
    INTERFACE
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

target_compile_features(redux INTERFACE cxx_std_17)
