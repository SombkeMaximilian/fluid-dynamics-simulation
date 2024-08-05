// File: test/test_bound.cpp
#include <complex>
#include <vector>
#include <gtest/gtest.h>
#include "test_utils.h"
#include "poisson2d/poisson2d.h"

using BoundTypes = ::testing::Types<
    int,
    float,
    double,
    std::complex<float>,
    std::complex<double>
>;

template<typename T>
class BoundConstructor : public BoundTestBase<T> {};

TYPED_TEST_SUITE(BoundConstructor, BoundTypes);

TYPED_TEST(BoundConstructor, Default) {
  fluid_dynamics::Bound<TypeParam> bound;

  this->verifyType(bound, fluid_dynamics::BoundaryType::kDirichlet);
}

TYPED_TEST(BoundConstructor, Dirichlet) {
  fluid_dynamics::Bound<TypeParam> bound(fluid_dynamics::BoundaryType::kDirichlet);

  this->verifyType(bound, fluid_dynamics::BoundaryType::kDirichlet);
}

TYPED_TEST(BoundConstructor, Periodic) {
  fluid_dynamics::Bound<TypeParam> bound(fluid_dynamics::BoundaryType::kPeriodic);

  this->verifyType(bound, fluid_dynamics::BoundaryType::kPeriodic);
}

TYPED_TEST(BoundConstructor, DirichletWithBoundaries) {
  std::vector<fluid_dynamics::Boundary<TypeParam>> boundaries = {
      {[](size_t i, size_t j) { return i == 0; }, [](size_t i, size_t j) { return 1; }},
      {[](size_t i, size_t j) { return i == 1; }, [](size_t i, size_t j) { return 2; }},
      {[](size_t i, size_t j) { return i == 2; }, [](size_t i, size_t j) { return 3; }}
  };
  fluid_dynamics::Bound<TypeParam> bound(fluid_dynamics::BoundaryType::kDirichlet, boundaries);

  this->verifyType(bound, fluid_dynamics::BoundaryType::kDirichlet);
  EXPECT_EQ(bound.size(), boundaries.size());
}

TYPED_TEST(BoundConstructor, PeriodicWithBoundaries) {
  std::vector<fluid_dynamics::Boundary<TypeParam>> boundaries = {
      {[](size_t i, size_t j) { return i == 0; }, [](size_t i, size_t j) { return 1; }},
      {[](size_t i, size_t j) { return i == 1; }, [](size_t i, size_t j) { return 2; }},
      {[](size_t i, size_t j) { return i == 2; }, [](size_t i, size_t j) { return 3; }}
  };
  fluid_dynamics::Bound<TypeParam> bound(fluid_dynamics::BoundaryType::kPeriodic, boundaries);

  this->verifyType(bound, fluid_dynamics::BoundaryType::kPeriodic);
  EXPECT_EQ(bound.size(), boundaries.size());
}

TYPED_TEST(BoundConstructor, DirichletWithBoundariesMove) {
  std::vector<fluid_dynamics::Boundary<TypeParam>> boundaries = {
      {[](size_t i, size_t j) { return i == 0; }, [](size_t i, size_t j) { return 1; }},
      {[](size_t i, size_t j) { return i == 1; }, [](size_t i, size_t j) { return 2; }},
      {[](size_t i, size_t j) { return i == 2; }, [](size_t i, size_t j) { return 3; }}
  };
  std::vector<fluid_dynamics::Boundary<TypeParam>> boundaries_copy = boundaries;
  fluid_dynamics::Bound<TypeParam> bound(fluid_dynamics::BoundaryType::kDirichlet, std::move(boundaries));

  this->verifyType(bound, fluid_dynamics::BoundaryType::kDirichlet);
  EXPECT_EQ(bound.size(), boundaries_copy.size());
  EXPECT_TRUE(boundaries.empty());
}

TYPED_TEST(BoundConstructor, PeriodicWithBoundariesMove) {
  std::vector<fluid_dynamics::Boundary<TypeParam>> boundaries = {
      {[](size_t i, size_t j) { return i == 0; }, [](size_t i, size_t j) { return 1; }},
      {[](size_t i, size_t j) { return i == 1; }, [](size_t i, size_t j) { return 2; }},
      {[](size_t i, size_t j) { return i == 2; }, [](size_t i, size_t j) { return 3; }}
  };
  std::vector<fluid_dynamics::Boundary<TypeParam>> boundaries_copy = boundaries;
  fluid_dynamics::Bound<TypeParam> bound(fluid_dynamics::BoundaryType::kPeriodic, std::move(boundaries));

  this->verifyType(bound, fluid_dynamics::BoundaryType::kPeriodic);
  EXPECT_EQ(bound.size(), boundaries_copy.size());
  EXPECT_TRUE(boundaries.empty());
}

template<typename T>
class BoundPublicMethod : public BoundTestBase<T> {};

TYPED_TEST_SUITE(BoundPublicMethod, BoundTypes);

TYPED_TEST(BoundPublicMethod, MutateType) {
  fluid_dynamics::Bound<TypeParam> bound;

  this->verifyType(bound, fluid_dynamics::BoundaryType::kDirichlet);
  bound.type(fluid_dynamics::BoundaryType::kPeriodic);
  this->verifyType(bound, fluid_dynamics::BoundaryType::kPeriodic);
}

TYPED_TEST(BoundPublicMethod, Boundaries) {
  fluid_dynamics::Grid<TypeParam> grid(4, 4), expected(4, 4, {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0, 0});
  std::vector<fluid_dynamics::Boundary<TypeParam>> boundaries = {
      {[](size_t i, size_t j) { return i == 0; }, [](size_t i, size_t j) { return 1; }},
      {[](size_t i, size_t j) { return i == 1; }, [](size_t i, size_t j) { return 2; }},
      {[](size_t i, size_t j) { return i == 2; }, [](size_t i, size_t j) { return 3; }}
  };
  fluid_dynamics::Bound<TypeParam> bound;

  EXPECT_EQ(bound.size(), 0);
  bound.AddBoundary(boundaries[0]);
  EXPECT_EQ(bound.size(), 1);
  bound.AddBoundary(boundaries[1]);
  EXPECT_EQ(bound.size(), 2);
  bound.AddBoundary(boundaries[2]);
  EXPECT_EQ(bound.size(), 3);

  grid.Fill(static_cast<TypeParam>(0));
  for (size_t i = 0; i < grid.rows(); ++i) {
    for (size_t j = 0; j < grid.cols(); ++j) {
      for (const auto& boundary : boundaries) {
        if (boundary.condition(i, j)) {
          grid(i, j) = boundary.value(i, j);
          break;
        }
      }
    }
  }
  this->verifyData(grid, expected);
}
