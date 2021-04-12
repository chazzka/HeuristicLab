#include <vector>
#include <iostream>
#include <random>
#include "../cec20_test_func.cpp"

struct jedinec
{
    int id;
    std::vector<double> position;
    double cost;
};

struct ohodnocenaPopulace
{
    jedinec leader;
    std::vector<jedinec> populace;
};

struct result
{
    int fez;
    double cost;
};

ohodnocenaPopulace getLeader(std::vector<jedinec> population, int testFunction, int dimension)
{
    double best_cost = 0;
    cec20_test_func(population[0].position.data(), &best_cost, dimension, 1, testFunction);

    jedinec tempLeader = population[0];
    //toto zlepšilo to, že leader nebude 0
    tempLeader.cost = best_cost;

    for (int i = 0; i < population.size(); i++)
    {
        //        double actual_cost = first_dejong(population[i].position);
        double actual_cost = 0;
        cec20_test_func(population[i].position.data(), &actual_cost, dimension, 1, testFunction);
        population[i].cost = actual_cost;
        if (actual_cost < best_cost)
        {
            best_cost = actual_cost;
            tempLeader = population[i];
        }
    }

    return {tempLeader, population};
}

std::vector<double> generateRandom(int size, double min, double max)
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

std::vector<double> vec_subtract(std::vector<double> first, std::vector<double> second)
{
    std::vector<double> result;
    for (int i = 0; i < first.size(); i++)
    {
        result.push_back(first[i] - second[i]);
    }
    return result;
}

std::vector<double> vec_add(std::vector<double> first, std::vector<double> second)
{
    std::vector<double> result;
    for (int i = 0; i < first.size(); i++)
    {
        result.push_back(first[i] + second[i]);
    }
    return result;
}

std::vector<double> vec_multiply_with_cislo(std::vector<double> first, double cislo)
{
    std::vector<double> result;
    for (int i = 0; i < first.size(); i++)
    {
        result.push_back(first[i] * cislo);
    }
    return result;
}

std::vector<double> vec_multiply_with_vector(std::vector<double> first, std::vector<double> second)
{
    std::vector<double> result;
    for (int i = 0; i < first.size(); i++)
    {
        result.push_back(first[i] * second[i]);
    }
    return result;
}


//nedojede do 50k, protože je omezeny migracemi
std::vector<result> run(int dimension, int testFunction, int boundaryLow, int boundaryUp)
{

    std::vector<result> best_results;
    double t = 0;
    int path_length = 3;
    double step = 0.33;
    double prt = 0.3;
    int d = dimension;
    int pop_size = 50; //dycky pade
    int pocet_accepted_fezu = 5000 * d;
    int migrace = 50;

    int konec = 0;
    int fezcounter = 0;
    //22050 fezů
    std::vector<jedinec> populace;
    for (int i = 0; i < pop_size; i++)
    {

        std::vector<double> pozice;
        for (double prvek : generateRandom(d, boundaryLow, boundaryUp))
        {
            pozice.push_back(prvek);
        }
        jedinec tmpJedinec = {i, pozice, 0};
        populace.push_back(tmpJedinec);
    }

    ohodnocenaPopulace resPo = getLeader(populace, testFunction, dimension);
    jedinec leader = resPo.leader;
    populace = resPo.populace;

    //migrace

    for (int i_m = 0; i_m < migrace; i_m++)
    {
        jedinec leader_of_population = leader;
        for (jedinec jedinec : populace)
        {
            if (konec == 1)
            {
                std::cout << "konec" << std::endl;
                continue;
            }

            if (jedinec.id != leader_of_population.id)
            {
                std::vector<double> potencial_position = jedinec.position;
                t = 0;
                double actual_cost = jedinec.cost;
                int countera = 0;
                //tady jsem to o 0.2 zmenšil aby to jelo jen 9x a ne 10x
                while (t < path_length - 0.2)
                {
                    countera++;

                    std::vector<double> PRTVector = generateRandom(dimension, 0, 1);

                    for (int i_p = 0; i_p < PRTVector.size(); i_p++)
                    {
                        if (PRTVector[i_p] > prt)
                        {
                            PRTVector[i_p] = 0;
                        }
                        else
                        {
                            PRTVector[i_p] = 1;
                        }
                    }

                    potencial_position = vec_add(potencial_position, vec_multiply_with_vector(vec_multiply_with_cislo(vec_subtract(leader.position, potencial_position), t), PRTVector));
                    //potencial position dobrý

                    //vrat do dimenzi - check OK
                    for (int ip = 0; ip < potencial_position.size(); ip++)
                    {
                        if (boundaryLow > potencial_position[ip] || potencial_position[ip] > boundaryUp)
                        {
                            std::vector<double> randomNUm = generateRandom(1, boundaryLow, boundaryUp);
                            potencial_position[ip] = randomNUm[0];
                        }
                    }

                    //zjisti zda se vylepsil
                    double potencial_cost = 0;
                    cec20_test_func(potencial_position.data(), &potencial_cost, dimension, 1, testFunction);

                    fezcounter++;
                    result res;
                    res.fez = fezcounter;
                    res.cost = leader.cost;
                    best_results.push_back(res);

                    if (fezcounter > pocet_accepted_fezu)
                    {
                        konec = 1;
                        std::cout << "break fez";
                        //todo: tady asi spis return
                        break;
                    }

                    if (potencial_cost < actual_cost)
                    {
                        if (potencial_cost < leader.cost)
                        {
                            leader_of_population.position = potencial_position;
                            leader_of_population.cost = potencial_cost;
                        }
                        jedinec.cost = potencial_cost;
                        jedinec.position = potencial_position;
                    }
                    t += step;
                }
            }
            leader = leader_of_population;
        }
    }

    return best_results;
}
