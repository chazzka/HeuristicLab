#include "gtest/gtest.h"
#include "src/utils/utils.cpp"
#include "test/mock/cec_20_test_func_mock.cc" //first dejong mock.
#include "src/alg/PSO.cpp"

TEST(initPopulationReturnBest, isBestReallyBest)
{
    int popSize = 4;
    int dimension = 2;
    Algorithm pso(1, popSize, dimension, 1, 1, 1, 1, 1, 1, BOUNDARY_LOW, BOUNDARY_UP);

    std::vector<Particle> population;
    Particle gBest;
    pso.initPopulationReturnBest(population, gBest);

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