//
// Created by Vladimir on 15.05.2021.
//
#include <functional>

#include "../../Morphers/headers/PopulationFactory.h"

#ifndef GENETIC_ALGS_GENETICSTRAINER_H
#define GENETIC_ALGS_GENETICSTRAINER_H


class EvolutionChamber{
private:

    std::function<double(double)> fitnessFunction;

    Population* initialPopulation;

    Population* currentPopulation;

    int populationSize;
    int generationsAmount;

public:

    EvolutionChamber(const std::function<double(double)> &fitnessFunction) :
    fitnessFunction(fitnessFunction) {}

    EvolutionChamber() {}

    virtual ~EvolutionChamber() {}

    const std::function<double(double)> &getFitnessFunction() const;

    void setFitnessFunction(std::function<double(double)> fitnessFunction);

    const double executeFitnessFunction(double arg);

    //TODO: microevolution

    //TODO: move selection operators to external Selection class
    //TODO: move breeding operators to external Breeding class

    Population*
    selectionElite(Population* population,
                   std::function<bool(double)> selectionCondition);

    static Population*
            selectionElite(Population* population,
                           std::function<double(double)> fitnessFunction,
                           std::function<bool(double)> selectionCondition);

    static Population*
            breedingRandom(Population* population);

    static Population*
            breedingInbreeding(Population* population);

};


#endif //GENETIC_ALGS_GENETICSTRAINER_H
