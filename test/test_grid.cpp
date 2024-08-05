// File: test/test_grid.cpp
#include <complex>
#include <vector>
#include <gtest/gtest.h>
#include "test_utils.h"
#include "poisson2d/poisson2d.h"

using GridTypes = ::testing::Types<
    int,
    float,
    double,
    std::complex<float>,
    std::complex<double>
>;

template<typename T>
class GridConstructor : public GridTestBase<T> {};

TYPED_TEST_SUITE(GridConstructor, GridTypes);

TYPED_TEST(GridConstructor, Default) {
  fluid_dynamics::Grid<TypeParam> grid;

  this->verifyDimensions(grid, 0, 0);
}

TYPED_TEST(GridConstructor, Square) {
  size_t dim = 10;
  fluid_dynamics::Grid<TypeParam> grid(dim);

  this->verifyDimensions(grid, dim, dim);
}

TYPED_TEST(GridConstructor, SquareWithData) {
  size_t dim = 10;
  std::vector<TypeParam> data(dim * dim, 1);
  fluid_dynamics::Grid<TypeParam> grid(dim, data);

  this->verifyDimensions(grid, dim, dim);
  this->verifyData(grid, data);
}

TYPED_TEST(GridConstructor, SquareWithDataMove) {
  size_t dim = 10;
  std::vector<TypeParam> data(dim * dim, 1);
  std::vector<TypeParam> data_copy = data;
  fluid_dynamics::Grid<TypeParam> grid(dim, std::move(data));

  this->verifyDimensions(grid, dim, dim);
  this->verifyData(grid, data_copy);
  EXPECT_TRUE(data.empty());
}

TYPED_TEST(GridConstructor, Rectangle) {
  size_t rows = 5;
  size_t cols = 10;
  fluid_dynamics::Grid<TypeParam> grid(rows, cols);

  this->verifyDimensions(grid, rows, cols);
}

TYPED_TEST(GridConstructor, RectangleWithData) {
  size_t rows = 5;
  size_t cols = 10;
  std::vector<TypeParam> data(rows * cols, 1);
  fluid_dynamics::Grid<TypeParam> grid(rows, cols, data);

  this->verifyDimensions(grid, rows, cols);
  this->verifyData(grid, data);
}

TYPED_TEST(GridConstructor, RectangleWithDataMove) {
  size_t rows = 5;
  size_t cols = 10;
  std::vector<TypeParam> data(rows * cols, 1);
  std::vector<TypeParam> data_copy = data;
  fluid_dynamics::Grid<TypeParam> grid(rows, cols, std::move(data));

  this->verifyDimensions(grid, rows, cols);
  this->verifyData(grid, data_copy);
  EXPECT_TRUE(data.empty());
}

template<typename T>
class GridPublicMethod : public GridTestBase<T> {};

TYPED_TEST_SUITE(GridPublicMethod, GridTypes);

TYPED_TEST(GridPublicMethod, DataPointer) {
  size_t rows = 5;
  size_t cols = 10;
  std::vector<TypeParam> data(rows * cols);
  fluid_dynamics::Grid<TypeParam> grid(rows, cols);

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      *grid.data(i, j) = static_cast<TypeParam>(i * cols + j);
      data[i * cols + j] = static_cast<TypeParam>(i * cols + j);
    }
  }

  this->verifyData(grid, data);
}

TYPED_TEST(GridPublicMethod, ResizeDefault) {
  size_t rows = 10;
  size_t cols = 10;
  size_t new_rows = 5;
  size_t new_cols = 5;
  std::vector<TypeParam> data(new_rows * new_cols, 1);
  fluid_dynamics::Grid<TypeParam> grid(rows, cols);

  grid.Fill(static_cast<TypeParam>(1));
  grid.Resize(new_rows, new_cols);

  this->verifyDimensions(grid, new_rows, new_cols);
  this->verifyData(grid, data);
}

TYPED_TEST(GridPublicMethod, ResizeWithOffset) {
  size_t rows = 10;
  size_t cols = 10;
  size_t new_rows = 5;
  size_t new_cols = 5;
  std::vector<TypeParam> data(new_rows * new_cols, 1);
  fluid_dynamics::Grid<TypeParam> grid(rows, cols);

  for (size_t i = 0; i < new_rows; ++i) {
    for (size_t j = 0; j < new_cols; ++j) {
      if (i < 2 || j < 2) {
        data[i * new_cols + j] = static_cast<TypeParam>(0);
      }
    }
  }

  grid.Fill(static_cast<TypeParam>(1));
  grid.Resize(new_rows, new_cols, {2, 2});

  this->verifyDimensions(grid, new_rows, new_cols);
  this->verifyData(grid, data);
}

TYPED_TEST(GridPublicMethod, FillValue) {
  size_t rows = 5;
  size_t cols = 10;
  auto value = static_cast<TypeParam>(1);
  std::vector<TypeParam> data(rows * cols, value);
  fluid_dynamics::Grid<TypeParam> grid(rows, cols);

  grid.Fill(value);

  this->verifyData(grid, data);
}

TYPED_TEST(GridPublicMethod, FillValues) {
  size_t rows = 5;
  size_t cols = 2;
  std::vector<TypeParam> values{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  fluid_dynamics::Grid<TypeParam> grid(rows, cols);

  grid.Fill(values);

  this->verifyData(grid, values);
}

TYPED_TEST(GridPublicMethod, FillValuesMove) {
  size_t rows = 5;
  size_t cols = 2;
  std::vector<TypeParam> values{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::vector<TypeParam> values_copy = values;
  fluid_dynamics::Grid<TypeParam> grid(rows, cols);

  grid.Fill(std::move(values));

  this->verifyData(grid, values_copy);
  EXPECT_TRUE(values.empty());
}
