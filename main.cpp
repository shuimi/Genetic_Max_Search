#include <iostream>

#include "Main/Subscriptions.h"
#include "Selection/Breeding.h"
#include "Factoring/GenerationFactory.h"
#include "Morphing/Crossover.h"
#include "Morphing/Mutation.h"
#include "Mating.h"

int main() {

    srand(time(NULL));

    double leftBound = 9;
    double rightBound = 14;

    int initialIndividualsAmount = 10;
    int generationsAmount = 50;

    double crossingProbability = 0.7;
    double mutationProbability = 0.3;

    auto fitnessFunction = [](double x){
        return x * x + 0.1 * x - 23;
    };

    auto constraints = new FunctionConstraints<int>(leftBound, rightBound);

    auto condition = [=](double estimationValue) {
        return (estimationValue >= fitnessFunction(constraints->getMean()));
    };

    Generation* initialGeneration;

    std::cout << "INITIAL GENERATION::";

    int dice = rand() % 2;

    if(dice == 0){
        std::cout << "SHOTGUN::\n";
        initialGeneration = GenerationFactory::getUsingShotgun(
            leftBound,
            rightBound,
            initialIndividualsAmount,
            0
        );
    }
    else{
        std::cout << "FOCUSING::\n";
        initialGeneration = GenerationFactory::getUsingFocusing(
            leftBound,
            rightBound,
            initialIndividualsAmount,
            0
        );
    }

    initialGeneration->printout();

    Generation* currentGeneration = initialGeneration->copy();

    auto customEstimation = [=](int decimal){
        return ((double)fitnessFunction(decimal) / (
                fitnessFunction(
                        initialGeneration->getWithMaxEstimation(fitnessFunction)->getDecimal()
                )
        )
        );
    };


    Mating* mating = new Mating(0.7, 0.3);

    std::vector<crossFunction> myCrossOperatorsSet = {
            Crossover::doublePoint,
//            Crossover::CX,
//            Crossover::fibonacci
    };
    std::vector<mutationFunction> myMutationOperatorsSet = {
//            Mutation::swapFibonacci,
//            Mutation::inversion
    };

    std::vector<crossFunction> maxCrossOperatorsSet = {
//            Crossover::PMX,
//            Crossover::OX,
//            Crossover::goldenRatio
    };
    std::vector<mutationFunction> maxMutationOperatorsSet = {
//            Mutation::transpose,
//            Mutation::simple
    };

    mating->add(myCrossOperatorsSet);
    mating->add(myMutationOperatorsSet);

    for(int generationIndex = 0; generationIndex < 50; generationIndex++){
        currentGeneration = mating->execute(currentGeneration);
        currentGeneration = constraints->reduceGenerationToInterval(currentGeneration);
        currentGeneration->makeUnique();
        currentGeneration->printout();
    }


//    Breeding::random(initialGeneration)->printout();
//
//
//    Breeding::inbreeding(
//            initialGeneration, generationB
//    )->printout();
//
//
//    Breeding::inbreeding(
//            initialGeneration, generationB,
//            condition,
//            fitnessFunction
//    )->printout();
//
//
//    Breeding::scaleBased(
//            initialGeneration,
//            customEstimation,
//            0.5
//    )->printout();

//
//    Generation* test = GenerationFactory::getUsingFocusing(10, 10, 15, 0);
//    test->printout();
//    FunctionConstraints<int>(0, 5).reduceGenerationToInterval(test)->printout();

}
