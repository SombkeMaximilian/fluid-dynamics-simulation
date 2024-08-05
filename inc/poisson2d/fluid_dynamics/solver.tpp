// File: inc/poisson2d/fluid_dynamics/solver.tpp
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
Grid<T> Solver<T>::Solve(size_t rows, size_t cols, const Bound<T>& bound, bool verbose) {
  Grid<T> prev{rows, cols};
  Grid<T> curr{rows, cols};
  T norm;
  size_t iter;
  bool converged = false;
  int progress_intervals = static_cast<int>(max_iter_ * 0.05);
  int progress_steps = 0;

  for (size_t i = 0; i < prev.rows(); ++i) {
    for (size_t j = 0; j < prev.cols(); ++j) {
      prev(i, j) = source_(i, j);
    }
  }

  auto start = std::chrono::high_resolution_clock::now();

  for (iter = 0; iter < max_iter_; ++iter) {
    curr = Update(prev, bound);
    norm = norm_(prev, curr, false);
    if (norm < epsilon_) {
      converged = true;
      break;
    }
    prev = curr;

    if (verbose && iter == progress_steps * progress_intervals) {
      Progress(iter, max_iter_);
      ++progress_steps;
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<long double> time_taken = end - start;

  if (verbose) {
    Progress(max_iter_, max_iter_);
    if (converged) {
      std::cout << "Number of iterations to converge: " << iter << std::endl;
    } else {
      std::cout << "Reached maximum number of iterations: " << max_iter_ << std::endl;
      std::cout << "Norm: " << norm << std::endl;
    }
    std::cout << std::setprecision(3) << "Time Taken: " << time_taken.count() << "s\n" << std::endl;
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
void Solver<T>::Progress(size_t iter, size_t max_iter) {
  double progress = static_cast<double>(iter) / static_cast<double>(max_iter);
  int total_width = 50;
  int curr_width = static_cast<int>(total_width * progress);

  std::cout << "[";
  for (int i = 0; i < total_width; ++i) {
    if (i < curr_width) {
      std::cout << "=";
    } else if (i == curr_width) {
      std::cout << ">";
    } else {
      std::cout << " ";
    }
  }
  std::cout << "] " << static_cast<int>(progress * 100.0) << "%\r";
  std::cout << std::endl;
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
