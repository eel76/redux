cmake_minimum_required (VERSION 3.11)

add_executable (todo)
target_sources (todo PRIVATE src/main.cpp)
target_sources (todo PRIVATE src/action/addtodo.h)
target_sources (todo PRIVATE src/action/toggletodo.h)
target_sources (todo PRIVATE src/action/setvisibilityfilter.h)
target_sources (todo PRIVATE src/state.h)

target_include_directories(todo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries (todo PRIVATE redux)
