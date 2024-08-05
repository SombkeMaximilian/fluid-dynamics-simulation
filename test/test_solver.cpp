// File: test/test_solver.cpp
#include <gtest/gtest.h>
#include "test_utils.h"
#include "poisson2d/poisson2d.h"

using SolverTypes = ::testing::Types<
    float,
    double,
    long double
>;
