/*
    novelty bude nasazeno v případě že se dlouho opakují stejné či podobné výsledky, jen do konce populace
*/
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
    double bestCost;
    double ro;
    double bestRo;
};

class Algorithm
{

private:
    int maxHomogeneity_, neighboursK_, singleDimensionFes_, popSize_, dimension_, testFunction_;
    double c1_, c2_, wStart_, wEnd_, w_, vMax_, boundaryLow_, boundaryUp_;

    void initParticleRandom(Particle &p)
    {
        std::vector<double> zeros(dimension_, 0.0);
        std::vector<double> randomPosition = utils::generateRandomRange(dimension_, boundaryLow_, boundaryUp_);

        p.positionXi = randomPosition;
        p.velocityVectorVi = zeros;
        p.pBestPi = randomPosition;
        cec20_test_func(p.pBestPi.data(), &p.bestCost, dimension_, 1, testFunction_);
        p.ro = 0.0;
        p.bestRo = 0.0;
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

    void getPositions(std::vector<Particle> &population, std::vector<std::vector<double>> &positions)
    {
        positions.clear();

        for (auto &particle : population)
        {
            positions.push_back(particle.positionXi);
        }
    }

public:
    Algorithm(int maxHomogeneity = 20, int neighboursK = 3, int singleDimensionFes = 5000, int popSize = 25, int dimension = 0, int testFunction = 0, double c1 = 1.496180, double c2 = 1.496180,
              double wStart = 0.9, double wEnd = 0.4, double vMax = 0.2, double boundaryLow = 0, double boundaryUp = 0)
        : maxHomogeneity_(maxHomogeneity), neighboursK_(neighboursK), singleDimensionFes_(singleDimensionFes), popSize_(popSize), dimension_(dimension), testFunction_(testFunction),
          c1_(c1), c2_(c2), wStart_(wStart), wEnd_(wEnd), w_(wStart),
          vMax_(vMax), boundaryLow_(boundaryLow), boundaryUp_(boundaryUp)
    {
    }

    void initPopulationReturnBest(std::vector<Particle> &population, Particle &gBestParticle)
    {
        initParticleRandom(gBestParticle);
        population.push_back(gBestParticle);
        for (int i = 1; i < popSize_; i++)
        {
            Particle p;
            initParticleRandom(p);

            //find best swarm position and cost
            if (p.bestCost < gBestParticle.bestCost)
            {
                gBestParticle.positionXi = p.pBestPi;
                gBestParticle.bestCost = p.bestCost;
            }
            population.push_back(p);
        }
    }

    void dimensionMove(Particle &currentParticle, Particle &gBestParticle, int d)
    {

        double rp = utils::generateRandomDouble(0, 1);
        double rg = utils::generateRandomDouble(0, 1);

        double inertia = w_ * currentParticle.velocityVectorVi[d];
        double attractionToSelf = c1_ * rp * (currentParticle.pBestPi[d] - currentParticle.positionXi[d]);
        double attractionToBest = c2_ * rg * (gBestParticle.positionXi[d] - currentParticle.positionXi[d]);
        double potentialVectorD = inertia + attractionToSelf + attractionToBest;
        //Update the particle's velocity
        double vParam = vMax_ * (boundaryUp_ - boundaryLow_);
        currentParticle.velocityVectorVi[d] = fmax(fmin(potentialVectorD, vParam), -vParam);
        //Update the particle's position
        currentParticle.positionXi[d] = currentParticle.positionXi[d] + currentParticle.velocityVectorVi[d];

        backToBoundaries(currentParticle, d);
    }

    void evaluateRoGetMostUnique(std::vector<Particle> &population, Particle &mostUnique, std::vector<std::vector<double>> &positions)
    {

        getPositions(population, positions);

        for (int i = 0; i < popSize_; i++)
        {
            population[i].ro = utils::getRo(population[i].positionXi, positions, neighboursK_);
            if (population[i].ro > population[i].bestRo)
            {
                population[i].bestRo = population[i].ro;
            }
            if (population[i].ro > mostUnique.ro)
            {
                mostUnique = population[i];
            }
        }
    }

    void doNoveltyPopulationRun(std::vector<Particle> &population, std::vector<std::vector<double>> &positions, Particle &mostUnique, Particle &gBestParticle, int i)
    {
        Particle &currentParticle = population[i];

        evaluateRoGetMostUnique(population, mostUnique, positions);

        for (int d = 0; d < dimension_; d++)
        {
            dimensionMove(currentParticle, mostUnique, d);
        }

        double newXiCost = 0;
        cec20_test_func(currentParticle.positionXi.data(), &newXiCost, dimension_, 1, testFunction_);

        if (newXiCost < currentParticle.bestCost)
        {
            //Update the particle's best known position
            currentParticle.pBestPi = currentParticle.positionXi;
            currentParticle.bestCost = newXiCost;

            if (newXiCost < gBestParticle.bestCost)
            {
                //Update the swarm's best known position
                gBestParticle.bestCost = newXiCost;
                gBestParticle.positionXi = currentParticle.positionXi;
            }
        }
    }

    void doClassicPopulationRun(std::vector<Particle> &population, Particle &gBestParticle, int i)
    {
        Particle &currentParticle = population[i];

        for (int d = 0; d < dimension_; d++)
        {
            dimensionMove(currentParticle, gBestParticle, d);
        }

        double newXiCost = 0;
        cec20_test_func(currentParticle.positionXi.data(), &newXiCost, dimension_, 1, testFunction_);

        if (newXiCost < currentParticle.bestCost)
        {
            //Update the particle's best known position
            currentParticle.pBestPi = currentParticle.positionXi;
            currentParticle.bestCost = newXiCost;

            if (newXiCost < gBestParticle.bestCost)
            {
                //Update the swarm's best known position
                gBestParticle.bestCost = newXiCost;
                gBestParticle.positionXi = currentParticle.positionXi;
            }
        }
    }

    std::vector<result> run(int dimensionsCount, int testFunction, double boundaryLow, double boundaryUp)
    {
        dimension_ = dimensionsCount;
        testFunction_ = testFunction;
        boundaryLow_ = boundaryLow;
        boundaryUp_ = boundaryUp;
        int MAX_FEZ = singleDimensionFes_ * dimensionsCount;
        int generations = MAX_FEZ / popSize_;
        //novelty
        int homogeneousCounter = 0;
        double previousCost = 0;

        std::vector<result> best_results;
        int fezCounter = 0;
        std::vector<Particle> population;

        Particle gBestParticle;
        initPopulationReturnBest(population, gBestParticle);

        Particle &mostUnique = population[0];
        std::vector<std::vector<double>> positions;

        int novelty = 0;

        for (int g = 0; g < generations; g++)
        {
            //novelty turn off
            novelty = 0;

            for (int i = 0; i < population.size(); i++)
            {

                if (gBestParticle.bestCost == previousCost)
                {
                    if (novelty == 0)
                    {
                        doClassicPopulationRun(population, gBestParticle, i);
                        fezCounter++;
                        homogeneousCounter++;

                        if (homogeneousCounter > maxHomogeneity_)
                        {
                            novelty = 1;
                            homogeneousCounter = 0;
                        }
                    }
                    else
                    {
                        doNoveltyPopulationRun(population, positions, mostUnique, gBestParticle, i);
                        fezCounter++;
                    }
                }
                else
                {
                    novelty = 0;
                    doClassicPopulationRun(population, gBestParticle, i);
                    fezCounter++;
                }

                previousCost = gBestParticle.bestCost;

                best_results.push_back({fezCounter, gBestParticle.bestCost});
            }
            //after generation run, recount w
            w_ = wStart_ - ((wStart_ - wEnd_) * g) / generations;
        }

        return best_results;
    }
};
