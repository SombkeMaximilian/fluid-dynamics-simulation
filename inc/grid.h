// File: inc/grid.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_GRID_H_
#define FLUID_DYNAMICS_SIMULATION_INC_GRID_H_

#include <vector>

namespace fluid_dynamics {

template<typename T>
class Grid {
 public:
  Grid();
  Grid(const Grid&) = default;
  Grid(Grid&&) noexcept = default;
  explicit Grid(size_t dim);
  Grid(size_t dim, const std::vector<T>& data);
  Grid(size_t dim, std::vector<T>&& data);
  Grid(size_t width, size_t height);
  Grid(size_t width, size_t height, const std::vector<T>& data);
  Grid(size_t width, size_t height, std::vector<T>&& data);
  ~Grid() = default;

  Grid& operator=(const Grid&) = default;
  Grid& operator=(Grid&&) noexcept = default;

  [[nodiscard]] size_t width() const;
  [[nodiscard]] size_t height() const;

  T& operator()(size_t x, size_t y);
  const T& operator()(size_t x, size_t y) const;

 private:
  std::vector<T> data_;
  size_t width_;
  size_t height_;
}; // class Grid

} // namespace fluid_dynamics

#include "grid.tpp"

#endif //FLUID_DYNAMICS_SIMULATION_INC_GRID_H_
