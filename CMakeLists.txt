cmake_minimum_required(VERSION 3.25)

project(FluidDynamicsSimulation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(inc)

set(SERIAL_SOURCE_FILES
    examples/serial/main.cpp
)
add_executable(FDSimSerial ${SERIAL_SOURCE_FILES})

set(MPI_SOURCE_FILES
    examples/mpi/main.cpp
)
add_executable(FDSimMPI ${MPI_SOURCE_FILES})
target_link_libraries(FDSimMPI MPI::MPI_CXX OpenMP::OpenMP_CXX)

add_subdirectory(test)
