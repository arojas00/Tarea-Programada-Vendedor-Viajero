#include <iostream>
#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(int populationSize,float elitismRatio,float mutationRatio,float sporadicRatio){
    GeneticAlgorithm::populationSize = populationSize;
    GeneticAlgorithm::elitismRatio = elitismRatio;
    GeneticAlgorithm::mutationRatio = mutationRatio;
    GeneticAlgorithm::sporadicRatio = sporadicRatio;
}
GeneticAlgorithm::~GeneticAlgorithm(){
}
unsigned int GeneticAlgorithm::getEpoch(){
    return currentEpoch;
}
void GeneticAlgorithm::reset(int populationSize,float elitismRatio,float mutationRatio,float sporadicRatio){
    GeneticAlgorithm::currentEpoch = 1;
    GeneticAlgorithm::populationSize = populationSize;
    GeneticAlgorithm::elitismRatio = elitismRatio;
    GeneticAlgorithm::mutationRatio = mutationRatio;
    GeneticAlgorithm::sporadicRatio = sporadicRatio;
}

main(){
    char c;
    std::cin >> c;
    return 0;
}
