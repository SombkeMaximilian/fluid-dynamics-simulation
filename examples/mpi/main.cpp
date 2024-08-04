#include "mpipoisson2d.h"

fluid_dynamics::Bound<double> CreateBound(size_t L) {
  size_t L_half = L / 2;
  auto L_quarter = static_cast<size_t>(std::ceil(L / 4));
  auto three_L_quarter = static_cast<size_t>(std::floor(3 * L / 4));
  fluid_dynamics::Bound<double> bound(fluid_dynamics::BoundaryType::kDirichlet);

  // y = L/2, L/4 <= x <= 3L/4, phi(x, y) = 0
  bound.AddBoundary({
                        [L_half, L_quarter, three_L_quarter](size_t i, size_t j) -> bool {
                          return i == L_half && j >= L_quarter && j <= three_L_quarter;
                        },
                        [](size_t i, size_t j) -> double {
                          return 0.0;
                        }
                    });
  // x = L/2, L/4 <= y <= 3L/4, phi(x, y) = 0
  bound.AddBoundary({
                        [L_half, L_quarter, three_L_quarter](size_t i, size_t j) -> bool {
                          return j == L_half && i >= L_quarter && i <= three_L_quarter;
                        },
                        [](size_t i, size_t j) -> double {
                          return 0.0;
                        }
                    });
  // y = 0, 0 <= x <= L/4, phi(x, y) = 14
  bound.AddBoundary({
                        [L_quarter](size_t i, size_t j) -> bool {
                          return i == 0 && j <= L_quarter;
                        },
                        [](size_t i, size_t j) -> double {
                          return 14.0;
                        }
                    });
  // y = 0, L/4 < x < L/2, phi(x, y) = 14 - (x - L/4) * 56/L
  bound.AddBoundary({
                        [L_half, L_quarter](size_t i, size_t j) -> bool {
                          return i == 0 && j > L_quarter && j < L_half;
                        },
                        [L, L_quarter](size_t i, size_t j) -> double {
                          return 14.0 - (static_cast<double>(j) - static_cast<double>(L_quarter)) * 56.0 / static_cast<double>(L);
                        }
                    });
  // y = 0, L/2 <= x < L, phi(x, y) = 0
  bound.AddBoundary({
                        [L_half](size_t i, size_t j) -> bool {
                          return i == 0 && j >= L_half;
                        },
                        [](size_t i, size_t j) -> double {
                          return 0.0;
                        }
                    });
  // y = L - 1, phi(x, y) = 7
  bound.AddBoundary({
                        [L](size_t i, size_t j) -> bool {
                          return i == L - 1;
                        },
                        [](size_t i, size_t j) -> double {
                          return 7.0;
                        }
                    });
  // x = 0, 0 <= y <= L/2, phi(x, y) = 14
  bound.AddBoundary({
                        [L_half](size_t i, size_t j) -> bool {
                          return j == 0 && i <= L_half;
                        },
                        [](size_t i, size_t j) -> double {
                          return 14.0;
                        }
                    });
  // x = 0, L/2 < y < 3L/4, phi(x, y) = 7 + (3L/4 - y) * 28/L
  bound.AddBoundary({
                        [L_half, three_L_quarter](size_t i, size_t j) -> bool {
                          return j == 0 && i > L_half && i < three_L_quarter;
                        },
                        [L, three_L_quarter](size_t i, size_t j) -> double {
                          return 7.0 + (static_cast<double>(three_L_quarter) - static_cast<double>(i)) * 28.0 / static_cast<double>(L);
                        }
                    });
  // x = 0, 3L/4 <= y < L, phi(x, y) = 7
  bound.AddBoundary({
                        [three_L_quarter](size_t i, size_t j) -> bool {
                          return j == 0 && i >= three_L_quarter;
                        },
                        [](size_t i, size_t j) -> double {
                          return 7.0;
                        }
                    });
  // x = L - 1, 0 <= y <= L/2, phi(x, y) = 0
  bound.AddBoundary({
                        [L, L_half](size_t i, size_t j) -> bool {
                          return j == L - 1 && i <= L_half;
                        },
                        [](size_t i, size_t j) -> double {
                          return 0.0;
                        }
                    });
  // x = L - 1, L/2 < y < 3L/4, phi(x, y) = (y - L/2) * 28/L
  bound.AddBoundary({
                        [L, L_half, three_L_quarter](size_t i, size_t j) -> bool {
                          return j == L - 1 && i > L_half && i < three_L_quarter;
                        },
                        [L, L_half](size_t i, size_t j) -> double {
                          return (static_cast<double>(i) - static_cast<double>(L_half)) * 28.0 / static_cast<double>(L);
                        }
                    });
  // x = L - 1, 3L/4 <= y < L, phi(x, y) = 7
  bound.AddBoundary({
                        [L, three_L_quarter](size_t i, size_t j) -> bool {
                          return j == L - 1 && i >= three_L_quarter;
                        },
                        [](size_t i, size_t j) -> double {
                          return 7.0;
                        }
                    });

  return bound;
}

int main() {
  fluid_dynamics::MpiGrid2D mpi_grid(MPI_COMM_WORLD);
  size_t L = 102;
  size_t local_rows = mpi_grid.LocalRows(L);
  size_t local_cols = mpi_grid.LocalCols(L);
  fluid_dynamics::Grid<double> grid(local_rows, local_cols);
  fluid_dynamics::Grid<std::pair<double, double>> grad;
  fluid_dynamics::Bound<double> bound = CreateBound(L);
  fluid_dynamics::SolverMpi<double> solver(1e-6, 10000);

  grid = solver.Solve(local_rows, local_cols, bound, mpi_grid);
  grad = solver.Gradient(grid, mpi_grid);

  fluid_dynamics::Grid<std::pair<double, double>> velocities(grad.rows(), grad.cols());

  for (size_t i = 0; i < local_rows; ++i) {
    for (size_t j = 0; j < local_cols; ++j) {
      velocities(i, j).first = grad(i, j).second;
      if (grad(i, j).first != 0.0) {
        velocities(i, j).second = -grad(i, j).first;
      } else {
        velocities(i, j).second = 0;
      }
    }
  }

  WriteGridBinary(velocities, "vec.bin", mpi_grid);

  return 0;
}
