#include "gtest/gtest.h"
#include "src/utils/utils.cpp"

TEST(getEuclideanDistance, checkDistance)
{
    std::vector<double> first = {1};
    std::vector<double> second = {3};
    double res = utils::getEuclideanDistance(first, second);
    EXPECT_EQ(res, 2);
}

TEST(generateRandomDouble, isInRightDimensions)
{
    double res = utils::generateRandomDouble(9, 10);
    ASSERT_LT(res, 10);
    ASSERT_GT(res, 9);
}

TEST(generateRandomInt, isInRightDimensions)
{
    double res = utils::generateRandomInt(8, 8);
    EXPECT_EQ(res, 8);
}

TEST(generateRandomRange, areItemsInRightDimensions)
{
    std::vector<double> res = utils::generateRandomRange(10, 1, 1);
    EXPECT_EQ(res.size(), 10);

    for (auto& item : res)
    {
        EXPECT_EQ(item, 1);
    }
}

TEST(generateRandomRange, doItemsDiffer)
{
    std::vector<double> res = utils::generateRandomRange(10, 1, 100);
    
    EXPECT_NE(res[0], res[1]);
    EXPECT_NE(res[3], res[4]);
    
}

TEST(getRo, returnRo)
{
    const std::vector<double> current = {1};
	const std::vector<std::vector<double>> all = {{2}, {1}, {3}, {7}, {8}, {9}};
	int k = 3;
    //euclidean distances are: 0,1,2,6....
    double res = utils::getRo(current, all, k);
    EXPECT_EQ(res, 3);
}
