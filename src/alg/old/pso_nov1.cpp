#include <vector>
#include <iostream>
#include <random>
#include "../../utils/utils.cpp"
#include "../../cec20_test_func.cpp"

//K in K-NN
#define K 5

const int popSize = 25; //nebo 50, hlavně ať je to dělitelné 5000 - experimentuj a dívej se jak to vychází
int D, TF;
double c1 = 1.496180, c2 = c1, wStart = 0.9, wEnd = 0.4, vMax = 0.2, w = wStart, BOUNDARY_LOW, BOUNDARY_UP;

struct result
{
    int fez;
    double cost;
};

struct Particle
{
    std::vector<double> positionXi;
    std::vector<double> velocityVectorVi;
    std::vector<double> pBestPi;
    double pBestCost;
    double ro;
};

std::vector<double> generateRandomRange(int size, double min, double max)
{
    std::vector<double> rndNumbers;

    double randomNumber;
    for (int index = 0; index < size; index++)
    {
        randomNumber = (max - min) * ((double)rand() / (double)RAND_MAX) + min;
        rndNumbers.push_back(randomNumber);
    }

    return rndNumbers;
}

double generateRandomDouble(double min, double max)
{

    return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

void initPopulation(std::vector<Particle> &population, std::vector<double> &leadingPosG, double gCost, std::vector<std::vector<double>> &positions)
{
    for (int i = 0; i < popSize; i++)
    {
        std::vector<double> randomPosition = generateRandomRange(D, BOUNDARY_LOW, BOUNDARY_UP);
        positions.push_back(randomPosition);
        //Initialize the particle's position with a uniformly distributed random vector xi ~ U(BOUNDARY_LOW, BOUNDARY_UP)
        //Initialize the particle's velocity: vi ~ U(-|BOUNDARY_UP-BOUNDARY_LOW|, |BOUNDARY_UP-BOUNDARY_LOW|), maybe 0 would be better
        //Initialize the particle's best known position to its initial position: pi ← xi
        Particle p = {randomPosition, generateRandomRange(D, 0, 0), randomPosition};
        double pCost = 0;
        cec20_test_func(p.pBestPi.data(), &pCost, D, 1, TF);
        p.pBestCost = pCost;
        //find best swarm position and cost
        if (pCost < gCost)
        {
            //update the swarm's best known position: g ← pi
            leadingPosG = p.pBestPi;
            gCost = pCost;
        }
        population.push_back(p);
    }
}

void initRo(std::vector<Particle> &population, std::vector<std::vector<double>> &positions, Particle &mostUnique, double &biggestRo)
{
    biggestRo = 0; //biggestRo is most unique

    for (int i = 0; i < popSize; i++)
    {
        population[i].ro = utils::getRo(population[i].positionXi, positions, K);
        if (population[i].ro > biggestRo)
        {
            biggestRo = population[i].ro;
            mostUnique = population[i];
        }
    }
}

void backToBoundaries(Particle &particle, int iteration)
{
    if (BOUNDARY_LOW > particle.positionXi[iteration] || particle.positionXi[iteration] > BOUNDARY_UP)
    {
        double randomNum = generateRandomDouble((double)BOUNDARY_LOW, (double)BOUNDARY_UP);
        particle.positionXi[iteration] = randomNum;
        particle.velocityVectorVi[iteration] = 0;
    }
}

std::vector<result> run(int dimension, int testFunction, int boundaryLow, int boundaryUp)
{
    D = dimension;
    TF = testFunction;
    BOUNDARY_LOW = boundaryLow;
    BOUNDARY_UP = boundaryUp;
    int MAX_FEZ = 5000 * dimension;
    int generations = MAX_FEZ / popSize;

    std::vector<result> best_results;
    int fezCounter = 0;
    std::vector<Particle> population;
    std::vector<double> leadingPosG = generateRandomRange(dimension, boundaryLow, boundaryUp);
    double gCost = 0;
    cec20_test_func(leadingPosG.data(), &gCost, dimension, 1, testFunction);

    //positions will be used for novelty evaluation
    std::vector<std::vector<double>> positions;
    initPopulation(population, leadingPosG, gCost, positions);

    Particle mostUnique;
    double biggestRo = 0;
    initRo(population, positions, mostUnique, biggestRo);

    for (int g = 0; g < generations; g++)
    {
        for (int i = 0; i < population.size(); i++)
        {
            Particle &currentParticle = population[i];
            //positions are being changed during this cycle, keep track of the most unique one
            double currentBestRo = utils::getRo(currentParticle.positionXi, positions, K);
            if (currentBestRo > biggestRo)
            {
                mostUnique = currentParticle;
            }

            for (int d = 0; d < dimension; d++)
            {
                double rp = generateRandomDouble(0, 1);
                double rg = generateRandomDouble(0, 1);

                //get some random to go for novelty only in 1:X
                double noveltyRandom = generateRandomDouble(0, 1);
                int novelty = 0;

                double potentialVectorD = 0;

                //Update the particle's velocity
                if (noveltyRandom > 0.2) // do classic
                {
                    potentialVectorD = w * currentParticle.velocityVectorVi[d] + c1 * rp * (currentParticle.pBestPi[d] - currentParticle.positionXi[d]) + c2 * rg * (leadingPosG[d] - currentParticle.positionXi[d]);
                }
                else //do novelty
                {
                    potentialVectorD = w * currentParticle.velocityVectorVi[d] + (mostUnique.positionXi[d] - currentParticle.positionXi[d]);
                }

                double vParam = vMax * (boundaryUp - boundaryLow);
                currentParticle.velocityVectorVi[d] = fmax(fmin(potentialVectorD, vParam), -vParam);
                //Update the particle's position
                currentParticle.positionXi[d] = currentParticle.positionXi[d] + currentParticle.velocityVectorVi[d];

                //boundary check
                backToBoundaries(currentParticle, d);
            }
            double newXiCost = 0;
            cec20_test_func(currentParticle.positionXi.data(), &newXiCost, dimension, 1, testFunction);
            fezCounter++;

            if (newXiCost < currentParticle.pBestCost)
            {
                //Update the particle's best known position
                currentParticle.pBestPi = currentParticle.positionXi;
                currentParticle.pBestCost = newXiCost;

                if (newXiCost < gCost)
                {
                    //Update the swarm's best known position
                    gCost = newXiCost;
                    leadingPosG = currentParticle.positionXi;
                }
            }

            result res;
            res.fez = fezCounter;
            res.cost = gCost;
            best_results.push_back(res);
        }
        //after generation run, recount w
        w = wStart - ((wStart - wEnd) * g) / generations;
    }
    return best_results;
}