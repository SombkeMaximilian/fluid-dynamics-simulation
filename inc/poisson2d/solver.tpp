// File: inc/poisson2d/solver.tpp
namespace fluid_dynamics {

template<typename T>
Solver<T>::Solver() : epsilon_{kDefaultEpsilon}, max_iter_{kDefaultMaxIter}, norm_{DefaultNorm}, source_{DefaultSource} {}

template<typename T>
Solver<T>::Solver(T epsilon) : epsilon_{epsilon}, max_iter_{kDefaultMaxIter}, norm_{DefaultNorm}, source_{DefaultSource} {}

template<typename T>
Solver<T>::Solver(size_t max_iter) : epsilon_{kDefaultEpsilon}, max_iter_{max_iter}, norm_{DefaultNorm}, source_{DefaultSource} {}

template<typename T>
Solver<T>::Solver(T epsilon, size_t max_iter) : epsilon_{epsilon}, max_iter_{max_iter}, norm_{DefaultNorm}, source_{DefaultSource} {}

template<typename T>
T Solver<T>::epsilon() const {
  return epsilon_;
}

template<typename T>
size_t Solver<T>::max_iter() const {
  return max_iter_;
}

template<typename T>
void Solver<T>::epsilon(T epsilon) {
  epsilon_ = epsilon;
}

template<typename T>
void Solver<T>::max_iter(size_t max_iter) {
  max_iter_ = max_iter;
}

template<typename T>
void Solver<T>::norm(std::function<T(const Grid<T>&, const Grid<T>&)> norm) {
  norm_ = norm;
}

template<typename T>
void Solver<T>::source(std::function<T(size_t, size_t)> source) {
  source_ = source;
}

template<typename T>
Grid<T> Solver<T>::Solve(size_t rows, size_t cols, const Bound<T>& bound) {
  Grid<T> prev{rows, cols};
  Grid<T> curr{rows, cols};

  for (size_t i = 0; i < prev.rows(); ++i) {
    for (size_t j = 0; j < prev.cols(); ++j) {
      prev(i, j) = source_(i, j);
    }
  }
  for (size_t iter = 0; iter < max_iter_; ++iter) {
    curr = Update(prev, bound);
    if (norm_(prev, curr, false) < epsilon_) {
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
      grad(i, j).second = (field(i + 1, j) - field(i - 1, j)) / 2;
    }
  }

  return grad;
}

template<typename T>
T Solver<T>::DefaultNorm(const Grid<T>& prev, const Grid<T>& curr, bool exclude_boundaries) {
  T norm = 0;
  size_t start_row = exclude_boundaries ? 1 : 0;
  size_t start_col = exclude_boundaries ? 1 : 0;
  size_t end_row = exclude_boundaries ? prev.rows() - 1 : prev.rows();
  size_t end_col = exclude_boundaries ? prev.cols() - 1 : prev.cols();

  for (size_t i = start_row; i < end_row; ++i) {
    for (size_t j = start_col; j < end_col; ++j) {
      norm += (prev(i, j) - curr(i, j)) * (prev(i, j) - curr(i, j));
    }
  }

  return sqrt(norm);
}

template<typename T>
T Solver<T>::DefaultSource(size_t, size_t) {
  return 0;
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
        next(i, j) = 0.25 * (prev(i - 1, j) + prev(i + 1, j) + prev(i, j - 1) + prev(i, j + 1) + source_(i, j));
      }
      is_boundary = false;
    }
  }

  return next;
}

template<typename T>
Bound<T> Solver<T>::LocalBoundaries(const Bound<T>& global_bound, size_t rows, size_t cols, MpiGrid2D& mpi_grid) {
  Bound<T> local_bound(global_bound.type());

  for (auto &b : global_bound.boundaries()) {
    if (TestBoundary(b, rows, cols, mpi_grid)) {
      local_bound.AddBoundary(b);
    }
  }

  return local_bound;
}

template<typename T>
bool Solver<T>::TestBoundary(const Boundary<T>& boundary, size_t rows, size_t cols, MpiGrid2D& mpi_grid) {
  size_t origin_row = mpi_grid.GlobalRow(0, rows);
  size_t origin_col = mpi_grid.GlobalCol(0, cols);

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (boundary.condition(origin_row + i, origin_col + j)) {
        return true;
      }
    }
  }
  return false;
}

} // namespace fluid_dynamics
