#include <vector>
#include <set>
#include <cmath>
#include <random>

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
    }

    int generateRandomInt(int minIncluded, int maxIncluded)
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(minIncluded, maxIncluded);
        return uni(rng);
    }

    double generateRandomDouble(double minIncluded, double maxIncluded)
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_real_distribution<double> unif(minIncluded, maxIncluded);
        return unif(rng);
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
        std::multiset<double, std::less<double>> euclideanSet;

        double sum = 0;

        for (size_t i = 0; i < all.size(); i++)
        {
            euclideanSet.insert(getEuclideanDistance(current, all[i]));
        }

        std::multiset<double>::iterator it = euclideanSet.begin();
        //begin from 1 - always ignore the first zero distance (self distance)
        for (int i = 0; i <= k; ++i)
        {
            sum += *it++;
        }

        if (k != 0)
        {
            return sum / k;
        }
        else
            return 0;
    }

}
