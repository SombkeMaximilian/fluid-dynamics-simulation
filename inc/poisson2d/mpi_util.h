// File: inc/poisson2d/mpi_util.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_MPI_UTIL_H_
#define FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_MPI_UTIL_H_

#include <complex>
#include <type_traits>
#include <mpi.h>
#include "bound.h"

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
  [[nodiscard]] int rows() const;
  [[nodiscard]] int cols() const;
  [[nodiscard]] const int* dims() const;
  [[nodiscard]] const int* periods() const;
  [[nodiscard]] const int* coords() const;

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
}; // class MpiGrid2D

template<typename T> static inline MPI_Datatype GetMpiType();

} // namespace fluid_dynamics

#include "mpi_util.tpp"

#endif // FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_MPI_UTIL_H_
