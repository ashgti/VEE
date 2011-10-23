cmake_minimum_required (VERSION 2.6)
project (VEE)

include_directories ("${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include")

add_definitions(-O3 -g -Wall -W -Wno-unused-parameter -Wwrite-strings -pedantic -Wno-long-long)

if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
  message(STATUS "OS X detected, building universal binary.")
endif(APPLE)

set(VEE_SRC_FILE "student.cc" CACHE STRING "Target src code")

add_library(vee SHARED src/hardware_serial.cc
                       src/pin_commands.cc
                       src/time_commands.cc
                       src/env.cc
                       src/ardulator.cc
                       src/pin_config.cc
                       src/interrupt_commands.cc
                       src/bit_value.cc
                       src/interface.cc
                       src/emulator_commands.cc
                       src/interface.cc
                       ${VEE_SRC_FILE})


