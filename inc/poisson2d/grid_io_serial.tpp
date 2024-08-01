// File: inc/io_serial.h
namespace fluid_dynamics {

template<typename T>
void WriteGridBinary(const Grid<T>& grid, const std::string& filename) {
  std::ofstream file(filename, std::ios::binary);

  if (!file.is_open()) {
      throw std::runtime_error("Failed to open file: " + filename);
  }

  for (size_t i = 0; i < grid.cols(); ++i) {
    for (size_t j = 0; j < grid.rows(); ++j) {
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

  for (size_t i = 0; i < grid.cols(); ++i) {
    for (size_t j = 0; j < grid.rows(); ++j) {
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

  for (size_t i = 0; i < grid.cols(); ++i) {
    for (size_t j = 0; j < grid.rows(); ++j) {
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

  for (size_t i = 0; i < grid.cols(); ++i) {
    for (size_t j = 0; j < grid.rows(); ++j) {
      file << j << " " << i << " " << grid(i, j).first << " " << grid(i, j).second << "\n";
    }
  }
  file.close();
}

} // namespace fluid_dynamics
