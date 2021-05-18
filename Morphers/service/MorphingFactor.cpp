//
// Created by Vladimir on 15.05.2021.
//

#include "MorphingFactor.h"


MorphingFactor::MorphingFactor(double crossProbability, double mutationProbability) :
        crossProbability(crossProbability), mutationProbability(mutationProbability) {}

//TODO: morph method
Generation *MorphingFactor::morph(Generation *generation) {
    return nullptr;
}

/// crossovers
/// crossovers
/// crossovers

Generation *MorphingFactor::crossTwoPoint(BinaryChromosome* parentA, BinaryChromosome* parentB) {

    BinaryChromosome::complementChromosome(parentA, parentB);

    int leftBound = rand() % parentA->get().size();
    int rightBound = rand() % parentA->get().size();
    while (rightBound <= leftBound) rightBound = rand() % parentA->get().size();

    std::vector<bool> childA, childB, childC, childD, childE, childF;

    for (int i = 0; i < leftBound; i++) {
        childA.push_back(parentA->get().at(i));
        childB.push_back(parentA->get().at(i));
        childC.push_back(parentA->get().at(i));
        childD.push_back(parentB->get().at(i));
        childE.push_back(parentB->get().at(i));
        childF.push_back(parentB->get().at(i));
    }
    for (int i = leftBound; i < rightBound; i++) {
        childA.push_back(parentA->get().at(i));
        childB.push_back(parentB->get().at(i));
        childC.push_back(parentB->get().at(i));
        childD.push_back(parentA->get().at(i));
        childE.push_back(parentA->get().at(i));
        childF.push_back(parentB->get().at(i));
    }
    for (int i = rightBound; i < parentA->get().size(); i++) {
        childA.push_back(parentB->get().at(i));
        childB.push_back(parentA->get().at(i));
        childC.push_back(parentB->get().at(i));
        childD.push_back(parentA->get().at(i));
        childE.push_back(parentB->get().at(i));
        childF.push_back(parentA->get().at(i));
    }

    std::vector<BinaryChromosome*> children = {
            new BinaryChromosome(childA), new BinaryChromosome(childB), new BinaryChromosome(childC),
            new BinaryChromosome(childD), new BinaryChromosome(childE), new BinaryChromosome(childF)
    };
    return new Generation(children);
}

Generation *MorphingFactor::crossFibonacci(BinaryChromosome* parentA, BinaryChromosome* parentB) {

    BinaryChromosome::complementChromosome(parentA, parentB);

    std::vector<int>
            fibNumbers = getFibonacciVector(parentA->getSize());
    std::vector<std::vector<bool>>
            binaryTable = makeBinaryNumbersTable(parentA->getSize()); //A->0, B->1

    std::vector<BinaryChromosome*>
            partsParentA = makeParentParts(parentA, fibNumbers.size());
    std::vector<BinaryChromosome*>
            partsParentB = makeParentParts(parentB, fibNumbers.size());

    std::vector<BinaryChromosome *> children;

    for(int i = 1; i < binaryTable.size() - 1; i++) {
        BinaryChromosome *child = new BinaryChromosome();

        for(int j = 0; j < parentA->getSize() - 1; j++) {
            if (!binaryTable[i][j]) child->glue(partsParentA.at(j));
            else child->glue(partsParentB.at(j));
        }

        if (children.empty()) children.push_back(child);

        bool childAdded = false;

        for(int k = 0; k < children.size(); k++) {
            for (int j = 0; j < child->getSize(); j++){

                if (children.at(k)->getGen(j) != child->getGen(j)) {
                    children.push_back(child);
                    childAdded = true;
                    break;
                }

            }
            if (childAdded) break;
        }
    }

    return new Generation(children);
}

std::vector<int> MorphingFactor::getFibonacciVector(int numbersAmount) {
    std::vector<int> output;
    for(int i = 0; i < numbersAmount; i++){
        output.push_back(MorphingFactor::fibonacci(i));
    }
    return output;
}

std::vector<std::vector<bool>> MorphingFactor::makeBinaryNumbersTable(int n) {
    int rowsAmount = pow(2, n);
    std::vector<std::vector<bool>> table;
    for (int i = 0; i < rowsAmount; i++) {
        std::vector<bool> row;
        int temp_i = i;
        do {
            row.push_back(temp_i % 2);
            temp_i /= 2;
        } while (temp_i > 0);

        while (row.size() < n)
            row.push_back(0);

        std::reverse(row.begin(), row.end());
        table.push_back(row);
    }
    return table;
}

std::vector<BinaryChromosome *> MorphingFactor::makeParentParts(BinaryChromosome *parent, int n) {
    int t = 1;
    while (n > fibonacci(t)) { t++; }
    std::vector<int> fibNumbers = getFibonacciVector(t);
    std::vector<BinaryChromosome *> parentParts;

    //separately push first(left) part
    parentParts.push_back(new BinaryChromosome(std::vector<bool>(1, parent->get().at(0))));

    for (int i = 0; i < fibNumbers.size() - 1; i++) {
        BinaryChromosome *tempChromosome = parent->getSubsequence(fibNumbers.at(i), fibNumbers.at(i + 1) - 1);
        parentParts.push_back(tempChromosome);
    }

    //separately push last(right) part
    BinaryChromosome *tempChromosome = parent->getSubsequence(fibNumbers.back(), parent->get().size() - 1);
    if (!tempChromosome->get().empty()) parentParts.push_back(tempChromosome);

    return parentParts;
}

/// mutations
/// mutations
/// mutations

BinaryChromosome *MorphingFactor::mutateSimple(BinaryChromosome *individual) {
    int index = std::rand() % individual->getSize();
    individual->setGen(index, !individual->getGen(index));
    return individual;
}

BinaryChromosome *MorphingFactor::mutateInversion(BinaryChromosome *individual) {
    int startIndex = std::rand() % individual->getSize();
    int endIndex = std::rand() % individual->getSize();
    while (startIndex >= endIndex) endIndex = std::rand() % individual->getSize();
    individual->reverse(startIndex, endIndex);
    return individual;
}

BinaryChromosome *MorphingFactor::mutateSwapFibonacci(BinaryChromosome *individual) {
    int a = MorphingFactor::fibonacci(std::rand());
    int b = MorphingFactor::fibonacci(std::rand());
    while (b == a) b = MorphingFactor::fibonacci(std::rand());
    individual->swap(a, b);
    return individual;
}

BinaryChromosome *MorphingFactor::mutateTranspose(BinaryChromosome *individual) {
    int startIndex = std::rand() % individual->getSize();
    int endIndex = std::rand() % individual->getSize();
    while (startIndex >= endIndex) endIndex = std::rand() % individual->getSize();
    BinaryChromosome* erased = individual->erase(startIndex, endIndex);
    individual->insert(std::rand() % individual->getSize(), erased);
    return individual;
}
