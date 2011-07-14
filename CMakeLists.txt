cmake_minimum_required (VERSION 2.6)
project (VEE)

include_directories ("${PROJECT_SOURCE_DIR}/include")

add_definitions(-Wall -W -Wno-unused-parameter -Wwrite-strings -pedantic -Wno-long-long)

if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
  message(STATUS "OS X detected, building universal binary.")
endif(APPLE)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-fPIC" SUPPORTS_FPIC_FLAG)
if( SUPPORTS_FPIC_FLAG )
  message(STATUS "Building with -fPIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
else( SUPPORTS_FPIC_FLAG )
  message(WARNING "-fPIC not supported.")
endif()

add_library(vee SHARED src/hardware_serial.cc
                       src/pin_commands.cc
                       src/time_commands.cc
                       src/env.cc
                       src/ardulator.cc
                       src/rand_num.cc
                       src/signal.cc
                       src/interrupt_commands.cc
                       src/bit_value.cc
                       src/interface.cc
                       src/emulator_commands.cc
                       src/interface.cc
                       student.cc)

