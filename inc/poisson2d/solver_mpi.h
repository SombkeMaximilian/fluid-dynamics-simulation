// File: inc/poisson2d/solver_mpi.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_SOLVER_MPI_H_
#define FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_SOLVER_MPI_H_

#include <cmath>
#include <vector>
#include <functional>
#include "grid.h"
#include "bound.h"
#include "solver.h"
#include "mpi_util.h"

namespace fluid_dynamics {

template<typename T>
class SolverMpi : public Solver<T> {
 public:
  using Solver<T>::Solver;

  Grid<T> Solve(size_t rows, size_t cols, Bound<T>& global_bound, MpiGrid2D& mpi_grid);
  Grid<std::pair<T, T>> Gradient(const Grid<T>& field, MpiGrid2D& mpi_grid);
  Grid<std::pair<T, T>> Velocity(const Grid<std::pair<T, T>>& grad) override;

 private:
  Grid<T> Update(const Grid<T>& prev, Bound<T>& local_bound, MpiGrid2D& mpi_grid);
  Bound<T> LocalBoundaries(const Bound<T>& global_bound, size_t rows, size_t cols, MpiGrid2D& mpi_grid);
  bool TestBoundary(const Boundary<T>& boundary, size_t rows, size_t cols, MpiGrid2D& mpi_grid);
  void ExchangeBoundaryData(Grid<T>& grid, MpiGrid2D& mpi_grid);
}; // class SolverMpi

} // namespace fluid_dynamics

#include "solver_mpi.tpp"

#endif // FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_SOLVER_MPI_H_
