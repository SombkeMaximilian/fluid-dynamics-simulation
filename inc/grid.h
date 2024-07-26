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
  Grid(size_t dim);
  Grid(size_t dim, const std::vector<T>& data);
  Grid(size_t dim, std::vector<T>&& data);
  Grid(size_t width, size_t height);
  Grid(size_t width, size_t height, const std::vector<T>& data);
  Grid(size_t width, size_t height, std::vector<T>&& data);
  ~Grid() = default;

  Grid& operator=(const Grid&) = default;
  Grid& operator=(Grid&&) noexcept = default;

 private:
  std::vector<T> data_;
  size_t width_;
  size_t height_;
};
} // namespace fluid_dynamics

#endif //FLUID_DYNAMICS_SIMULATION_INC_GRID_H_
