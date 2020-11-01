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

class TravellingSalesman : public GeneticAlgorithm{
    public:
        TravellingSalesman(int populationSize,float elitismRatio,float mutationRatio,float sporadicRatio) : GeneticAlgorithm(populationSize, elitismRatio, mutationRatio, sporadicRatio){
        }
        ~TravellingSalesman(){
        }
        void reset(int populationSize,float elitismRatio,float mutationRatio,float sporadicRatio){
            GeneticAlgorithm::reset(populationSize, elitismRatio, mutationRatio, sporadicRatio);
        }
    	void epoch(){
            
        }
        float getBestIndividual(){
            
        }
    	void drawBestIndividual(float** points,unsigned int &numPoints){
            
        }
        void selection(){}
        void crossover(){}
        void mutation(){}
        void fitness(){}
};

main(){
    TravellingSalesman* travellingSalesman = new TravellingSalesman(1,2.0f,3.0f,4.0f);
    char c;
    std::cin >> c;
    return 0;
}
