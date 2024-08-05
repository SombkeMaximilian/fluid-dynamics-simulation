// File: inc/poisson2d/fluid_dynamics/mpi_util.tpp

namespace fluid_dynamics {

MpiGrid2D::MpiGrid2D()
    : comm_{MPI_COMM_WORLD}, initialized_{-1}, finalized_{-1}, rank_{0}, size_{0},
      neighbors_{0, 0, 0, 0}, dims_{0, 0},
      periods_{0, 0}, coords_{0, 0},
      row_type_{MPI_DATATYPE_NULL}, col_type_{MPI_DATATYPE_NULL} {
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
      row_type_{MPI_DATATYPE_NULL}, col_type_{MPI_DATATYPE_NULL} {
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
      row_type_{MPI_DATATYPE_NULL}, col_type_{MPI_DATATYPE_NULL} {
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
      row_type_{MPI_DATATYPE_NULL}, col_type_{MPI_DATATYPE_NULL} {
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

int MpiGrid2D::row() const {
  return coords_[1];
}

int MpiGrid2D::col() const {
  return coords_[0];
}

int MpiGrid2D::rows() const {
  return dims_[1];
}

int MpiGrid2D::cols() const {
  return dims_[0];
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

MPI_Datatype MpiGrid2D::row_type() const {
  return row_type_;
}

MPI_Datatype MpiGrid2D::col_type() const {
  return col_type_;
}

void MpiGrid2D::row_type(MPI_Datatype row_type) {
  row_type_ = row_type;
}

void MpiGrid2D::col_type(MPI_Datatype col_type) {
  col_type_ = col_type;
}

void MpiGrid2D::CreateRowType(size_t cols, MPI_Datatype type) {
  MPI_Type_contiguous(static_cast<int>(cols), type, &row_type_);
  MPI_Type_commit(&row_type_);
}

void MpiGrid2D::CreateColType(size_t rows, size_t cols_offset, MPI_Datatype type) {
  MPI_Type_vector(static_cast<int>(rows), 1, static_cast<int>(cols_offset),
                  type, &col_type_);
  MPI_Type_commit(&col_type_);
}

void MpiGrid2D::CreateTypes(size_t rows, size_t cols, size_t cols_offset, MPI_Datatype type) {
  CreateRowType(cols, type);
  CreateColType(rows, cols_offset, type);
}

void MpiGrid2D::FreeRowType() {
  if (row_type_ != MPI_DATATYPE_NULL) {
    MPI_Type_free(&row_type_);
    row_type_ = MPI_DATATYPE_NULL;
  }
}

void MpiGrid2D::FreeColType() {
  if (col_type_ != MPI_DATATYPE_NULL) {
    MPI_Type_free(&col_type_);
    col_type_ = MPI_DATATYPE_NULL;
  }
}

void MpiGrid2D::FreeTypes() {
  FreeRowType();
  FreeColType();
}

size_t MpiGrid2D::GlobalRow(size_t i, size_t data_rows) const {
  return row() * data_rows + i;
}

size_t MpiGrid2D::GlobalCol(size_t j, size_t data_cols) const {
  return col() * data_cols + j;
}

size_t MpiGrid2D::LocalRows(size_t global_rows) const {
  if (global_rows % rows() == 0) {
    return global_rows / rows();
  } else {
    throw std::runtime_error("Global rows is not divisible by the number of rows");
  }
}

size_t MpiGrid2D::LocalCols(size_t global_cols) const {
  if (global_cols % cols() == 0) {
    return global_cols / cols();
  } else {
    throw std::runtime_error("Global cols is not divisible by the number of cols");
  }
}

template<typename T>
void WriteGridBinary(Grid<T>& grid, const std::string& filename, MpiGrid2D& mpi_grid) {
  MPI_File file;
  MPI_Offset file_size = mpi_grid.rows() * grid.rows() * mpi_grid.cols() * grid.cols() * sizeof(T);
  MPI_Offset row_size = grid.cols() * sizeof(T);
  MPI_Offset row_offset = 0;
  int mode = MPI_MODE_CREATE | MPI_MODE_WRONLY;

  mpi_grid.CreateRowType(grid.cols(), GetMpiType<T>());
  MPI_File_open(mpi_grid.comm(), filename.c_str(), mode, MPI_INFO_NULL, &file);
  MPI_File_set_size(file, file_size);

  for (size_t i = 0; i < grid.rows(); ++i) {
    row_offset = (i + mpi_grid.row() * grid.rows()) * mpi_grid.cols() + mpi_grid.col();
    MPI_File_write_at(file, row_offset * row_size, grid.data(i, 0), 1,
                      mpi_grid.row_type(), MPI_STATUS_IGNORE);
  }

  MPI_File_close(&file);
  mpi_grid.FreeRowType();
}

template<typename T>
void WriteGridBinary(Grid<std::pair<T, T>>& grid, const std::string& filename, MpiGrid2D& mpi_grid) {
  Grid<T> unpaired_grid{grid.rows(), 2 * grid.cols()};

  #pragma omp parallel for default(none) collapse(2) shared(grid, unpaired_grid)
  for (size_t i = 0; i < grid.rows(); ++i) {
    for (size_t j = 0; j < grid.cols(); ++j) {
      unpaired_grid(i, 2 * j) = grid(i, j).first;
      unpaired_grid(i, 2 * j + 1) = grid(i, j).second;
    }
  }

  WriteGridBinary(unpaired_grid, filename, mpi_grid);
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
