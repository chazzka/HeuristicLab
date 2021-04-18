#include <vector>
#include <set>
#include <cmath>
#include <random>

//every method returns 1;
namespace utils
{
    namespace
    {
        double getEuclideanDistance(const std::vector<double> &from, const std::vector<double> &to)
        {
            return 1.0;
        }
    }

    int generateRandomInt(int minIncluded, int maxIncluded)
    {
        return 1;
    }

    double generateRandomDouble(double minIncluded, double maxIncluded)
    {
        return 1.0;
    }

    std::vector<double> generateRandomRange(int size, double min, double max)
    {
        std::vector<double> rndNumbers;

        for (int index = 0; index < size; index++)
        {
            rndNumbers.push_back(generateRandomDouble(min, max));
        }

        return rndNumbers;
    }

    //k - number of neihgbours
    //result - avg of distance Ro (bigger Ro = more unique)
    double getRo(const std::vector<double> &current, const std::vector<std::vector<double>> &all, int k)
    {
        return 1;
    }

}
