// File: inc/solver.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_SOLVER_H_
#define FLUID_DYNAMICS_SIMULATION_INC_SOLVER_H_

#include <vector>
#include "grid.h"
#include "bound.h"

namespace fluid_dynamics {

template<typename T>
class Solver {
 public:
  Solver();
  Solver(const Solver&) = default;
  Solver(Solver&&) noexcept = default;
  ~Solver() = default;

  Solver& operator=(const Solver&) = default;
  Solver& operator=(Solver&&) noexcept = default;

  [[nodiscard]] const Grid<T>& oldGrid() const;
  [[nodiscard]] const Grid<T>& newGrid() const;
  [[nodiscard]] const Bound<T>& boundaryConditions() const;
  [[nodiscard]] size_t max_iter() const;
  [[nodiscard]] T epsilon() const;

 private:
  Grid<T> u_;
  Grid<T> u_new_;
  Bound<T> u_bound_;
  size_t max_iter_;
  T epsilon_;
}; // class Solver

} // namespace fluid_dynamics

#include "solver_serial.tpp"

#endif //FLUID_DYNAMICS_SIMULATION_INC_SOLVER_H_
