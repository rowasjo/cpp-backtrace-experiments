
add_library(bt_qt2_core SHARED
    core.cpp
    include/row/bt_qt2_core/core.hpp)

target_link_libraries(bt_qt2_core PUBLIC Qt6::Core)

target_include_directories(bt_qt2_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
