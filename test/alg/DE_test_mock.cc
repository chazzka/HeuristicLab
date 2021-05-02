#include "gtest/gtest.h"
#include "test/mock/utils_mock.cc"            //every method returns 1.
#include "test/mock/cec_20_test_func_mock.cc" //first dejong mock.
#include "src/alg/DE.cpp"


TEST(run, isCorrectNumberOfFes)
{
    Algorithm de(1, 10, 10, 1, 1, 1, BOUNDARY_LOW, BOUNDARY_UP);
    std::vector<result> res = de.run(10, 1, BOUNDARY_LOW, BOUNDARY_UP);
    EXPECT_EQ(res.size(), 10);
}
