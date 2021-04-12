#include <vector>
#include "../utils/result.cpp"
#include "../utils/utils.cpp"
#include "../cec20_test_func.cpp"

class Algorithm
{
private:
    int singleDimensionFes_, popSize_, dimension_, testFunction_;
    double CR_, F_, boundaryLow_, boundaryUp_;

    struct Agent
    {
        std::vector<double> position;
        double cost;
    };

    std::vector<Agent> get3unique(std::vector<Agent> populace, Agent jed)
    {

        int i = 0;
        for (Agent j : populace)
        {
            if (j.position == jed.position)
            {
                populace.erase(populace.begin() + i);
                break;
            }
            i++;
        }

        std::vector<Agent> result;

        for (int i = 0; i < 3; i++)
        {
            int ran = utils::generateRandomInt(0,populace.size());
            result.push_back(populace.at(ran));
            populace.erase(populace.begin() + ran);
        }

        return result;
    }

    std::vector<Agent> get2unique(std::vector<Agent> populace, Agent jed)
    {
        int i = 0;
        for (Agent j : populace)
        {
            if (j.position == jed.position)
            {
                populace.erase(populace.begin() + i);
                break;
            }
            i++;
        }

        std::vector<Agent> result;
        for (int i = 0; i < 2; i++)
        {
            int ran = utils::generateRandomInt(0,populace.size());
            result.push_back(populace.at(ran));
            populace.erase(populace.begin() + ran);
        }

        return result;
    }

    void initPopulation(std::vector<Agent> &population, std::vector<double> &leadingPosG, double gCost)
    {
        for (int i = 0; i < popSize_; i++)
        {
            std::vector<double> randomPosition = utils::generateRandomRange(dimension_, boundaryLow_, boundaryUp_);
            //Initialize the particle's position with a uniformly distributed random vector xi ~ U(BOUNDARY_LOW, BOUNDARY_UP)
            //TODO initial velocity 0?
            //Initialize the particle's velocity: vi ~ U(-|BOUNDARY_UP-BOUNDARY_LOW|, |BOUNDARY_UP-BOUNDARY_LOW|), maybe 0 would be better
            //Initialize the particle's best known position to its initial position: pi ← xi
            Agent a = {randomPosition, 0};
            cec20_test_func(a.position.data(), &a.cost, dimension_, 1, testFunction_);
            if (a.cost < gCost)
            {
                //update the swarm's best known position: g ← pi
                leadingPosG = a.position;
                gCost = a.cost;
            }
            population.push_back(a);
        }
    }

    void backToBoundariesRandom(double yi, std::vector<double> &y)
    {
        if (boundaryLow_ > yi || yi > boundaryUp_)
        {
            y.push_back(utils::generateRandomDouble(boundaryLow_, boundaryUp_));
        }
        else
        {
            y.push_back(yi);
        }
    }

public:
    Algorithm(int singleDimensionFes = 5000, int popSize = 50, int dimension = 0, int testFunction = 0,
              double CR = 0.8, double F = 0.5, double boundaryLow = 0, double boundaryUp = 0)
        : singleDimensionFes_(singleDimensionFes), popSize_(popSize), dimension_(dimension), testFunction_(testFunction),
          CR_(CR), F_(F), boundaryLow_(boundaryLow), boundaryUp_(boundaryUp)
    {
    }

    std::vector<result> run(int dimension, int testFunction, double boundaryLow, double boundaryUp)
    {
        dimension_ = dimension;
        testFunction_ = testFunction;
        boundaryLow_ = boundaryLow;
        boundaryUp_ = boundaryUp;
        int MAX_FEZ = singleDimensionFes_ * dimension;
        int iterations = MAX_FEZ / popSize_;

        std::vector<result> bestResults;
        int fezCounter = 0;
        std::vector<Agent> population;
        std::vector<double> gPosition = utils::generateRandomRange(dimension_, boundaryLow, boundaryUp);
        double gCost = 0;
        cec20_test_func(gPosition.data(), &gCost, dimension, 1, testFunction);

        initPopulation(population, gPosition, gCost);

        while (iterations--)
        {
            std::vector<Agent> uiPopulace;
            //For each agent x in the population do:
            for (Agent x : population)
            {
                //Pick three unique agents
                std::vector<Agent> twoUnique = get2unique(population, x);
                //Pick a random index R
                double R = utils::generateRandomDouble(1, dimension_);
                //Compute the agent's potentially new position y
                std::vector<double> y;
                for (int d = 0; d < dimension_; d++)
                {
                    //pick a uniformly distributed random number ri
                    int r = utils::generateRandomInt(1, dimension_);
                    if (r < CR_ || d == R)
                    {
                        //verze BEST/1/bin
                        double yi = gPosition[d] + F_ * (twoUnique[0].position[d] - twoUnique[1].position[d]);
                        //out of dimension check - TODO, udělej mirroring nebo reflection
                        backToBoundariesRandom(yi, y);
                    }
                    // otherwise set yi = xi
                    else
                    {
                        y.push_back(x.position[d]);
                    }
                }
                double yCost = 0;
                cec20_test_func(y.data(), &yCost, dimension, 1, testFunction);
                fezCounter++;

                if (yCost <= x.cost)
                {
                    //then replace the agent x in the population with  y.
                    x.position = y;
                    x.cost = yCost;
                    if (yCost < gCost)
                    {
                        gCost = yCost;
                        gPosition = y;
                    }
                }

                result res;
                res.cost = gCost;
                res.fez = fezCounter;
                bestResults.push_back(res);
            }
        }
        return bestResults;
    }
};
