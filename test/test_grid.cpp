// File: test/test_grid.cpp
#include <complex>
#include <vector>
#include <gtest/gtest.h>
#include "grid.h"

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
  void verifyDimensions(const fluid_dynamics::Grid<T>& grid, size_t expected_width, size_t expected_height) {
    EXPECT_EQ(grid.width(), expected_width);
    EXPECT_EQ(grid.height(), expected_height);
  }

  void verifyData(const fluid_dynamics::Grid<T>& grid, const std::vector<T>& data) {
    for (size_t i = 0; i < grid.height(); ++i) {
      for (size_t j = 0; j < grid.width(); ++j) {
        EXPECT_EQ(grid(i, j), data[i * grid.width() + j]);
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
  size_t width = 10;
  size_t height = 5;
  fluid_dynamics::Grid<TypeParam> grid(width, height);

  this->verifyDimensions(grid, width, height);
}

TYPED_TEST(GridConstructorTest, RectangleWithData) {
  size_t width = 10;
  size_t height = 5;
  std::vector<TypeParam> data(width * height, 1);
  fluid_dynamics::Grid<TypeParam> grid(width, height, data);

  this->verifyDimensions(grid, width, height);
  this->verifyData(grid, data);
}

TYPED_TEST(GridConstructorTest, RectangleWithDataMove) {
  size_t width = 10;
  size_t height = 5;
  std::vector<TypeParam> data(width * height, 1);
  std::vector<TypeParam> data_copy = data;
  fluid_dynamics::Grid<TypeParam> grid(width, height, std::move(data));

  this->verifyDimensions(grid, width, height);
  this->verifyData(grid, data_copy);
  EXPECT_TRUE(data.empty());
}
