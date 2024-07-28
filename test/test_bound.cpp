// File: test/test_bound.cpp
#include <complex>
#include <vector>
#include <gtest/gtest.h>
#include "grid.h"
#include "bound.h"

using BoundTypes = ::testing::Types<
    int,
    float,
    double,
    std::complex<float>,
    std::complex<double>
>;

template<typename T>
class BoundConstructorTest : public ::testing::Test {
 protected:
  void verifyType(const fluid_dynamics::Bound<T>& bound, fluid_dynamics::BoundaryType expected_type) {
    EXPECT_EQ(bound.type(), expected_type);
  }
};

TYPED_TEST_SUITE(BoundConstructorTest, BoundTypes);

TYPED_TEST(BoundConstructorTest, Default) {
  fluid_dynamics::Bound<TypeParam> bound;
  this->verifyType(bound, fluid_dynamics::BoundaryType::kDirichlet);
}
