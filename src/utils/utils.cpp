#include <vector>
#include <set>
#include <cmath>
#include <random>
#include <assert.h>

namespace utils
{
    namespace
    {
        double getEuclideanDistance(const std::vector<double> &from, const std::vector<double> &to)
        {
            double sum = 0;
            for (size_t i = 0; i < from.size(); i++)
            {
                sum += pow((to[i] - from[i]), 2);
            }
            return sqrt(sum);
        }

        double getEuclideanDistance(double from, double to)
        {
            return std::abs(to - from);
        }
    }


    std::random_device rd;
    std::mt19937 rng(rd());

    int generateRandomInt(int minIncluded, int maxIncluded, std::mt19937 &rn = rng)
    {
        std::uniform_int_distribution<int> uni(minIncluded, maxIncluded);
        return uni(rn);
    }

    double generateRandomDouble(double minIncluded, double maxIncluded)
    {
        std::uniform_real_distribution<double> unif(minIncluded, maxIncluded);
        return unif(rng);
    }

    std::vector<double> generateRandomRange(size_t size, double min, double max)
    {
        std::vector<double> rndNumbers;

        for (size_t index = 0; index < size; index++)
        {
            rndNumbers.push_back(generateRandomDouble(min, max));
        }

        return rndNumbers;
    }

    //k - number of neihgbours
    //result - avg of distance Ro (bigger Ro = more unique)
    double getRo(const std::vector<double> &current, const std::vector<std::vector<double>> &all, size_t k)
    {
        if (k == 0)
            return 0;

        assert(k <= all.size());
        std::multiset<double, std::less<double>> euclideanSet;

        double sum = 0;

        for (auto other : all)
        {
            euclideanSet.insert(getEuclideanDistance(current, other));
        }

        std::multiset<double>::iterator it = euclideanSet.begin();
        //begin from 1 - always ignore the first zero distance (self distance)
        for (size_t i = 0; i < k; ++i)
        {
            sum += *++it;
        }

        return sum / k;
    }

    //k - number of neihgbours
    //d - dimension
    //result - avg of distance Ro (bigger Ro = more unique)
    double getRoOneDimension(const std::vector<double> &current, const std::vector<std::vector<double>> &all, size_t k, int d)
    {
        if (k == 0)
            return 0;

        assert(k <= all.size());
        std::multiset<double, std::less<double>> euclideanSet;

        double sum = 0;

        for (auto other: all)
        {
            euclideanSet.insert(getEuclideanDistance(current[d], other[d]));
        }

        std::multiset<double>::iterator it = euclideanSet.begin();
        //begin from 1 - always ignore the first zero distance (self distance)
        for (size_t i = 0; i < k; ++i)
        {
            sum += *++it;
        }

        return sum / k;
    }

}
