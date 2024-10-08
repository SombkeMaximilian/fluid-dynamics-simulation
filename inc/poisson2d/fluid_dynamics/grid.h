// File: inc/poisson2d/fluid_dynamics/grid.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_GRID_H_
#define FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_GRID_H_

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
  Grid(size_t rows, size_t cols);
  Grid(size_t rows, size_t cols, const std::vector<T>& data);
  Grid(size_t rows, size_t cols, std::vector<T>&& data);
  ~Grid() = default;

  Grid& operator=(const Grid&) = default;
  Grid& operator=(Grid&&) noexcept = default;

  [[nodiscard]] T* data();
  [[nodiscard]] T* data(size_t i, size_t j);

  [[nodiscard]] size_t rows() const;
  [[nodiscard]] size_t cols() const;

  T& operator()(size_t i, size_t j);
  const T& operator()(size_t i, size_t j) const;

  void Resize(size_t rows, size_t cols, std::pair<int, int> offset);
  void Resize(size_t rows, size_t cols);

  void Fill(T value);
  void Fill(const std::vector<T>& values);
  void Fill(std::vector<T>&& values);
  void Fill(std::function<T(size_t, size_t)> value_func);

 private:
  std::vector<T> data_;
  size_t rows_;
  size_t cols_;
}; // class Grid

} // namespace fluid_dynamics

#include "grid.tpp"

#endif // FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_GRID_H_
