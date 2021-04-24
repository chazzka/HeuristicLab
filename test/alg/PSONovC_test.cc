#include "gtest/gtest.h"
#include "test/mock/cec_20_test_func_mock.cc" //first dejong mock.
#include "src/utils/utils.cpp"
#include "src/alg/PSONovC.cpp"

//utils (and hence randomness) is NOT mocked!

TEST(initPopulationReturnBest, isBestReallyBest)
{
    int popSize = 4;
    int dimension = 2;
    Algorithm psoNovC(20, 4, 12, popSize, dimension, 1, 1.496180, 1.496180, 0.9, 0.4, 0.2, BOUNDARY_LOW, BOUNDARY_UP);

    std::vector<Particle> population;
    Particle gBest;
    psoNovC.initPopulationReturnBest(population, gBest);

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
    Algorithm psoNovC(20, 4, 12, 4, 2, 1, 1.496180, 1.496180, 0.9, 0.4, 0.2, BOUNDARY_LOW, BOUNDARY_UP);
    std::vector<result> res = psoNovC.run(1, 1, 1, 1);
    ASSERT_EQ(res.size(), 12);
}

//doNoveltyPopulationRun nehleda besta
//doNoveltyPopulationRun vždy nejdřív pohne částicí až pak se ptá jestli je lepší
TEST(doNoveltyPopulationRun, isGBestBetter)
{

    Algorithm psoNovC(20, 4, 12, 4, 2, 1, 1.496180, 1.496180, 0.9, 0.4, 0.2, BOUNDARY_LOW, BOUNDARY_UP);

    Particle test1;
    test1.positionXi = {500, 500};
    test1.pBestPi = {500, 500};
    test1.velocityVectorVi = {1, 1};
    test1.ro = 0;
    test1.bestRo = 0;
    cec20_test_func(test1.positionXi.data(), &test1.bestCost, 2, 1, 1);

    Particle test2;
    test2.positionXi = {700000, 800000};
    test2.pBestPi = {700000, 800000};
    test2.velocityVectorVi = {1, 1};
    test2.ro = 0;
    test2.bestRo = 0;
    cec20_test_func(test2.positionXi.data(), &test2.bestCost, 2, 1, 1);

    Particle test3;
    test3.positionXi = {100, 102};
    test3.pBestPi = {100, 102};
    test3.velocityVectorVi = {1, 1};
    test3.ro = 0;
    test3.bestRo = 0;
    cec20_test_func(test3.positionXi.data(), &test3.bestCost, 2, 1, 1);

    Particle test4;
    test4.positionXi = {200, 103};
    test4.pBestPi = {200, 103};
    test4.velocityVectorVi = {1, 1};
    test4.ro = 0;
    test4.bestRo = 0;
    cec20_test_func(test4.positionXi.data(), &test4.bestCost, 2, 1, 1);

    std::vector<Particle> population;

    population.push_back(test1);
    population.push_back(test2);
    population.push_back(test3);
    population.push_back(test4);

    //gBest has to be initialized
    Particle gBestParticle = population[3];

    Particle &mostUnique = population[3];

    std::vector<std::vector<double>> positions;
    psoNovC.initRo(population, mostUnique, positions);

    for (int i = 0; i < population.size(); i++)
    {
        psoNovC.doNoveltyPopulationRun(population, positions, mostUnique, gBestParticle, i);
    }

    std::vector<double> fsad = {1, 2};

    Particle testBest = population[1];

    for (auto &p : population)
    {
        if (p.bestCost < testBest.bestCost)
        {
            testBest = p;
        }
    }

    ASSERT_EQ(gBestParticle.positionXi, testBest.positionXi);
    ASSERT_EQ(gBestParticle.bestCost, testBest.bestCost);
}


TEST(doClassicPopulationRun, isGBestBetter)
{

    Algorithm psoNovC(20, 4, 12, 4, 2, 1, 1.496180, 1.496180, 0.9, 0.4, 0.2, BOUNDARY_LOW, BOUNDARY_UP);

    Particle test1;
    test1.positionXi = {500, 500};
    test1.pBestPi = {500, 500};
    test1.velocityVectorVi = {1, 1};
    test1.ro = 0;
    test1.bestRo = 0;
    cec20_test_func(test1.positionXi.data(), &test1.bestCost, 2, 1, 1);

    Particle test2;
    test2.positionXi = {700000, 800000};
    test2.pBestPi = {700000, 800000};
    test2.velocityVectorVi = {1, 1};
    test2.ro = 0;
    test2.bestRo = 0;
    cec20_test_func(test2.positionXi.data(), &test2.bestCost, 2, 1, 1);

    Particle test3;
    test3.positionXi = {100, 102};
    test3.pBestPi = {100, 102};
    test3.velocityVectorVi = {1, 1};
    test3.ro = 0;
    test3.bestRo = 0;
    cec20_test_func(test3.positionXi.data(), &test3.bestCost, 2, 1, 1);

    Particle test4;
    test4.positionXi = {200, 103};
    test4.pBestPi = {200, 103};
    test4.velocityVectorVi = {1, 1};
    test4.ro = 0;
    test4.bestRo = 0;
    cec20_test_func(test4.positionXi.data(), &test4.bestCost, 2, 1, 1);

    std::vector<Particle> population;

    population.push_back(test1);
    population.push_back(test2);
    population.push_back(test3);
    population.push_back(test4);

    //gBest has to be initialized
    Particle gBestParticle = population[3];

    Particle &mostUnique = population[3];

    std::vector<std::vector<double>> positions;
    psoNovC.initRo(population, mostUnique, positions);

    for (int i = 0; i < population.size(); i++)
    {
        psoNovC.doClassicPopulationRun(population, gBestParticle, i);
    }

    std::vector<double> fsad = {1, 2};

    Particle testBest = population[1];

    for (auto &p : population)
    {
        if (p.bestCost < testBest.bestCost)
        {
            testBest = p;
        }
    }

    ASSERT_EQ(gBestParticle.positionXi, testBest.positionXi);
    ASSERT_EQ(gBestParticle.bestCost, testBest.bestCost);
}
