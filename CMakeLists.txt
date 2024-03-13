cmake_minimum_required(VERSION 3.16)

include(CMakePrintHelpers)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS FALSE)

project(BacktraceExperiments VERSION 0.1.0 LANGUAGES CXX)
message(STATUS "Configuring project ${CMAKE_PROJECT_NAME} ...")
enable_testing()

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "" OFF)

if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core)

add_subdirectory(bt_std)
add_subdirectory(bt_qt)
