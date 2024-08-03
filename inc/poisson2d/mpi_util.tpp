// File: inc/poisson2d/mpi_util.tpp

namespace fluid_dynamics {

MpiGrid2D::MpiGrid2D()
    : comm_{MPI_COMM_WORLD}, initialized_{-1}, finalized_{-1}, rank_{0}, size_{0},
      neighbors_{0, 0, 0, 0}, dims_{0, 0},
      periods_{0, 0}, coords_{0, 0},
      boundary_row_{MPI_DATATYPE_NULL}, boundary_col_{MPI_DATATYPE_NULL} {
  MPI_Initialized(&initialized_);
  if (!initialized_) {
    MPI_Init(nullptr, nullptr);
  }
  MPI_Finalized(&finalized_);
  MPI_Comm_size(comm_, &size_);
  MPI_Comm_rank(comm_, &rank_);
  MPI_Dims_create(size_, 2, dims_);
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims_, periods_, 1, &comm_);
  MPI_Cart_coords(comm_, rank_, 2, coords_);
  MPI_Cart_shift(comm_, 0, 1, &neighbors_[0], &neighbors_[1]);
  MPI_Cart_shift(comm_, 1, 1, &neighbors_[2], &neighbors_[3]);
}

MpiGrid2D::MpiGrid2D(MPI_Comm comm)
    : comm_{comm}, initialized_{-1}, finalized_{-1}, rank_{0}, size_{0},
      neighbors_{0, 0, 0, 0}, dims_{0, 0},
      periods_{0, 0}, coords_{0, 0},
      boundary_row_{MPI_DATATYPE_NULL}, boundary_col_{MPI_DATATYPE_NULL} {
  MPI_Initialized(&initialized_);
  if (!initialized_) {
    MPI_Init(nullptr, nullptr);
  }
  MPI_Finalized(&finalized_);
  MPI_Comm_size(comm_, &size_);
  MPI_Comm_rank(comm_, &rank_);
  MPI_Dims_create(size_, 2, dims_);
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims_, periods_, 1, &comm_);
  MPI_Cart_coords(comm_, rank_, 2, coords_);
  MPI_Cart_shift(comm_, 0, 1, &neighbors_[0], &neighbors_[1]);
  MPI_Cart_shift(comm_, 1, 1, &neighbors_[2], &neighbors_[3]);
}

MpiGrid2D::MpiGrid2D(int argc, char** argv)
    : comm_{MPI_COMM_WORLD}, initialized_{-1}, finalized_{-1}, rank_{0}, size_{0},
      neighbors_{0, 0, 0, 0}, dims_{0, 0},
      periods_{0, 0}, coords_{0, 0},
      boundary_row_{MPI_DATATYPE_NULL}, boundary_col_{MPI_DATATYPE_NULL} {
  MPI_Initialized(&initialized_);
  if (!initialized_) {
    MPI_Init(&argc, &argv);
  }
  MPI_Finalized(&finalized_);
  MPI_Comm_size(comm_, &size_);
  MPI_Comm_rank(comm_, &rank_);
  MPI_Dims_create(size_, 2, dims_);
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims_, periods_, 1, &comm_);
  MPI_Cart_coords(comm_, rank_, 2, coords_);
  MPI_Cart_shift(comm_, 0, 1, &neighbors_[0], &neighbors_[1]);
  MPI_Cart_shift(comm_, 1, 1, &neighbors_[2], &neighbors_[3]);
}

MpiGrid2D::MpiGrid2D(int argc, char** argv, MPI_Comm comm)
    : comm_{comm}, initialized_{-1}, finalized_{-1}, rank_{0}, size_{0},
      neighbors_{0, 0, 0, 0}, dims_{0, 0},
      periods_{0, 0}, coords_{0, 0},
      boundary_row_{MPI_DATATYPE_NULL}, boundary_col_{MPI_DATATYPE_NULL} {
  MPI_Initialized(&initialized_);
  if (!initialized_) {
    MPI_Init(&argc, &argv);
  }
  MPI_Finalized(&finalized_);
  MPI_Comm_size(comm_, &size_);
  MPI_Comm_rank(comm_, &rank_);
  MPI_Dims_create(size_, 2, dims_);
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims_, periods_, 1, &comm_);
  MPI_Cart_coords(comm_, rank_, 2, coords_);
  MPI_Cart_shift(comm_, 0, 1, &neighbors_[0], &neighbors_[1]);
  MPI_Cart_shift(comm_, 1, 1, &neighbors_[2], &neighbors_[3]);
}

MpiGrid2D::~MpiGrid2D() {
  MPI_Finalized(&finalized_);
  if (!finalized_) {
    MPI_Finalize();
  }
}

MPI_Comm MpiGrid2D::comm() const {
  return comm_;
}

int MpiGrid2D::initialized() const {
  return initialized_;
}

int MpiGrid2D::finalized() const {
  return finalized_;
}

int MpiGrid2D::size() const {
  return size_;
}

int MpiGrid2D::rank() const {
  return rank_;
}

int MpiGrid2D::left() const {
  return neighbors_[0];
}

int MpiGrid2D::right() const {
  return neighbors_[1];
}

int MpiGrid2D::top() const {
  return neighbors_[2];
}

int MpiGrid2D::bot() const {
  return neighbors_[3];
}

int MpiGrid2D::rows() const {
  return dims_[0];
}

int MpiGrid2D::cols() const {
  return dims_[1];
}

const int* MpiGrid2D::dims() const {
  return dims_;
}

const int* MpiGrid2D::periods() const {
  return periods_;
}

const int* MpiGrid2D::coords() const {
  return coords_;
}

MPI_Datatype MpiGrid2D::boundary_row() const {
  return boundary_row_;
}

MPI_Datatype MpiGrid2D::boundary_col() const {
  return boundary_col_;
}

void MpiGrid2D::boundary_row(MPI_Datatype boundary_row) {
  boundary_row_ = boundary_row;
}

void MpiGrid2D::boundary_col(MPI_Datatype boundary_col) {
  boundary_col_ = boundary_col;
}

void MpiGrid2D::CreateBoundaryRowType(size_t cols, MPI_Datatype type) {
  CreateRowType(static_cast<int>(cols), type);
}

void MpiGrid2D::CreateBoundaryColType(size_t rows, size_t cols_offset, MPI_Datatype type) {
  CreateColType(static_cast<int>(rows), static_cast<int>(cols_offset), type);
}

void MpiGrid2D::CreateBoundaryTypes(size_t rows, size_t cols, size_t cols_offset, MPI_Datatype type) {
  CreateRowType(static_cast<int>(cols), type);
  CreateColType(static_cast<int>(rows), static_cast<int>(cols_offset), type);
}

void MpiGrid2D::FreeBoundaryRowType() {
  if (boundary_row_ != MPI_DATATYPE_NULL) {
    MPI_Type_free(&boundary_row_);
    boundary_row_ = MPI_DATATYPE_NULL;
  }
}

void MpiGrid2D::FreeBoundaryColType() {
  if (boundary_col_ != MPI_DATATYPE_NULL) {
    MPI_Type_free(&boundary_col_);
    boundary_col_ = MPI_DATATYPE_NULL;
  }
}

void MpiGrid2D::FreeBoundaryTypes() {
  FreeBoundaryRowType();
  FreeBoundaryColType();
}

size_t MpiGrid2D::GlobalRow(size_t i, size_t data_cols) const {
  return coords_[1] * data_cols + i;
}

size_t MpiGrid2D::GlobalCol(size_t j, size_t data_rows) const {
  return coords_[0] * data_rows + j;
}

void MpiGrid2D::CreateRowType(int count, MPI_Datatype type) {
  MPI_Type_contiguous(count, type, &boundary_row_);
  MPI_Type_commit(&boundary_row_);
}

void MpiGrid2D::CreateColType(int count, int stride, MPI_Datatype type) {
  MPI_Type_vector(count, 1, stride, type, &boundary_col_);
  MPI_Type_commit(&boundary_col_);
}

template<typename T>
static inline MPI_Datatype GetMpiType() {
  MPI_Datatype mpi_type = MPI_DATATYPE_NULL;

  if (std::is_same<T, signed short>::value) {
    mpi_type = MPI_SHORT;
  } else if (std::is_same<T, unsigned short>::value) {
    mpi_type = MPI_UNSIGNED_SHORT;
  } else if (std::is_same<T, signed int>::value) {
    mpi_type = MPI_INT;
  } else if (std::is_same<T, unsigned int>::value) {
    mpi_type = MPI_UNSIGNED;
  } else if (std::is_same<T, signed long int>::value) {
    mpi_type = MPI_LONG;
  } else if (std::is_same<T, unsigned long int>::value) {
    mpi_type = MPI_UNSIGNED_LONG;
  } else if (std::is_same<T, signed long long int>::value) {
    mpi_type = MPI_LONG_LONG;
  } else if (std::is_same<T, unsigned long long int>::value) {
    mpi_type = MPI_UNSIGNED_LONG_LONG;
  } else if (std::is_same<T, float>::value) {
    mpi_type = MPI_FLOAT;
  } else if (std::is_same<T, double>::value) {
    mpi_type = MPI_DOUBLE;
  } else if (std::is_same<T, long double>::value) {
    mpi_type = MPI_LONG_DOUBLE;
  } else if (std::is_same<T, std::int8_t>::value) {
    mpi_type = MPI_INT8_T;
  } else if (std::is_same<T, std::int16_t>::value) {
    mpi_type = MPI_INT16_T;
  } else if (std::is_same<T, std::int32_t>::value) {
    mpi_type = MPI_INT32_T;
  } else if (std::is_same<T, std::int64_t>::value) {
    mpi_type = MPI_INT64_T;
  } else if (std::is_same<T, std::uint8_t>::value) {
    mpi_type = MPI_UINT8_T;
  } else if (std::is_same<T, std::uint16_t>::value) {
    mpi_type = MPI_UINT16_T;
  } else if (std::is_same<T, std::uint32_t>::value) {
    mpi_type = MPI_UINT32_T;
  } else if (std::is_same<T, std::uint64_t>::value) {
    mpi_type = MPI_UINT64_T;
  } else if (std::is_same<T, bool>::value) {
    mpi_type = MPI_C_BOOL;
  } else if (std::is_same<T, std::complex<float>>::value) {
    mpi_type = MPI_C_COMPLEX;
  } else if (std::is_same<T, std::complex<double>>::value) {
    mpi_type = MPI_C_DOUBLE_COMPLEX;
  } else if (std::is_same<T, std::complex<long double>>::value) {
    mpi_type = MPI_C_LONG_DOUBLE_COMPLEX;
  }

  return mpi_type;
}

} // namespace fluid_dynamics
