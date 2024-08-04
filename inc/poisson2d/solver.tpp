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
T Solver<T>::norm(const Grid<T>& prev, const Grid<T>& curr, bool exclude_boundaries) {
  return norm_(prev, curr, exclude_boundaries);
}

template<typename T>
T Solver<T>::source(size_t i, size_t j) {
  return source_(i, j);
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
void Solver<T>::norm(std::function<T(const Grid<T>&, const Grid<T>&, bool)> norm) {
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
Grid<std::pair<T, T>> Solver<T>::Velocity(const Grid<std::pair<T, T>>& grad) {
  Grid<std::pair<T, T>> velocity{grad.rows(), grad.cols()};

  for (size_t i = 0; i < grad.rows(); ++i) {
    for (size_t j = 0; j < grad.cols(); ++j) {
      velocity(i, j).first = grad(i, j).second;
      if (grad(i, j).first != static_cast<T>(0)) {
        velocity(i, j).second = -grad(i, j).first;
      } else {
        velocity(i, j).second = static_cast<T>(0);
      }
    }
  }

  return velocity;
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
      for (Boundary<T> boundary : bound.boundaries()) {
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

} // namespace fluid_dynamics
