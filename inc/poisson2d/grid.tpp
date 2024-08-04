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
void Grid<T>::Resize(size_t rows, size_t cols, std::pair<int, int> offset) {
  std::vector<T> new_data(rows * cols);

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (static_cast<int>(i) < offset.first || static_cast<int>(i) >= offset.first + rows_
          || static_cast<int>(j) < offset.second || static_cast<int>(j) >= offset.second + cols_) {
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

template<typename T>
void WriteGridBinary(const Grid<T>& grid, const std::string& filename) {
  std::ofstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  for (size_t i = 0; i < grid.rows(); ++i) {
    for (size_t j = 0; j < grid.cols(); ++j) {
      file.write(reinterpret_cast<const char*>(&grid(i, j)), sizeof(T));
    }
  }
  file.close();
}

template<typename T>
void WriteGridBinary(const Grid<std::pair<T, T>>& grid, const std::string& filename) {
  std::ofstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  for (size_t i = 0; i < grid.rows(); ++i) {
    for (size_t j = 0; j < grid.cols(); ++j) {
      file.write(reinterpret_cast<const char*>(&grid(i, j).first), sizeof(T));
      file.write(reinterpret_cast<const char*>(&grid(i, j).second), sizeof(T));
    }
  }
  file.close();
}

template<typename T>
void WriteGridText(const Grid<T>& grid, const std::string& filename) {
  std::ofstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  for (size_t i = 0; i < grid.rows(); ++i) {
    for (size_t j = 0; j < grid.cols(); ++j) {
      file << j << " " << i << " " << grid(i, j) << " ";
    }
  }
  file.close();
}

template<typename T>
void WriteGridText(const Grid<std::pair<T, T>>& grid, const std::string& filename) {
  std::ofstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  for (size_t i = 0; i < grid.rows(); ++i) {
    for (size_t j = 0; j < grid.cols(); ++j) {
      file << j << " " << i << " " << grid(i, j).first << " " << grid(i, j).second << "\n";
    }
  }
  file.close();
}

} // namespace fluid_dynamics
