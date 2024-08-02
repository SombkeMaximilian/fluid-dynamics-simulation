// File: inc/poisson2d/grid.tpp
namespace fluid_dynamics {

template<typename T>
Grid<T>::Grid() : data_{}, rows_{0}, cols_{0} {}

template<typename T>
Grid<T>::Grid(size_t dim) : data_(dim * dim), rows_{dim}, cols_{dim} {}

template<typename T>
Grid<T>::Grid(size_t dim, const std::vector<T>& data)
    : data_{data}, rows_{dim}, cols_{dim} {}

template<typename T>
Grid<T>::Grid(size_t dim, std::vector<T>&& data)
    : data_{std::move(data)}, rows_{dim}, cols_{dim} {}

template<typename T>
Grid<T>::Grid(size_t rows, size_t cols)
    : data_(rows * cols), rows_{rows}, cols_{cols} {}

template<typename T>
Grid<T>::Grid(size_t rows, size_t cols, const std::vector<T>& data)
    : data_{data}, rows_{rows}, cols_{cols} {}

template<typename T>
Grid<T>::Grid(size_t rows, size_t cols, std::vector<T>&& data)
    : data_{std::move(data)}, rows_{rows}, cols_{cols} {}

template<typename T>
T* Grid<T>::data() {
  return data_.data();
}

template<typename T>
T* Grid<T>::data(size_t i, size_t j) {
  return data_.data() + i * cols_ + j;
}

template<typename T>
size_t Grid<T>::rows() const {
  return rows_;
}

template<typename T>
size_t Grid<T>::cols() const {
  return cols_;
}

template<typename T>
T& Grid<T>::operator()(size_t i, size_t j) {
  return data_[i * cols_ + j];
}

template<typename T>
const T& Grid<T>::operator()(size_t i, size_t j) const {
  return data_[i * cols_ + j];
}

template<typename T>
void Grid<T>::Resize(size_t rows, size_t cols, std::pair<size_t, size_t> offset) {
  std::vector<T> new_data(rows * cols);

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (i < offset.first || i >= offset.first + cols_
          || j < offset.second || j >= offset.second + rows_) {
        new_data[i * cols + j] = T{};
      } else {
        new_data[i * cols + j] = data_[(i - offset.first) * cols_ + j - offset.second];
      }
    }
  }
  data_ = std::move(new_data);
  rows_ = rows;
  cols_ = cols;
}

template<typename T>
void Grid<T>::Resize(size_t rows, size_t cols) {
  this->Resize(rows, cols, {0, 0});
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
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      data_[i * cols_ + j] = value_func(i, j);
    }
  }
}

} // namespace fluid_dynamics
