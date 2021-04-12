#include <vector>
#include "../utils/result.cpp"
#include "../utils/utils.cpp"
#include "../cec20_test_func.cpp"

class Algorithm
{
private:
    int neighboursK_, singleDimensionFes_, popSize_, dimension_, testFunction_;
    double c1_, c2_, wStart_, wEnd_, w_, vMax_, boundaryLow_, boundaryUp_;

    struct Particle
    {
        std::vector<double> positionXi;
        std::vector<double> velocityVectorVi;
        std::vector<double> pBestPi;
        double pBestCost;
        double ro;
    };

    void initPopulation(std::vector<Particle> &population, std::vector<double> &leadingPosG, double gCost, std::vector<std::vector<double>> &positions)
    {
        for (int i = 0; i < popSize_; i++)
        {
            std::vector<double> randomPosition = utils::generateRandomRange(dimension_, boundaryLow_, boundaryUp_);
            positions.push_back(randomPosition);
            //Initialize the particle's position with a uniformly distributed random vector xi ~ U(BOUNDARY_LOW, BOUNDARY_UP)
            //Initialize the particle's velocity: vi ~ U(-|BOUNDARY_UP-BOUNDARY_LOW|, |BOUNDARY_UP-BOUNDARY_LOW|), maybe 0 would be better
            //Initialize the particle's best known position to its initial position: pi ← xi
            Particle p = {randomPosition, utils::generateRandomRange(dimension_, 0, 0), randomPosition};
            double pCost = 0;
            cec20_test_func(p.pBestPi.data(), &pCost, dimension_, 1, testFunction_);
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

        for (int i = 0; i < popSize_; i++)
        {
            population[i].ro = utils::getRo(population[i].positionXi, positions, neighboursK_);
            if (population[i].ro > biggestRo)
            {
                biggestRo = population[i].ro;
                mostUnique = population[i];
            }
        }
    }

    void backToBoundaries(Particle &particle, int iteration)
    {
        if (boundaryLow_ > particle.positionXi[iteration] || particle.positionXi[iteration] > boundaryUp_)
        {
            double randomNum = utils::generateRandomDouble(boundaryLow_, boundaryUp_);
            particle.positionXi[iteration] = randomNum;
            particle.velocityVectorVi[iteration] = 0;
        }
    }

public:
    Algorithm(int neighboursK = 3, int singleDimensionFes = 5000, int popSize = 25, int dimension = 0, int testFunction = 0, double c1 = 1.496180, double c2 = 1.496180,
              double wStart = 0.9, double wEnd = 0.4, double vMax = 0.2, double boundaryLow = 0, double boundaryUp = 0)
        : neighboursK_(neighboursK), singleDimensionFes_(singleDimensionFes), popSize_(popSize), dimension_(dimension), testFunction_(testFunction),
          c1_(c1), c2_(c2), wStart_(wStart), wEnd_(wEnd), w_(wStart),
          vMax_(vMax), boundaryLow_(boundaryLow), boundaryUp_(boundaryUp)
    {
    }

    std::vector<result> run(int dimension, int testFunction, double boundaryLow, double boundaryUp)
    {
        dimension_ = dimension;
        testFunction_ = testFunction;
        boundaryLow_ = boundaryLow;
        boundaryUp_ = boundaryUp;
        int MAX_FEZ = singleDimensionFes_ * dimension;
        int generations = MAX_FEZ / popSize_;

        std::vector<result> best_results;
        int fezCounter = 0;
        std::vector<Particle> population;
        std::vector<double> leadingPosG = utils::generateRandomRange(dimension, boundaryLow, boundaryUp);
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
                double currentBestRo = utils::getRo(currentParticle.positionXi, positions, neighboursK_);
                if (currentBestRo > biggestRo)
                {
                    mostUnique = currentParticle;
                }

                for (int d = 0; d < dimension; d++)
                {
                    double rp = utils::generateRandomDouble(0, 1);
                    double rg = utils::generateRandomDouble(0, 1);

                    //get some random to go for novelty only in 1:X
                    double noveltyRandom = utils::generateRandomDouble(0, 1);
                    int novelty = 0;

                    double potentialVectorD = 0;

                    if (noveltyRandom > 0.2) // do classic
                    {
                        potentialVectorD = w_ * currentParticle.velocityVectorVi[d] + c1_ * rp * (currentParticle.pBestPi[d] - currentParticle.positionXi[d]) + c2_ * rg * (leadingPosG[d] - currentParticle.positionXi[d]);
                    }
                    else //do novelty
                    {
                        potentialVectorD = w_ * currentParticle.velocityVectorVi[d] + (mostUnique.positionXi[d] - currentParticle.positionXi[d]);
                    }

                    //Update the particle's velocity
                    double vParam = vMax_ * (boundaryUp - boundaryLow);
                    currentParticle.velocityVectorVi[d] = fmax(fmin(potentialVectorD, vParam), -vParam);
                    //Update the particle's position
                    currentParticle.positionXi[d] = currentParticle.positionXi[d] + currentParticle.velocityVectorVi[d];

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
            w_ = wStart_ - ((wStart_ - wEnd_) * g) / generations;
        }

        return best_results;
    }
};
