// File: inc/poisson2d/fluid_dynamics/solver_mpi.h
namespace fluid_dynamics {

template<typename T>
Grid<T> SolverMpi<T>::Solve(size_t rows, size_t cols, Bound<T>& global_bound, MpiGrid2D& mpi_grid, bool verbose) {
  Grid<T> prev{rows, cols};
  Grid<T> curr{rows, cols};
  Bound<T> local_bound{LocalBoundaries(global_bound, rows, cols, mpi_grid)};
  size_t origin_row = mpi_grid.GlobalRow(0, prev.rows());
  size_t origin_col = mpi_grid.GlobalCol(0, prev.cols());
  T local_norm, global_norm;
  int progress_intervals = static_cast<int>(Solver<T>::max_iter() * 0.05);
  int progress_steps = 0;

  #pragma omp parallel for default(none) collapse(2) shared(prev, origin_row, origin_col)
  for (size_t i = 0; i < prev.rows(); ++i) {
    for (size_t j = 0; j < prev.cols(); ++j) {
      prev(i, j) = Solver<T>::source(origin_row + i, origin_col + j);
    }
  }

  mpi_grid.CreateRowType(prev.cols(), MpiType<T>());
  mpi_grid.CreateColType(prev.rows(), prev.cols() + 2, MpiType<T>());
  prev.Resize(prev.rows() + 2, prev.cols() + 2, {1, 1});
  curr.Resize(curr.rows() + 2, curr.cols() + 2, {1, 1});

  for (size_t iter = 0; iter < Solver<T>::max_iter(); ++iter) {
    ExchangeBoundaryData(prev, mpi_grid);
    curr = Update(prev, local_bound, mpi_grid);

    local_norm = Solver<T>::norm(prev, curr, true);
    MPI_Allreduce(&local_norm, &global_norm, 1, MpiType<T>(), MPI_SUM, mpi_grid.comm());
    if (global_norm < Solver<T>::epsilon()) {
      break;
    }

    prev = curr;

    if (verbose && mpi_grid.rank() == 0 && iter == progress_steps * progress_intervals) {
      Solver<T>::Progress(iter, Solver<T>::max_iter());
      ++progress_steps;
    }
  }
  if (verbose && mpi_grid.rank() == 0) {
    Solver<T>::Progress(Solver<T>::max_iter(), Solver<T>::max_iter());
  }

  curr.Resize(curr.rows() - 2, curr.cols() - 2, {-1, -1});

  mpi_grid.FreeTypes();

  return curr;
}

template<typename T>
Grid<std::pair<T, T>> SolverMpi<T>::Gradient(const Grid<T>& field, MpiGrid2D& mpi_grid) {
  Grid<T> expanded_field{field};
  Grid<std::pair<T, T>> grad{field.rows(), field.cols()};
  size_t start_row = (mpi_grid.row() == 0) ? 1 : 0;
  size_t start_col = (mpi_grid.col() == 0) ? 1 : 0;
  size_t end_row = (mpi_grid.row() == mpi_grid.rows() - 1) ? field.rows() - 1 : field.rows();
  size_t end_col = (mpi_grid.col() == mpi_grid.cols() - 1) ? field.cols() - 1 : field.cols();

  mpi_grid.CreateRowType(field.cols(), MpiType<T>());
  mpi_grid.CreateColType(field.rows(), field.cols() + 2, MpiType<T>());
  expanded_field.Resize(expanded_field.rows() + 2, expanded_field.cols() + 2, {1, 1});

  ExchangeBoundaryData(expanded_field, mpi_grid);

  #pragma omp parallel for default(none) collapse(2) \
          shared(expanded_field, grad, start_row, end_row, start_col, end_col)
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
Grid<std::pair<T, T>> SolverMpi<T>::Velocity(const Grid<std::pair<T, T>>& grad) {
  Grid<std::pair<T, T>> velocity{grad.rows(), grad.cols()};

  #pragma omp parallel for default(none) collapse(2) shared(grad, velocity)
  for (size_t i = 0; i < grad.rows(); ++i) {
    for (size_t j = 0; j < grad.cols(); ++j) {
      velocity(i, j).first = grad(i, j).second;
      if (grad(i, j).first != 0) {
        velocity(i, j).second = -grad(i, j).first;
      } else {
        velocity(i, j).second = 0;
      }
    }
  }

  return velocity;
}


template<typename T>
Grid<T> SolverMpi<T>::Update(const Grid<T>& prev, Bound<T>& local_bound, MpiGrid2D& mpi_grid) {
  Grid<T> next{prev.rows(), prev.cols()};
  size_t origin_row = mpi_grid.GlobalRow(0, prev.rows() - 2);
  size_t origin_col = mpi_grid.GlobalCol(0, prev.cols() - 2);
  bool is_boundary = false;

  for (size_t i = 1; i < prev.rows() - 1; ++i) {
    for (size_t j = 1; j < prev.cols() - 1; ++j) {
      for (Boundary<T> boundary : local_bound.boundaries()) {
        if (boundary.condition(origin_row + i - 1, origin_col + j - 1)) {
          next(i, j) = boundary.value(origin_row + i - 1, origin_col + j - 1);
          is_boundary = true;
          break;
        }
      }
      if (!is_boundary) {
        next(i, j) = 0.25 * (prev(i - 1, j) + prev(i + 1, j) + prev(i, j - 1) + prev(i, j + 1)
                             + Solver<T>::source(origin_row + i - 1, origin_col + j - 1));
      }
      is_boundary = false;
    }
  }

  return next;
}

template<typename T>
Bound<T> SolverMpi<T>::LocalBoundaries(const Bound<T>& global_bound, size_t rows, size_t cols, MpiGrid2D& mpi_grid) {
  Bound<T> local_bound(global_bound.type());

  for (Boundary<T> b : global_bound.boundaries()) {
    if (TestBoundary(b, rows, cols, mpi_grid)) {
      local_bound.AddBoundary(b);
    }
  }

  return local_bound;
}

template<typename T>
bool SolverMpi<T>::TestBoundary(const Boundary<T>& boundary, size_t rows, size_t cols, MpiGrid2D& mpi_grid) {
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
void SolverMpi<T>::ExchangeBoundaryData(Grid<T>& grid, MpiGrid2D& mpi_grid) {
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
