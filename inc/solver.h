// File: inc/solver.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_SOLVER_H_
#define FLUID_DYNAMICS_SIMULATION_INC_SOLVER_H_

#include <cmath>
#include <vector>
#include <functional>
#include "grid.h"
#include "bound.h"

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
  void norm(std:: function<T(const Grid<T>&, const Grid<T>&)> norm);
  void source(std::function<T(size_t, size_t)> source);

 private:
  T epsilon_;
  size_t max_iter_;
  std::function<T(const Grid<T>&, const Grid<T>&)> norm_;
  std::function<T(size_t, size_t)> source_;

  static constexpr T kDefaultEpsilon = static_cast<T>(1e-6);
  static constexpr size_t kDefaultMaxIter = 100;

  static T DefaultNorm(const Grid<T>& prev, const Grid<T>& curr);
  static T DefaultSource(size_t, size_t);
}; // class Solver

} // namespace fluid_dynamics

#include "solver.tpp"

#endif //FLUID_DYNAMICS_SIMULATION_INC_SOLVER_H_
