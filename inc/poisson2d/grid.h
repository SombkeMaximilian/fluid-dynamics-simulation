// File: inc/grid.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_GRID_H_
#define FLUID_DYNAMICS_SIMULATION_INC_GRID_H_

#include <fstream>
#include <utility>
#include <vector>
#include <functional>

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

  T& operator()(size_t i, size_t j);
  const T& operator()(size_t i, size_t j) const;

  void Resize(size_t width, size_t height, std::pair<size_t, size_t> offset);
  void Resize(size_t width, size_t height);

  void Fill(T value);
  void Fill(const std::vector<T>& values);
  void Fill(std::vector<T>&& values);
  void Fill(std::function<T(size_t, size_t)> value_func);

 private:
  std::vector<T> data_;
  size_t width_;
  size_t height_;
}; // class Grid

} // namespace fluid_dynamics

#include "grid.tpp"
#include "grid_io_serial.tpp"

#endif // FLUID_DYNAMICS_SIMULATION_INC_GRID_H_
