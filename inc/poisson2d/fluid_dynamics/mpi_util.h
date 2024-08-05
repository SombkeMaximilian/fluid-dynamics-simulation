// File: inc/poisson2d/fluid_dynamics/mpi_util.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_FLUID_DYNAMICS_MPI_UTIL_H_
#define FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_FLUID_DYNAMICS_MPI_UTIL_H_

#include <complex>
#include <utility>
#include <type_traits>
#include <mpi.h>
#include "grid.h"

namespace fluid_dynamics {

class MpiGrid2D {
 public:
  MpiGrid2D();
  explicit MpiGrid2D(MPI_Comm comm);
  MpiGrid2D(int argc, char** argv);
  MpiGrid2D(int argc, char** argv, MPI_Comm comm);
  MpiGrid2D(const MpiGrid2D&) = delete;
  MpiGrid2D(MpiGrid2D&&) noexcept = delete;
  ~MpiGrid2D();

  MpiGrid2D& operator=(const MpiGrid2D&) = delete;
  MpiGrid2D& operator=(MpiGrid2D&&) noexcept = delete;

  [[nodiscard]] MPI_Comm comm() const;
  [[nodiscard]] int initialized() const;
  [[nodiscard]] int finalized() const;
  [[nodiscard]] int size() const;
  [[nodiscard]] int rank() const;
  [[nodiscard]] int left() const;
  [[nodiscard]] int right() const;
  [[nodiscard]] int top() const;
  [[nodiscard]] int bot() const;
  [[nodiscard]] int row() const;
  [[nodiscard]] int col() const;
  [[nodiscard]] int rows() const;
  [[nodiscard]] int cols() const;
  [[nodiscard]] const int* dims() const;
  [[nodiscard]] const int* periods() const;
  [[nodiscard]] const int* coords() const;
  [[nodiscard]] MPI_Datatype row_type() const;
  [[nodiscard]] MPI_Datatype col_type() const;

  void row_type(MPI_Datatype row_type);
  void col_type(MPI_Datatype col_type);

  void CreateRowType(size_t cols, MPI_Datatype type);
  void CreateColType(size_t rows, size_t cols_offset, MPI_Datatype type);
  void CreateTypes(size_t rows, size_t cols, size_t cols_offset, MPI_Datatype type);

  void FreeRowType();
  void FreeColType();
  void FreeTypes();

  [[nodiscard]] size_t GlobalRow(size_t i, size_t data_rows) const;
  [[nodiscard]] size_t GlobalCol(size_t j, size_t data_cols) const;

  [[nodiscard]] size_t LocalRows(size_t global_rows) const;
  [[nodiscard]] size_t LocalCols(size_t global_cols) const;

 private:
  MPI_Comm comm_;
  int initialized_;
  int finalized_;
  int size_;
  int rank_;
  int neighbors_[4];
  int dims_[2];
  int periods_[2];
  int coords_[2];
  MPI_Datatype row_type_;
  MPI_Datatype col_type_;
}; // class MpiGrid2D

template<typename T> void WriteGridBinary(Grid<T>& grid, const std::string& filename, MpiGrid2D& mpi_grid);
template<typename T> void WriteGridBinary(Grid<std::pair<T, T>>& grid, const std::string& filename, MpiGrid2D& mpi_grid);

template<typename T> static inline MPI_Datatype GetMpiType();

} // namespace fluid_dynamics

#include "mpi_util.tpp"

#endif // FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_FLUID_DYNAMICS_MPI_UTIL_H_
