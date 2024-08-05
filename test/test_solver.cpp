// File: test/test_solver.cpp
#include <gtest/gtest.h>
#include "test_utils.h"
#include "poisson2d/poisson2d.h"

using SolverTypes = ::testing::Types<
    float,
    double,
    long double
>;

template<typename T>
class SolverConstructor : public SolverTestBase<T> {};

TYPED_TEST_SUITE(SolverConstructor, SolverTypes);

TYPED_TEST(SolverConstructor, Default) {
  TypeParam computed_norm, expected_norm;
  fluid_dynamics::Grid<TypeParam> computed_source(10, 10), expected_source(10, 10);
  fluid_dynamics::Solver<TypeParam> solver;

  expected_source.Fill(0);

  for (size_t i = 0; i < computed_source.rows(); ++i) {
    for (size_t j = 0; j < computed_source.cols(); ++j) {
      computed_source(i, j) = solver.source(i, j);
    }
  }

  expected_norm = 0;
  computed_norm = solver.norm(expected_source, computed_source, false);

  this->verifyEpsilon(solver, this->kDefaultEpsilon);
  this->verifyMaxIter(solver, this->kDefaultMaxIter);
  this->verifyData(computed_source, expected_source);
  EXPECT_TYPE_EQ(computed_norm, expected_norm);
}

TYPED_TEST(SolverConstructor, Epsilon) {
  TypeParam computed_norm, expected_norm;
  fluid_dynamics::Grid<TypeParam> computed_source(10, 10), expected_source(10, 10);
  TypeParam epsilon = 1e-10;
  fluid_dynamics::Solver<TypeParam> solver(epsilon);

  expected_source.Fill(0);

  for (size_t i = 0; i < computed_source.rows(); ++i) {
    for (size_t j = 0; j < computed_source.cols(); ++j) {
      computed_source(i, j) = solver.source(i, j);
    }
  }

  expected_norm = 0;
  computed_norm = solver.norm(expected_source, computed_source, false);

  this->verifyEpsilon(solver, epsilon);
  this->verifyMaxIter(solver, this->kDefaultMaxIter);
  this->verifyData(computed_source, expected_source);
  EXPECT_TYPE_EQ(computed_norm, expected_norm);
}

TYPED_TEST(SolverConstructor, MaxIter) {
  TypeParam computed_norm, expected_norm;
  fluid_dynamics::Grid<TypeParam> computed_source(10, 10), expected_source(10, 10);
  size_t max_iter = 20000;
  fluid_dynamics::Solver<TypeParam> solver(max_iter);

  expected_source.Fill(0);

  for (size_t i = 0; i < computed_source.rows(); ++i) {
    for (size_t j = 0; j < computed_source.cols(); ++j) {
      computed_source(i, j) = solver.source(i, j);
    }
  }

  expected_norm = 0;
  computed_norm = solver.norm(expected_source, computed_source, false);

  this->verifyEpsilon(solver, this->kDefaultEpsilon);
  this->verifyMaxIter(solver, max_iter);
  this->verifyData(computed_source, expected_source);
  EXPECT_TYPE_EQ(computed_norm, expected_norm);
}

TYPED_TEST(SolverConstructor, EpsilonMaxIter) {
  TypeParam computed_norm, expected_norm;
  fluid_dynamics::Grid<TypeParam> computed_source(10, 10), expected_source(10, 10);
  TypeParam epsilon = 1e-10;
  size_t max_iter = 20000;
  fluid_dynamics::Solver<TypeParam> solver(epsilon, max_iter);

  expected_source.Fill(0);

  for (size_t i = 0; i < computed_source.rows(); ++i) {
    for (size_t j = 0; j < computed_source.cols(); ++j) {
      computed_source(i, j) = solver.source(i, j);
    }
  }

  expected_norm = 0;
  computed_norm = solver.norm(expected_source, computed_source, false);

  this->verifyEpsilon(solver, epsilon);
  this->verifyMaxIter(solver, max_iter);
  this->verifyData(computed_source, expected_source);
  EXPECT_TYPE_EQ(computed_norm, expected_norm);
}

template<typename T>
class SolverPublicMethod : public SolverTestBase<T> {};

TYPED_TEST_SUITE(SolverPublicMethod, SolverTypes);

TYPED_TEST(SolverPublicMethod, MutateEpsilon) {
  TypeParam epsilon = 1e-10;
  fluid_dynamics::Solver<TypeParam> solver;

  this->verifyEpsilon(solver, this->kDefaultEpsilon);
  solver.epsilon(epsilon);
  this->verifyEpsilon(solver, epsilon);
}

TYPED_TEST(SolverPublicMethod, MutateMaxIter) {
  size_t max_iter = 20000;
  fluid_dynamics::Solver<TypeParam> solver;

  this->verifyMaxIter(solver, this->kDefaultMaxIter);
  solver.max_iter(max_iter);
  this->verifyMaxIter(solver, max_iter);
}

TYPED_TEST(SolverPublicMethod, MutateNorm) {
  TypeParam computed_norm, expected_norm;
  fluid_dynamics::Grid<TypeParam> prev(10, 10), curr(10, 10);
  fluid_dynamics::Solver<TypeParam> solver;

  prev.Fill(1);
  curr.Fill(2);

  solver.norm(&this->NewNorm);

  computed_norm = solver.norm(prev, curr, false);
  expected_norm = this->NewNorm(prev, curr, false);

  EXPECT_TYPE_EQ(computed_norm, expected_norm);
}

TYPED_TEST(SolverPublicMethod, MutateSource) {
  TypeParam computed_norm, expected_norm;
  fluid_dynamics::Grid<TypeParam> computed_source(10, 10), expected_source(10, 10);
  fluid_dynamics::Solver<TypeParam> solver;

  solver.source(this->NewSource);

  for (size_t i = 0; i < computed_source.rows(); ++i) {
    for (size_t j = 0; j < computed_source.cols(); ++j) {
      computed_source(i, j) = solver.source(i, j);
      expected_source(i, j) = this->NewSource(i, j);
    }
  }

  this->verifyData(computed_source, expected_source);
}
