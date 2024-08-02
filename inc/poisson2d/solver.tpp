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
T Solver<T>::DefaultNorm(const Grid<T>& prev, const Grid<T>& curr) {
  T norm = 0;

  for (size_t i = 0; i < prev.rows(); ++i) {
    for (size_t j = 0; j < prev.cols(); ++j) {
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
        next(i, j) = (prev(i - 1, j) + prev(i + 1, j) + prev(i, j - 1) + prev(i, j + 1) + source_(i, j)) / 4;
      }
      is_boundary = false;
    }
  }

  return next;
}

} // namespace fluid_dynamics
