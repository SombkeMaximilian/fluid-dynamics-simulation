// File: inc/poisson2d/solver.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_SOLVER_H_
#define FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_SOLVER_H_

#include <cmath>
#include <vector>
#include <functional>
#include "grid.h"
#include "bound.h"
#include "mpi_util.h"

namespace fluid_dynamics {

template<typename T>
class Solver {
 public:
  Solver();
  Solver(const Solver&) = default;
  Solver(Solver&&) noexcept = default;
  explicit Solver(T epsilon);
  explicit Solver(size_t max_iter);
  Solver(T epsilon, size_t max_iter);
  ~Solver() = default;

  Solver& operator=(const Solver&) = default;
  Solver& operator=(Solver&&) noexcept = default;

  [[nodiscard]] T epsilon() const;
  [[nodiscard]] size_t max_iter() const;

  void epsilon(T epsilon);
  void max_iter(size_t max_iter);
  void norm(std::function<T(const Grid<T>&, const Grid<T>&)> norm);
  void source(std::function<T(size_t, size_t)> source);

  Grid<T> Solve(size_t rows, size_t cols, const Bound<T>& bound);
  Grid<T> SolveMpi(size_t rows, size_t cols, Bound<T>& global_bound, MpiGrid2D& mpi_grid);
  Grid<std::pair<T, T>> Gradient(const Grid<T>& field);
  Grid<std::pair<T, T>> GradientMpi(const Grid<T>& field, MpiGrid2D& mpi_grid);

 private:
  T epsilon_;
  size_t max_iter_;
  std::function<T(const Grid<T>&, const Grid<T>&, bool)> norm_;
  std::function<T(size_t, size_t)> source_;

  static constexpr T kDefaultEpsilon = static_cast<T>(1e-6);
  static constexpr size_t kDefaultMaxIter = 100;

  static T DefaultNorm(const Grid<T>& prev, const Grid<T>& curr, bool exclude_boundaries = false);
  static T DefaultSource(size_t, size_t);

  Grid<T> Update(const Grid<T>& prev, const Bound<T>& bound);
  Grid<T> UpdateMpi(const Grid<T>& prev, Bound<T>& local_bound, MpiGrid2D& mpi_grid);

  Bound<T> LocalBoundaries(const Bound<T>& global_bound, size_t rows, size_t cols, MpiGrid2D& mpi_grid);
  bool TestBoundary(const Boundary<T>& boundary, size_t rows, size_t cols, MpiGrid2D& mpi_grid);

  void ExchangeBoundaryData(Grid<T>& grid, MpiGrid2D& mpi_grid);
}; // class Solver

} // namespace fluid_dynamics

#include "solver.tpp"

#endif // FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_SOLVER_H_
