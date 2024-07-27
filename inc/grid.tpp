// File: inc/grid.tpp
namespace fluid_dynamics {

template<typename T>
Grid<T>::Grid() : data_(), width_(0), height_(0) {}

template<typename T>
Grid<T>::Grid(size_t dim) : data_(dim * dim), width_(dim), height_(dim) {}

template<typename T>
Grid<T>::Grid(size_t dim, const std::vector<T>& data)
    : data_(data), width_(dim), height_(dim) {}

template<typename T>
Grid<T>::Grid(size_t dim, std::vector<T>&& data)
    : data_(std::move(data)), width_(dim), height_(dim) {}

template<typename T>
Grid<T>::Grid(size_t width, size_t height)
    : data_(width * height), width_(width), height_(height) {}

template<typename T>
Grid<T>::Grid(size_t width, size_t height, const std::vector<T>& data)
    : data_(data), width_(width), height_(height) {}

template<typename T>
Grid<T>::Grid(size_t width, size_t height, std::vector<T>&& data)
    : data_(std::move(data)), width_(width), height_(height) {}

template<typename T>
size_t Grid<T>::width() const {
  return width_;
}

template<typename T>
size_t Grid<T>::height() const {
  return height_;
}

template<typename T>
T& Grid<T>::operator()(size_t x, size_t y) {
  return data_[y * width_ + x];
}

template<typename T>
const T& Grid<T>::operator()(size_t x, size_t y) const {
  return data_[y * width_ + x];
}

} // namespace fluid_dynamics
