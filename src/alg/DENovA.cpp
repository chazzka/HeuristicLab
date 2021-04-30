/*
nová pozice y se přijme pokud je lepší NEBO se stala nejunikátnější pozicí
*/
#include <vector>

struct result
{
    int fez;
    double cost;
};

struct Agent
{
    std::vector<double> position;
    double cost;
    double ro;
};

class Algorithm
{
private:
    int neighboursK_, singleDimensionFes_, popSize_, dimension_, testFunction_;
    double CR_, F_, boundaryLow_, boundaryUp_;

    void getPositions(std::vector<Agent> &population, std::vector<std::vector<double>> &positions)
    {
        positions.clear();

        for (auto &agent : population)
        {
            positions.push_back(agent.position);
        }
    }

public:
    Algorithm(int neighboursK = 3, int singleDimensionFes = 5000, int popSize = 25, int dimension = 0, int testFunction = 0,
              double CR = 0.8, double F = 0.5, double boundaryLow = 0, double boundaryUp = 0)
        : neighboursK_(neighboursK), singleDimensionFes_(singleDimensionFes), popSize_(popSize), dimension_(dimension), testFunction_(testFunction),
          CR_(CR), F_(F), boundaryLow_(boundaryLow), boundaryUp_(boundaryUp)
    {
    }

    std::vector<Agent> get3distinct(std::vector<Agent> populace, Agent jed)
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
            int ran = utils::generateRandomInt(0, populace.size() - 1);
            result.push_back(populace.at(ran));
            populace.erase(populace.begin() + ran);
        }

        return result;
    }

    std::vector<Agent> get2distinct(std::vector<Agent> populace, Agent jed)
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
            int ran = utils::generateRandomInt(0, populace.size() - 1);
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
            Agent a = {randomPosition, 0};
            a.ro = 0;
            cec20_test_func(a.position.data(), &a.cost, dimension_, 1, testFunction_);
            if (a.cost < gCost)
            {
                leadingPosG = a.position;
                gCost = a.cost;
            }
            population.push_back(a);
        }
    }

    double backToBoundariesMirror(double yi)
    {
        if (yi < boundaryLow_)
        {
            yi = boundaryLow_ + (boundaryLow_ - yi);
        }
        if (yi > boundaryUp_)
        {
            yi = boundaryUp_ + (boundaryUp_ - yi);
        }
        return yi;
    }

    double backToBoundariesRandom(double yi)
    {
        if (boundaryLow_ > yi || yi > boundaryUp_)
        {
            return utils::generateRandomDouble(boundaryLow_, boundaryUp_);
        }
        else
        {
            return yi;
        }
    }

    double dimensionMove(double ai, double bi, double ci)
    {
        double yi = ai + F_ * (bi - ci);
        return backToBoundariesMirror(yi);
    }

    void evaluateRoGetMostUnique(std::vector<Agent> &population, Agent &mostUnique, std::vector<std::vector<double>> &positions)
    {

        getPositions(population, positions);

        for (int i = 0; i < popSize_; i++)
        {
            double ro = utils::getRo(population[i].position, positions, neighboursK_);
            if (population[i].ro > mostUnique.ro)
            {
                mostUnique.ro = population[i].ro;
                mostUnique = population[i];
            }
        }
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

        Agent mostUnique = population[0];

        for (int i = 0; i < iterations; i++)
        {
            //vytvořim si temp populaci, ktera si nakopiruje prvky z populace
            std::vector<Agent> tempPopulation = population;
            //budu tuto temp populaci procházet
            for (int j = 0; j < tempPopulation.size(); j++)
            {
                std::vector<Agent> twoDistinct = get2distinct(population, tempPopulation[j]);

                //Pick a random index R
                int R = utils::generateRandomInt(1, dimension_);
                //Compute the agent's potentially new position y
                std::vector<double> y;

                for (int d = 0; d < dimension_; d++)
                {
                    //pick a uniformly distributed random number ri
                    double r = utils::generateRandomDouble(0, 1);
                    if (r < CR_ || d == R)
                    {
                        //verze BEST/1/bin
                        y.push_back(dimensionMove(gPosition[d], twoDistinct[0].position[d], twoDistinct[1].position[d]));
                    }
                    // otherwise set yi = xi
                    else
                    {
                        y.push_back(tempPopulation[j].position[d]);
                    }
                }

                //teď mám vektor y = nová pozice
                //nastavim ho falešnému agentovi
                tempPopulation[j].position = y;

                //teď spočítám novelty nad touto falešnou populaci se změněným agentem
                std::vector<std::vector<double>> positions;
                evaluateRoGetMostUnique(tempPopulation, mostUnique, positions);

                double yCost = 0;
                cec20_test_func(y.data(), &yCost, dimension, 1, testFunction);
                fezCounter++;
                //zjistim jestli je tento agent s novou pozicí lepší NEBO nejunikátnější
                if (yCost <= tempPopulation[j].cost || tempPopulation[j].ro == mostUnique.ro)
                {
                    //then replace the agent x in the population with  y.
                    //nahraď to ale v opravdové populaci už
                    population[j].position = y;
                    population[j].cost = yCost;
                    if (yCost < gCost)
                    {
                        gCost = yCost;
                        gPosition = y;
                    }
                }

                bestResults.push_back({fezCounter, gCost});
            }
        }
        return bestResults;
    }
};
