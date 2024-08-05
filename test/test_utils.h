// File: test/test_utils.h
#ifndef FLUID_DYNAMICS_SIMULATION_TEST_TEST_UTILS_H_
#define FLUID_DYNAMICS_SIMULATION_TEST_TEST_UTILS_H_

#include <complex>
#include <vector>
#include <gtest/gtest.h>
#include "poisson2d/poisson2d.h"

template<typename T>
inline void EXPECT_TYPE_EQ(const T& actual, const T& expected) {
  EXPECT_EQ(actual, expected);
}

template<>
inline void EXPECT_TYPE_EQ(const float& actual, const float& expected) {
  EXPECT_FLOAT_EQ(actual, expected);
}

template<>
inline void EXPECT_TYPE_EQ(const double& actual, const double& expected) {
  EXPECT_DOUBLE_EQ(actual, expected);
}

template<>
inline void EXPECT_TYPE_EQ(const std::complex<float>& actual, const std::complex<float>& expected) {
  EXPECT_FLOAT_EQ(actual.real(), expected.real());
  EXPECT_FLOAT_EQ(actual.imag(), expected.imag());
}

template<>
inline void EXPECT_TYPE_EQ(const std::complex<double>& actual, const std::complex<double>& expected) {
  EXPECT_DOUBLE_EQ(actual.real(), expected.real());
  EXPECT_DOUBLE_EQ(actual.imag(), expected.imag());
}

template<typename T>
class GridTestBase : public ::testing::Test {
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

template<typename T>
class BoundTestBase : public ::testing::Test {
 protected:
  void verifyType(const fluid_dynamics::Bound<T>& bound, fluid_dynamics::BoundaryType expected_type) {
    EXPECT_EQ(bound.type(), expected_type);
  }

  void verifyData(const fluid_dynamics::Grid<T>& grid, const fluid_dynamics::Grid<T>& expected) {
    for (size_t i = 0; i < grid.rows(); ++i) {
      for (size_t j = 0; j < grid.cols(); ++j) {
        EXPECT_TYPE_EQ(grid(i, j), expected(i, j));
      }
    }
  }
};

template<typename T>
class SolverTestBase : public ::testing::Test {
 protected:
  void verifyEpsilon(const fluid_dynamics::Solver<T>& solver, T expected_epsilon) {
    EXPECT_TYPE_EQ(solver.epsilon(), expected_epsilon);
  }

  void verifyMaxIter(const fluid_dynamics::Solver<T>& solver, size_t expected_max_iter) {
    EXPECT_EQ(solver.max_iter(), expected_max_iter);
  }

  void verifyNorm(const fluid_dynamics::Solver<T>& solver, const fluid_dynamics::Grid<T>& prev,
                  const fluid_dynamics::Grid<T>& curr, bool exclude_boundaries, T expected_norm) {
    EXPECT_TYPE_EQ(solver.norm(prev, curr, exclude_boundaries), expected_norm);
  }

  void verifySource(const fluid_dynamics::Solver<T>& solver, size_t i, size_t j, T expected_source) {
    EXPECT_TYPE_EQ(solver.source(i, j), expected_source);
  }
};

#endif // FLUID_DYNAMICS_SIMULATION_TEST_TEST_UTILS_H_
