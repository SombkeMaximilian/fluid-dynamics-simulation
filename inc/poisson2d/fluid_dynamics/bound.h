// File: inc/poisson2d/bound.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_BOUND_H_
#define FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_BOUND_H_

#include <vector>
#include <functional>
#include "grid.h"

namespace fluid_dynamics {

enum class BoundaryType {
  kDirichlet,
  kPeriodic
}; // enum class BoundaryType

template<typename T>
struct Boundary {
  std::function<bool(size_t, size_t)> condition;
  std::function<T(size_t, size_t)> value;
}; // struct Boundary

template<typename T>
class Bound {
 public:
  Bound();
  Bound(const Bound&) = default;
  Bound(Bound&&) noexcept = default;
  explicit Bound(BoundaryType type);
  Bound(BoundaryType type, const std::vector<Boundary<T>>& boundaries);
  Bound(BoundaryType type, std::vector<Boundary<T>>&& boundaries);
  ~Bound() = default;

  Bound& operator=(const Bound&) = default;
  Bound& operator=(Bound&&) noexcept = default;

  [[nodiscard]] BoundaryType type() const;
  [[nodiscard]] const std::vector<Boundary<T>>& boundaries() const;
  [[nodiscard]] std::vector<Boundary<T>>& boundaries();
  [[nodiscard]] size_t size() const;

  void type(BoundaryType type);
  void AddBoundary(const Boundary<T>& boundary);
  void AddBoundary(Boundary<T>&& boundary);

 private:
  BoundaryType type_;
  std::vector<Boundary<T>> boundaries_;
}; // class Bound

} // namespace fluid_dynamics

#include "bound.tpp"

#endif // FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_BOUND_H_
