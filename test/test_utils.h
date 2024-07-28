// File: test/test_utils.h
#ifndef FLUID_DYNAMICS_SIMULATION_TEST_TEST_UTILS_H_
#define FLUID_DYNAMICS_SIMULATION_TEST_TEST_UTILS_H_

#include <complex>
#include <vector>
#include <gtest/gtest.h>
#include "grid.h"
#include "bound.h"

template<typename T>
void EXPECT_TYPE_EQ(const T& actual, const T& expected) {
  EXPECT_EQ(actual, expected);
}

template<>
void EXPECT_TYPE_EQ(const float& actual, const float& expected) {
  EXPECT_FLOAT_EQ(actual, expected);
}

template<>
void EXPECT_TYPE_EQ(const double& actual, const double& expected) {
  EXPECT_DOUBLE_EQ(actual, expected);
}

template<>
void EXPECT_TYPE_EQ(const std::complex<float>& actual, const std::complex<float>& expected) {
  EXPECT_FLOAT_EQ(actual.real(), expected.real());
  EXPECT_FLOAT_EQ(actual.imag(), expected.imag());
}

template<>
void EXPECT_TYPE_EQ(const std::complex<double>& actual, const std::complex<double>& expected) {
  EXPECT_DOUBLE_EQ(actual.real(), expected.real());
  EXPECT_DOUBLE_EQ(actual.imag(), expected.imag());
}

#endif //FLUID_DYNAMICS_SIMULATION_TEST_TEST_UTILS_H_
