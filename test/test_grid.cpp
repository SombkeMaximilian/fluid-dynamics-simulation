// File: test/test_grid.cpp
#include <complex>
#include <vector>
#include <gtest/gtest.h>
#include "test_utils.h"
#include "poisson2d/grid.h"

using GridTypes = ::testing::Types<
    int,
    float,
    double,
    std::complex<float>,
    std::complex<double>
>;

template<typename T>
class GridConstructorTest : public ::testing::Test {
 protected:
  void verifyDimensions(const fluid_dynamics::Grid<T>& grid, size_t expected_rows, size_t expected_cols) {
    EXPECT_EQ(grid.rows(), expected_rows);
    EXPECT_EQ(grid.cols(), expected_cols);
  }

  void verifyData(const fluid_dynamics::Grid<T>& actual, const std::vector<T>& expected) {
    for (size_t i = 0; i < actual.rows(); ++i) {
      for (size_t j = 0; j < actual.cols(); ++j) {
          EXPECT_TYPE_EQ(actual(i, j), expected[i * actual.cols() + j]);
      }
    }
  }
};

TYPED_TEST_SUITE(GridConstructorTest, GridTypes);

TYPED_TEST(GridConstructorTest, Default) {
  fluid_dynamics::Grid<TypeParam> grid;
  this->verifyDimensions(grid, 0, 0);
}

TYPED_TEST(GridConstructorTest, Square) {
  size_t dim = 10;
  fluid_dynamics::Grid<TypeParam> grid(dim);

  this->verifyDimensions(grid, dim, dim);
}

TYPED_TEST(GridConstructorTest, SquareWithData) {
  size_t dim = 10;
  std::vector<TypeParam> data(dim * dim, 1);
  fluid_dynamics::Grid<TypeParam> grid(dim, data);

  this->verifyDimensions(grid, dim, dim);
  this->verifyData(grid, data);
}

TYPED_TEST(GridConstructorTest, SquareWithDataMove) {
  size_t dim = 10;
  std::vector<TypeParam> data(dim * dim, 1);
  std::vector<TypeParam> data_copy = data;
  fluid_dynamics::Grid<TypeParam> grid(dim, std::move(data));

  this->verifyDimensions(grid, dim, dim);
  this->verifyData(grid, data_copy);
  EXPECT_TRUE(data.empty());
}

TYPED_TEST(GridConstructorTest, Rectangle) {
  size_t rows = 5;
  size_t cols = 10;
  fluid_dynamics::Grid<TypeParam> grid(rows, cols);

  this->verifyDimensions(grid, rows, cols);
}

TYPED_TEST(GridConstructorTest, RectangleWithData) {
  size_t rows = 5;
  size_t cols = 10;
  std::vector<TypeParam> data(rows * cols, 1);
  fluid_dynamics::Grid<TypeParam> grid(rows, cols, data);

  this->verifyDimensions(grid, rows, cols);
  this->verifyData(grid, data);
}

TYPED_TEST(GridConstructorTest, RectangleWithDataMove) {
  size_t rows = 5;
  size_t cols = 10;
  std::vector<TypeParam> data(rows * cols, 1);
  std::vector<TypeParam> data_copy = data;
  fluid_dynamics::Grid<TypeParam> grid(rows, cols, std::move(data));

  this->verifyDimensions(grid, rows, cols);
  this->verifyData(grid, data_copy);
  EXPECT_TRUE(data.empty());
}
