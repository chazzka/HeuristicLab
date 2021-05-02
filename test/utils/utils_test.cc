#include "gtest/gtest.h"
#include "src/utils/utils.cpp"

TEST(getEuclideanDistanceVector, checkDistance)
{
    std::vector<double> first = {1};
    std::vector<double> second = {3};
    double res = utils::getEuclideanDistance(first, second);
    EXPECT_EQ(res, 2);
}

TEST(getEuclideanDistanceDouble, checkDistance)
{
    double first = 1.1;
    double second = 3.1;
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
    std::random_device rd;
    std::mt19937 rng(rd());
    double res = utils::generateRandomInt(8, 8);
    EXPECT_EQ(res, 8);

    std::mt19937 rng2 = rng;
    double res3 = utils::generateRandomInt(1, 100, rng);
    double res4 = utils::generateRandomInt(1, 100, rng2);
    EXPECT_EQ(res3, res4);
}

TEST(generateRandomRange, areItemsInRightDimensions)
{
    std::vector<double> res = utils::generateRandomRange(10, 1, 1);
    EXPECT_EQ(res.size(), 10);

    for (auto &item : res)
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

TEST(getRoOneDimension0, returnRo)
{
    const std::vector<double> current = {1, 2};
    const std::vector<std::vector<double>> all = {{2, 50}, {1, 50}, {3, 50}, {7, 60}, {8, 20}, {9, 100}};
    int k = 3;
    double res = utils::getRoOneDimension(current, all, k, 0);
    EXPECT_EQ(res, 3);
}

TEST(getRoOneDimension1, returnRo)
{
    const std::vector<double> current = {2, 1};
    const std::vector<std::vector<double>> all = {{50, 2}, {50, 1}, {50, 3}, {60, 7}, {20, 8}, {100, 9}};
    int k = 3;
    double res = utils::getRoOneDimension(current, all, k, 1);
    EXPECT_EQ(res, 3);
}
