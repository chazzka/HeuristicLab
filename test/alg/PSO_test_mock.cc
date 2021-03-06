#include "gtest/gtest.h"
#include "test/mock/utils_mock.cc"            //every method returns 1.
#include "test/mock/cec_20_test_func_mock.cc" //first dejong mock.
#include "src/alg/PSO.cpp"

TEST(dimensionMove, movedCorrectly)
{
    Algorithm pso(1, 1, 1, 1, 1, 1, 1, 1, 1, BOUNDARY_LOW, BOUNDARY_UP);
    Particle p = {{1}, {1}, {1}};
    Particle gp = {{2}, {2}};
    pso.dimensionMove(p, gp, 0);
    ASSERT_EQ(p.positionXi[0], 3);
}

TEST(dimensionMove, isBackInDimension)
{
    Algorithm pso(1, 1, 1, 1, 1, 1, 1, 1, 1, BOUNDARY_LOW, BOUNDARY_UP);
    Particle p = {{1}, {10000}, {10000}};
    Particle gp = {{20000}, {0}};
    pso.dimensionMove(p, gp, 0);

    ASSERT_LT(p.positionXi[0], 5.12);
}

TEST(run, isCorrectNumberOfFes)
{
    Algorithm pso(1, 1, 1, 1, 1, 1, 1, 1, 1, BOUNDARY_LOW, BOUNDARY_UP);
    std::vector<result> res = pso.run(10, 1, 1, 1);
    EXPECT_EQ(res.size(), 10);
}
