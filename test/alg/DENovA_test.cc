#include "gtest/gtest.h"
#include "test/mock/utils_mock.cc"            //every method returns 1.
#include "test/mock/cec_20_test_func_mock.cc" //first dejong mock.
#include "src/alg/DENovA.cpp"

TEST(run, isCorrectNumberOfFes)
{
    Algorithm deNovA(1, 1, 10, 10, 1, 1, 1, BOUNDARY_LOW, BOUNDARY_UP);
    std::vector<result> res = deNovA.run(10, 1, BOUNDARY_LOW, BOUNDARY_UP);
    EXPECT_EQ(res.size(), 10);
}
