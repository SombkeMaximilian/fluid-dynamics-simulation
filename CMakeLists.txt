cmake_minimum_required(VERSION 3.27)

project(FluidDynamicsSimulation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(inc)

add_executable(FluidDynamicsSimulation src/main.cpp)
