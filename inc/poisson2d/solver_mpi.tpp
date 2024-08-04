// File: inc/poisson2d/solver_mpi.h
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
Grid<T> Solver<T>::SolveMpi(size_t rows, size_t cols, Bound<T>& global_bound, MpiGrid2D& mpi_grid) {
  Grid<T> prev{rows, cols};
  Grid<T> curr{rows, cols};
  Bound<T> local_bound{LocalBoundaries(global_bound, rows, cols, mpi_grid)};
  size_t origin_row = mpi_grid.GlobalRow(0, prev.rows());
  size_t origin_col = mpi_grid.GlobalCol(0, prev.cols());
  T local_norm, global_norm;

  for (size_t i = 0; i < prev.rows(); ++i) {
    for (size_t j = 0; j < prev.cols(); ++j) {
      prev(i, j) = source_(origin_row + i, origin_col + j);
    }
  }

  mpi_grid.CreateRowType(prev.cols(), GetMpiType<T>());
  mpi_grid.CreateColType(prev.rows(), prev.cols() + 2, GetMpiType<T>());
  prev.Resize(prev.rows() + 2, prev.cols() + 2, {1, 1});
  curr.Resize(curr.rows() + 2, curr.cols() + 2, {1, 1});

  for (size_t iter = 0; iter < max_iter_; ++iter) {
    ExchangeBoundaryData(prev, mpi_grid);
    curr = UpdateMpi(prev, local_bound, mpi_grid);

    local_norm = norm_(prev, curr, true);
    MPI_Allreduce(&local_norm, &global_norm, 1, GetMpiType<T>(), MPI_SUM, mpi_grid.comm());
    if (global_norm < epsilon_) {
      break;
    }

    prev = curr;
  }

  curr.Resize(curr.rows() - 2, curr.cols() - 2, {-1, -1});

  mpi_grid.FreeTypes();

  return curr;
}

template<typename T>
Grid<std::pair<T, T>> Solver<T>::GradientMpi(const Grid<T>& field, MpiGrid2D& mpi_grid) {
  Grid<T> expanded_field{field};
  Grid<std::pair<T, T>> grad{field.rows(), field.cols()};
  size_t origin_row = mpi_grid.GlobalRow(0, field.rows());
  size_t origin_col = mpi_grid.GlobalCol(0, field.cols());
  size_t start_row = (origin_row == 0) ? 1 : 0;
  size_t start_col = (origin_col == 0) ? 1 : 0;
  size_t end_row = (origin_row == field.rows() * (mpi_grid.dims()[1] - 1)) ? field.rows() - 1 : field.rows();
  size_t end_col = (origin_col == field.cols() * (mpi_grid.dims()[0] - 1)) ? field.cols() - 1 : field.cols();

  mpi_grid.CreateRowType(field.cols(), GetMpiType<T>());
  mpi_grid.CreateColType(field.rows(), field.cols() + 2, GetMpiType<T>());
  expanded_field.Resize(expanded_field.rows() + 2, expanded_field.cols() + 2, {1, 1});

  ExchangeBoundaryData(expanded_field, mpi_grid);

  for (size_t i = start_row; i < end_row; ++i) {
    for (size_t j = start_col; j < end_col; ++j) {
      grad(i, j).first = (expanded_field(i + 1, j + 2) - expanded_field(i + 1, j)) / 2;
      grad(i, j).second = (expanded_field(i + 2, j + 1) - expanded_field(i, j + 1)) / 2;
    }
  }

  mpi_grid.FreeTypes();

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
Grid<T> Solver<T>::UpdateMpi(const Grid<T>& prev, Bound<T>& local_bound, MpiGrid2D& mpi_grid) {
  Grid<T> next{prev.rows(), prev.cols()};
  size_t origin_row = mpi_grid.GlobalRow(0, prev.rows() - 2);
  size_t origin_col = mpi_grid.GlobalCol(0, prev.cols() - 2);
  bool is_boundary = false;

  for (size_t i = 1; i < prev.rows() - 1; ++i) {
    for (size_t j = 1; j < prev.cols() - 1; ++j) {
      for (auto boundary : local_bound.boundaries()) {
        if (boundary.condition(origin_row + i - 1, origin_col + j - 1)) {
          next(i, j) = boundary.value(origin_row + i - 1, origin_col + j - 1);
          is_boundary = true;
          break;
        }
      }
      if (!is_boundary) {
        next(i, j) = 0.25 * (prev(i - 1, j) + prev(i + 1, j) + prev(i, j - 1) + prev(i, j + 1)
            + source_(origin_row + i - 1, origin_col + j - 1));
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

template<typename T>
void Solver<T>::ExchangeBoundaryData(Grid<T>& grid, MpiGrid2D& mpi_grid) {
  MPI_Sendrecv(grid.data(1, 1), 1, mpi_grid.row_type(), mpi_grid.top(), 0,
               grid.data(grid.rows() - 1, 1), 1, mpi_grid.row_type(), mpi_grid.bot(), 0,
               mpi_grid.comm(), MPI_STATUS_IGNORE);
  MPI_Sendrecv(grid.data(grid.rows() - 2, 1), 1, mpi_grid.row_type(), mpi_grid.bot(), 0,
               grid.data(0, 1), 1, mpi_grid.row_type(), mpi_grid.top(), 0,
               mpi_grid.comm(), MPI_STATUS_IGNORE);
  MPI_Sendrecv(grid.data(1, 1), 1, mpi_grid.col_type(), mpi_grid.left(), 1,
               grid.data(1, grid.cols() - 1), 1, mpi_grid.col_type(), mpi_grid.right(), 1,
               mpi_grid.comm(), MPI_STATUS_IGNORE);
  MPI_Sendrecv(grid.data(1, grid.cols() - 2), 1, mpi_grid.col_type(), mpi_grid.right(), 1,
               grid.data(1, 0), 1, mpi_grid.col_type(), mpi_grid.left(), 1,
               mpi_grid.comm(), MPI_STATUS_IGNORE);
}

} // namespace fluid_dynamics
