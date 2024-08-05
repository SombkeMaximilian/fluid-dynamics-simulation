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
  static constexpr T kDefaultEpsilon = static_cast<T>(1e-6);
  static constexpr size_t kDefaultMaxIter = 100;

  void verifyEpsilon(const fluid_dynamics::Solver<T>& solver, T expected_epsilon) {
    EXPECT_TYPE_EQ(solver.epsilon(), expected_epsilon);
  }

  void verifyMaxIter(const fluid_dynamics::Solver<T>& solver, size_t expected_max_iter) {
    EXPECT_EQ(solver.max_iter(), expected_max_iter);
  }

  void verifyData(const fluid_dynamics::Grid<T>& grid, const fluid_dynamics::Grid<T>& expected) {
    for (size_t i = 0; i < grid.rows(); ++i) {
      for (size_t j = 0; j < grid.cols(); ++j) {
        EXPECT_TYPE_EQ(grid(i, j), expected(i, j));
      }
    }
  }

  static T NewNorm(const fluid_dynamics::Grid<T>& prev, const fluid_dynamics::Grid<T>& curr,
            bool exclude_boundaries = false) {
    T norm = 0;
    size_t start_i = exclude_boundaries ? 1 : 0;
    size_t start_j = exclude_boundaries ? 1 : 0;
    size_t end_i = exclude_boundaries ? prev.rows() - 1 : prev.rows();
    size_t end_j = exclude_boundaries ? prev.cols() - 1 : prev.cols();

    for (size_t i = start_i; i < end_i; ++i) {
      for (size_t j = start_j; j < end_j; ++j) {
        norm += std::abs(curr(i, j) - prev(i, j));
      }
    }

    return norm;
  }

  static T NewSource(size_t, size_t) {
    return 1;
  }
};

#endif // FLUID_DYNAMICS_SIMULATION_TEST_TEST_UTILS_H_
