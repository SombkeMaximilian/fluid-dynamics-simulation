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

  [[nodiscard]] size_t max_iter() const;
  [[nodiscard]] T epsilon() const;

 private:
  size_t max_iter_;
  T epsilon_;
}; // class Solver

} // namespace fluid_dynamics

#include "solver.tpp"

#endif //FLUID_DYNAMICS_SIMULATION_INC_SOLVER_H_
