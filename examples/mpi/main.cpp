#include <iostream>
#include "poisson2d/poisson2d_mpi.h"
#include "../parse_args.h"

fluid_dynamics::Bound<double> CreateBound(size_t L) {
  size_t L_half = L / 2;
  auto L_quarter = static_cast<size_t>(std::ceil(L / 4));
  auto L_three_quarter = static_cast<size_t>(std::floor(3 * L / 4));
  auto L_three_eighth = static_cast<size_t>(std::floor(3 * L / 8));
  auto L_five_eighth = static_cast<size_t>(std::floor(5 * L / 8));
  fluid_dynamics::Bound<double> bound(fluid_dynamics::BoundaryType::kDirichlet);

  // y = L/2, L/4 <= x <= 3L/4, phi(x, y) = 0
  bound.AddBoundary({
                        [L_half, L_quarter, L_three_quarter](size_t i, size_t j) -> bool {
                          return i == L_half && j >= L_quarter && j <= L_three_quarter;
                        },
                        [](size_t i, size_t j) -> double {
                          return 0.0;
                        }
                    });
  // x = L/2, L/4 <= y <= 3L/4, phi(x, y) = 0
  bound.AddBoundary({
                        [L_half, L_quarter, L_three_quarter](size_t i, size_t j) -> bool {
                          return j == L_half && i >= L_quarter && i <= L_three_quarter;
                        },
                        [](size_t i, size_t j) -> double {
                          return 0.0;
                        }
                    });
  // y = 0, 0 <= x <= 3L/8, phi(x, y) = 14
  bound.AddBoundary({
                        [L_three_eighth](size_t i, size_t j) -> bool {
                          return i == 0 && j <= L_three_eighth;
                        },
                        [](size_t i, size_t j) -> double {
                          return 14.0;
                        }
                    });
  // y = 0, 3L/8 < x < 5L/8, phi(x, y) = 14 - (x - L/4) * 56/L
  bound.AddBoundary({
                        [L_five_eighth, L_three_eighth](size_t i, size_t j) -> bool {
                          return i == 0 && j > L_three_eighth && j < L_five_eighth;
                        },
                        [L, L_three_eighth](size_t i, size_t j) -> double {
                          return 14.0 - (static_cast<double>(j) - static_cast<double>(L_three_eighth)) * 56.0 / static_cast<double>(L);
                        }
                    });
  // y = 0, 5L/8 <= x < L, phi(x, y) = 0
  bound.AddBoundary({
                        [L_five_eighth](size_t i, size_t j) -> bool {
                          return i == 0 && j >= L_five_eighth;
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
                        [L_half, L_three_quarter](size_t i, size_t j) -> bool {
                          return j == 0 && i > L_half && i < L_three_quarter;
                        },
                        [L, L_three_quarter](size_t i, size_t j) -> double {
                          return 7.0 + (static_cast<double>(L_three_quarter) - static_cast<double>(i)) * 28.0 / static_cast<double>(L);
                        }
                    });
  // x = 0, 3L/4 <= y < L, phi(x, y) = 7
  bound.AddBoundary({
                        [L_three_quarter](size_t i, size_t j) -> bool {
                          return j == 0 && i >= L_three_quarter;
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
                        [L, L_half, L_three_quarter](size_t i, size_t j) -> bool {
                          return j == L - 1 && i > L_half && i < L_three_quarter;
                        },
                        [L, L_half](size_t i, size_t j) -> double {
                          return (static_cast<double>(i) - static_cast<double>(L_half)) * 28.0 / static_cast<double>(L);
                        }
                    });
  // x = L - 1, 3L/4 <= y < L, phi(x, y) = 7
  bound.AddBoundary({
                        [L, L_three_quarter](size_t i, size_t j) -> bool {
                          return j == L - 1 && i >= L_three_quarter;
                        },
                        [](size_t i, size_t j) -> double {
                          return 7.0;
                        }
                    });

  return bound;
}

int main(int argc, char** argv) {
  fluid_dynamics::MpiGrid2D mpi_grid(MPI_COMM_WORLD);
  size_t L;
  double epsilon;
  size_t max_iter;

  if (mpi_grid.rank() == 0) {
    parse_args::ParseArgs(argc, argv, L, epsilon, max_iter);
  }
  MPI_Bcast(&L, 1, MPI_UNSIGNED_LONG, 0, mpi_grid.comm());
  MPI_Bcast(&epsilon, 1, MPI_DOUBLE, 0, mpi_grid.comm());
  MPI_Bcast(&max_iter, 1, MPI_UNSIGNED_LONG, 0, mpi_grid.comm());
  if (mpi_grid.rank() == 0) {
    std::cout << "Running with L = " << L << ", epsilon = " << epsilon << ", max_iter = " << max_iter << std::endl;
  }
  MPI_Barrier(mpi_grid.comm());

  size_t local_rows = mpi_grid.LocalRows(L);
  size_t local_cols = mpi_grid.LocalCols(L);
  fluid_dynamics::Grid<double> grid(local_rows, local_cols);
  fluid_dynamics::Grid<std::pair<double, double>> grad(local_rows, local_cols);
  fluid_dynamics::Grid<std::pair<double, double>> velocities(local_rows, local_cols);
  fluid_dynamics::Bound<double> bound = CreateBound(L);
  fluid_dynamics::SolverMpi<double> solver(epsilon, max_iter);
  int epsilon_precision = 0;

  if (mpi_grid.rank() == 0) {
    while (epsilon < 1) {
      epsilon *= 10;
      ++epsilon_precision;
    }
    std::cout.setf(std::ios_base::fixed);
    std::cout.precision(epsilon_precision);
  }

  if (mpi_grid.rank() == 0) {
    std::cout << "Computing the stream function values on the grid.." << std::endl;
  }
  grid = solver.Solve(local_rows, local_cols, bound, mpi_grid, true);

  if (mpi_grid.rank() == 0) {
    std::cout << "Computing the gradient of the stream function.." << std::endl;
  }
  grad = solver.Gradient(grid, mpi_grid);

  if (mpi_grid.rank() == 0) {
    std::cout << "Computing the flow velocities.." << std::endl;
  }
  velocities = solver.Velocity(grad);

  if (mpi_grid.rank() == 0) {
    std::cout << "Writing the flow velocity values to file.." << std::endl;
  }
  WriteGridBinary(velocities, "plot/velocity.bin", mpi_grid);

  return 0;
}
