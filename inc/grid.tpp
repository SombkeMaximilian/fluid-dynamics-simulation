// File: inc/grid.tpp
namespace fluid_dynamics {

template<typename T>
Grid<T>::Grid() : data_{}, width_{0}, height_{0} {}

template<typename T>
Grid<T>::Grid(size_t dim) : data_(dim * dim), width_{dim}, height_{dim} {}

template<typename T>
Grid<T>::Grid(size_t dim, const std::vector<T>& data)
    : data_{data}, width_{dim}, height_{dim} {}

template<typename T>
Grid<T>::Grid(size_t dim, std::vector<T>&& data)
    : data_{std::move(data)}, width_{dim}, height_{dim} {}

template<typename T>
Grid<T>::Grid(size_t width, size_t height)
    : data_(width * height), width_{width}, height_{height} {}

template<typename T>
Grid<T>::Grid(size_t width, size_t height, const std::vector<T>& data)
    : data_{data}, width_{width}, height_{height} {}

template<typename T>
Grid<T>::Grid(size_t width, size_t height, std::vector<T>&& data)
    : data_{std::move(data)}, width_{width}, height_{height} {}

template<typename T>
size_t Grid<T>::width() const {
  return width_;
}

template<typename T>
size_t Grid<T>::height() const {
  return height_;
}

template<typename T>
T& Grid<T>::operator()(size_t i, size_t j) {
  return data_[i * width_ + j];
}

template<typename T>
const T& Grid<T>::operator()(size_t i, size_t j) const {
  return data_[i * width_ + j];
}

template<typename T>
void Grid<T>::Resize(size_t width, size_t height, std::pair<size_t, size_t> offset) {
  std::vector<T> new_data(width * height);

  for (size_t i = 0; i < width; ++i) {
    for (size_t j = 0; j < height; ++j) {
      if (i < offset.first || i >= offset.first + width_
          || j < offset.second || j >= offset.second + height_) {
        new_data[i * width + j] = T{};
      } else {
        new_data[i * width + j] = data_[(i - offset.first) * width_ + j - offset.second];
      }
    }
  }
  data_ = std::move(new_data);
  width_ = width;
  height_ = height;
}

template<typename T>
void Grid<T>::Resize(size_t width, size_t height) {
  this->Resize(width, height, {0, 0});
}

template<typename T>
void Grid<T>::Fill(T value) {
  std::fill(data_.begin(), data_.end(), value);
}

template<typename T>
void Grid<T>::Fill(const std::vector<T>& values) {
  data_ = values;
}

template<typename T>
void Grid<T>::Fill(std::vector<T>&& values) {
  data_ = std::move(values);
}

template<typename T>
void Grid<T>::Fill(std::function<T(size_t, size_t)> value_func) {
  for (size_t i = 0; i < width_; ++i) {
    for (size_t j = 0; j < height_; ++j) {
      data_[i * width_ + j] = value_func(i, j);
    }
  }
}

} // namespace fluid_dynamics
