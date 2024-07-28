cmake_minimum_required(VERSION 3.27)

project(FluidDynamicsSimulation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(inc)

set(SERIAL_SOURCE_FILES
    src/main.cpp
    src/solver.cpp
)
add_executable(FluidDynamicsSimulationSerial ${SERIAL_SOURCE_FILES})

# Google Test setup
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add test subdirectory
add_subdirectory(test)
