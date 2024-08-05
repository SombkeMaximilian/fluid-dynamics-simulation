# 2D Fluid Dynamics Simulation

A 2D fluid dynamics simulation library implemented in C++ using MPI and OpenMP. 
Poisson's equation is solved over a two-dimensional grid using the Jacobi iteration method.
Contains a serial implementation, and a hybrid implementation using MPI and OpenMP.

## Dependencies
- MPI 3.1 (other versions might work)
- OpenMP 5.1 (other versions might work)

## Usage

The headers in `inc/poisson2d/fluid_dynamics` contain the following classes:
- `Grid`: A 2D grid class that stores the data
- `Bound`: A class that stores boundary conditions as std::function objects
- `Solver`: A class that that solves Poisson's equation using the Jacobi iteration method
- `SolverMpi` : Extends Solver to solve the problem in parallel using MPI and OpenMP
- `MpiGrid2D` : Abstraction layer for MPI communication on a Cartesian grid

To use the library, include the appropriate header file:
- `poisson2d.h` : Serial implementation contains `Grid`, `Bound` and `Solver` classes
- `poisson2d_mpi.h` : MPI implementation additionally contains `SolverMpi` and `MpiGrid2D` classes

## Building the examples

To build the example programs, run the following commands:
```bash
mkdir build
cmake -S . -B build
make FDSimSerial FDSimMPI -C build --no-print-directory
```
which will create the following executables:
- `FDSimSerial`: Serial implementation
- `FDSimMPI`: MPI implementation with OpenMP directives

## Running the examples

### Serial example

The serial executable `FDSimSerial` can be run with the following command:
```bash
build/FDSimSerial -L 102 -epsilon 1e-2 -max_iter 1000
```
where the arguments are:
- `-L` : The size of the grid
- `-epsilon` : The convergence criterion
- `-max_iter` : The maximum number of iterations

### MPI example

The parallelized executable `FDSimMPI` can be run with the following command:
```bash
mpirun -np 4 build/FDSimMPI -L 102 -epsilon 1e-2 -max_iter 5000
```
where the arguments are the same as the serial example, with the additional `-np` flag to specify the number of MPI processes.
