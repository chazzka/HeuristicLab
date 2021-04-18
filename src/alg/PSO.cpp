#include <vector>

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
};

struct GlobalBest
{
    std::vector<double> gPosition;
    double gCost;
};

class Algorithm
{

private:
    int singleDimensionFes_, popSize_, dimension_, testFunction_;
    double c1_, c2_, wStart_, wEnd_, w_, vMax_, boundaryLow_, boundaryUp_;

    void initPopulation(std::vector<Particle> &population, GlobalBest &gBestParticle)
    {
        for (int i = 0; i < popSize_; i++)
        {
            std::vector<double> randomPosition = utils::generateRandomRange(dimension_, boundaryLow_, boundaryUp_);
            Particle p = {randomPosition, utils::generateRandomRange(dimension_, 0, 0), randomPosition};
            double pCost = 0;
            cec20_test_func(p.pBestPi.data(), &pCost, dimension_, 1, testFunction_);
            p.pBestCost = pCost;
            //find best swarm position and cost
            if (pCost < gBestParticle.gCost)
            {
                gBestParticle.gPosition = p.pBestPi;
                gBestParticle.gCost = pCost;
            }
            population.push_back(p);
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
    Algorithm(int singleDimensionFes = 5000, int popSize = 25, int dimension = 0, int testFunction = 0, double c1 = 1.496180, double c2 = 1.496180,
              double wStart = 0.9, double wEnd = 0.4, double vMax = 0.2, double boundaryLow = 0, double boundaryUp = 0)
        : singleDimensionFes_(singleDimensionFes), popSize_(popSize), dimension_(dimension), testFunction_(testFunction),
          c1_(c1), c2_(c2), wStart_(wStart), wEnd_(wEnd), w_(wStart),
          vMax_(vMax), boundaryLow_(boundaryLow), boundaryUp_(boundaryUp)
    {
    }

    void dimensionMove(Particle &currentParticle, GlobalBest &gBestParticleParticle, int d)
    {
        double rp = utils::generateRandomDouble(0, 1);
        double rg = utils::generateRandomDouble(0, 1);

        double potentialVectorD = w_ * currentParticle.velocityVectorVi[d] + c1_ * rp * (currentParticle.pBestPi[d] - currentParticle.positionXi[d]) + c2_ * rg * (gBestParticleParticle.gPosition[d] - currentParticle.positionXi[d]);
        //Update the particle's velocity
        double vParam = vMax_ * (boundaryUp_ - boundaryLow_);
        currentParticle.velocityVectorVi[d] = fmax(fmin(potentialVectorD, vParam), -vParam);
        //Update the particle's position
        currentParticle.positionXi[d] = currentParticle.positionXi[d] + currentParticle.velocityVectorVi[d];

        backToBoundaries(currentParticle, d);
    }

    std::vector<result> run(int dimensionsCount, int testFunction, double boundaryLow, double boundaryUp)
    {
        dimension_ = dimensionsCount;
        testFunction_ = testFunction;
        boundaryLow_ = boundaryLow;
        boundaryUp_ = boundaryUp;
        int MAX_FEZ = singleDimensionFes_ * dimensionsCount;
        int generations = MAX_FEZ / popSize_;

        std::vector<result> best_results;
        int fezCounter = 0;
        std::vector<Particle> population;

        GlobalBest gBestParticle;
        gBestParticle.gPosition = utils::generateRandomRange(dimensionsCount, boundaryLow, boundaryUp);
        cec20_test_func(gBestParticle.gPosition.data(), &gBestParticle.gCost, dimensionsCount, 1, testFunction);

        initPopulation(population, gBestParticle);

        for (int g = 0; g < generations; g++)
        {
            for (int i = 0; i < population.size(); i++)
            {
                Particle &currentParticle = population[i];

                for (int d = 0; d < dimensionsCount; d++)
                {
                    dimensionMove(currentParticle, gBestParticle, d);
                }

                double newXiCost = 0;
                cec20_test_func(currentParticle.positionXi.data(), &newXiCost, dimensionsCount, 1, testFunction);
                fezCounter++;

                if (newXiCost < currentParticle.pBestCost)
                {
                    //Update the particle's best known position
                    currentParticle.pBestPi = currentParticle.positionXi;
                    currentParticle.pBestCost = newXiCost;

                    if (newXiCost < gBestParticle.gCost)
                    {
                        //Update the swarm's best known position
                        gBestParticle.gCost = newXiCost;
                        gBestParticle.gPosition = currentParticle.positionXi;
                    }
                }

                result res;
                res.fez = fezCounter;
                res.cost = gBestParticle.gCost;
                best_results.push_back(res);
            }
            //after generation run, recount w
            w_ = wStart_ - ((wStart_ - wEnd_) * g) / generations;
        }

        return best_results;
    }
};
