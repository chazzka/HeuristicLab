#include "gtest/gtest.h"
#include "test/mock/cec_20_test_func_mock.cc" //first dejong mock.
#include "src/utils/utils.cpp"
#include "src/alg/PSONovA.cpp"

//utils (and hence randomness) is NOT mocked!

TEST(initPopulationReturnBest, isBestReallyBest)
{
    int popSize = 4;
    int dimension = 2;
    Algorithm psoNov(4, 12, 4, 1, 1, 1.496180, 1.496180, 0.9, 0.4, 0.2, BOUNDARY_LOW, BOUNDARY_UP);

    std::vector<Particle> population;
    Particle gBest;
    psoNov.initPopulationReturnBest(population, gBest);

    Particle testBest = population[1];
    for (auto &p : population)
    {
        if (p.bestCost < testBest.bestCost)
        {
            testBest = p;
        }
    }

    ASSERT_EQ(gBest.bestCost, testBest.bestCost);
    ASSERT_EQ(gBest.positionXi, testBest.positionXi);
}

TEST(run, isCorrectNumberOfFes)
{
    Algorithm psoNov(4, 12, 4, 1, 1, 1.496180, 1.496180, 0.9, 0.4, 0.2, BOUNDARY_LOW, BOUNDARY_UP);
    std::vector<result> res = psoNov.run(1, 1, 1, 1);
    ASSERT_EQ(res.size(), 12);
}

//getRo is tested in utils test
TEST(initRo, isBestRoOK)
{
    Algorithm psoNov(4, 12, 4, 1, 1, 1.496180, 1.496180, 0.9, 0.4, 0.2, BOUNDARY_LOW, BOUNDARY_UP);
    Particle test1;
    test1.positionXi = {1};

    Particle test2;
    test2.positionXi = {1.1};

    Particle test3;
    test3.positionXi = {1.2};

    Particle test4;
    test4.positionXi = {500};

    std::vector<Particle> population;

    population.push_back(test1);
    population.push_back(test2);
    population.push_back(test3);
    population.push_back(test4);

    std::vector<std::vector<double>> positions;

    psoNov.initRo(population, positions);

    ASSERT_EQ(population[0].ro, population[0].bestRo);
    ASSERT_EQ(population[1].ro, population[1].bestRo);
    ASSERT_EQ(population[2].ro, population[2].bestRo);
    ASSERT_EQ(population[3].ro, population[3].bestRo);
}
