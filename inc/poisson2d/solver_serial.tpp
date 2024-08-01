// File: inc/solver_serial.tpp
namespace fluid_dynamics {

template<typename T>
Grid<T> Solver<T>::Solve(const Grid<T>& initial, const Bound<T>& bound) {
  Grid<T> prev{initial};
  Grid<T> curr{initial.rows(), initial.cols()};

  for (size_t i = 0; i < initial.cols(); ++i) {
    for (size_t j = 0; j < initial.rows(); ++j) {
      prev(i, j) = source_(i, j);
    }
  }
  for (size_t iter = 0; iter < max_iter_; ++iter) {
    curr = Update(prev, bound);
    if (norm_(prev, curr) < epsilon_) {
      break;
    }
    prev = curr;
  }

  return curr;
}

template<typename T>
Grid<std::pair<T, T>> Solver<T>::Gradient(const Grid<T>& field) {
  Grid<std::pair<T, T>> grad{field.rows(), field.cols()};

  for (size_t i = 1; i < field.rows() - 1; ++i) {
    for (size_t j = 1; j < field.cols() - 1; ++j) {
      grad(i, j).first = (field(i, j + 1) - field(i, j - 1)) / 2;
      grad(i, j).second = (field(i + 1, j) - field(i - 1,j)) / 2;
    }
  }

  return grad;
}

template<typename T>
Grid<T> Solver<T>::Update(const Grid<T>& prev, const Bound<T>& bound) {
  Grid<T> next{prev.rows(), prev.cols()};
  bool is_boundary = false;

  for (size_t i = 0; i < prev.rows(); ++i) {
    for (size_t j = 0; j < prev.cols(); ++j) {
      for (auto boundary : bound.boundaries()) {
        if (boundary.condition(i, j)) {
          next(i, j) = boundary.value(i, j);
          is_boundary = true;
          break;
        }
      }
      if (!is_boundary) {
        next(i, j) = (prev(i - 1, j) + prev(i + 1, j) + prev(i, j - 1) + prev(i, j + 1) + source_(i, j)) / 4;
      }
      is_boundary = false;
    }
  }

  return next;
}

} // namespace fluid_dynamics
