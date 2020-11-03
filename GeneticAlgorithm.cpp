#include <iostream>
#include <cstdlib>
#include "GeneticAlgorithm.h"



class Cola {
private:
    class Nodo{
        public:
        Nodo* siguiente;
        float*** dato;
        float prioridad;
        Nodo(){
            siguiente=0;
        }
        Nodo(float*** dato, int prioridad){
            this->dato = dato;
            siguiente =0;
            this -> prioridad =prioridad;
        }
        ~Nodo(){
            if (siguiente != 0) {
                delete siguiente;
            }
        }
    };
    Nodo *cabeza, *ultimo;
    unsigned int tamaño;
public:
    
    void ordenar(){
        Nodo *temp ;
        Nodo *temp2 ;
        float*** tempDato;
        int tempPrioridad;
        
        temp = cabeza->siguiente;
        
        while (temp->siguiente != NULL) {
            temp2 = temp -> siguiente;
            while (temp2 != NULL) {
                if (temp->prioridad >= temp2 ->prioridad) {
                    tempDato = temp->dato;
                    tempPrioridad = temp -> prioridad;
                    
                    temp->dato = temp2->dato;
                    temp->prioridad =temp2->prioridad;
                    
                    temp2 ->dato =tempDato;
                    temp2->prioridad =tempPrioridad;

                }
                temp2 = temp2 -> siguiente;
            }
            temp = temp->siguiente;
        }
    }
    Cola(){
        cabeza= new Nodo();
        ultimo= cabeza;
        tamaño=0;
    }
    unsigned int getTamaño(){
        return tamaño;
    }
    void push (float*** d, int p){
     ultimo ->siguiente = new Nodo(d, p);
    ultimo = ultimo->siguiente;
        tamaño++;
       ordenar();
        
    }
};

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
    private:
        int citiesQuantity;
        float cities[][2];
    public:
        TravellingSalesman(int populationSize,float elitismRatio,float mutationRatio,float sporadicRatio) : GeneticAlgorithm(populationSize, elitismRatio, mutationRatio, sporadicRatio){
            float cities[15][2] = {
                {0.0,        0.0},
                {-28.8733,    0.0},
                {-79.2916,    -21.4033},
                  {-14.6577,    -43.3896},
                  {-64.7473,     21.8982},
                  {-29.0585,    -43.2167},
                  {-72.0785,    0.181581},
                  {-36.0366,    -21.6135},
                  {-50.4808,     7.37447},
                  {-50.5859,    -21.5882},
                 {-0.135819,    -28.7293},
                  {-65.0866,    -36.0625},
                  {-21.4983,     7.31942},
                  {-57.5687,    -43.2506},
                  {-43.0700,     14.5548}
                };
            citiesQuantity = sizeof(cities)/sizeof(cities[0]);
            for(int i = 0; i < citiesQuantity; i++){
                for(int j = 0; j < 2; j++){
                    std::cout << cities[i][j] << " ";
                }
                std::cout << std::endl;
            }
            solutions(cities, populationSize, citiesQuantity);
        }
        ~TravellingSalesman(){
        }
        void reset(int populationSize,float elitismRatio,float mutationRatio,float sporadicRatio){
            GeneticAlgorithm::reset(populationSize, elitismRatio, mutationRatio, sporadicRatio);
        }
        void epoch(){
            
        }
        float getBestIndividual(){
            return 1;
        }
        void drawBestIndividual(float** points,unsigned int &numPoints){
            
        }
        void solutions(float cities[][2], int populationSize, int citiesQuantity){
            int randNum = 0;
            float temp = 0;
            float*** individuals = new float**[populationSize];
            for(int i = 0; i < populationSize; i++){
                individuals[i] = new float*[citiesQuantity];
                for(int j = 0; j < citiesQuantity; j++){
                    individuals[i][j] = new float [2];
                }
            }
            for(int i = 0; i < populationSize; i++){
                for(int j = 0; j < citiesQuantity; j++){
                    for(int k = 0; k < 2; k++){
                        individuals[i][j][k] = cities[j][k];
                    }
                }
            }
            for(int i = 0; i < populationSize; i++){
                for(int j = 1; j < citiesQuantity; j++){
                    randNum = rand()%(citiesQuantity-1) + 1;
                    for(int k = 0; k < 2; k++){
                        temp = individuals[i][j][k];
                        individuals[i][j][k] = individuals[i][randNum][k];
                        individuals[i][randNum][k] = temp;
                    }
                }
            }
            for(int i = 0; i < populationSize; i++){
                std::cout <<"induvidual "<< i << std::endl;
                std::cout << individuals[i][0][0] << " " << individuals[i][0][1] << std::endl;
                for(int j = 1; j < citiesQuantity; j++){
                    for(int k = 0; k < 2; k++){
                        std::cout << individuals[i][j][k] << " ";
                    }
                std::cout << std::endl;
                }
            }
            fitness(individuals);
        }
        void swap(float cities[][2], float*** individuals){
                
        }
        void selection(){}
        void crossover(){}
        void mutation(){}
    
        void fitness(float*** matriz){
            Cola* lista = new Cola();
               float coorX;
               float coorY;
               float funcionX;
               float funcionY;
               
            
            for (int p=0; p<populationSize; p++) {
                float recorrido = 0.0;
                float*** punteroIndiv = &matriz[p];
                std::cout<< "puntero: "<< punteroIndiv;
                std::cout << std::endl;
                
                float ciudadUnoX = matriz[p][0][0];
                float ciudadUnoY = matriz[p][0][1];
               for (int x=1; x<=citiesQuantity; x++) {
                   if (x==citiesQuantity) {
                       coorY = matriz[p][0][1];
                       coorX = matriz[p][0][0];
                       
                       funcionX = pow((coorX-ciudadUnoX), 2);
                       funcionY = pow ((coorY-ciudadUnoY), 2);
                       recorrido = recorrido + sqrt(funcionX+funcionY);
                       std::cout<<"recorrido: "<<recorrido;
                       std::cout << std::endl;
                       
                       lista->push(punteroIndiv, recorrido);
                   }else{
                   
                   coorX = matriz[p][x][0];
                   coorY = matriz[p][x][1];
                   
                   funcionX = pow((coorX-ciudadUnoX), 2);
                   funcionY = pow ((coorY-ciudadUnoY), 2);
                   recorrido = recorrido + sqrt(funcionX+funcionY);
                   
                   ciudadUnoY = coorY;
                   ciudadUnoX = coorX;
                   }
               }
           }
        }

};

int main(){
    float caseLAU15[15][2] = {
    {0.0,        0.0},
    {-28.8733,    0.0},
    {-79.2916,    -21.4033},
      {-14.6577,    -43.3896},
      {-64.7473,     21.8982},
      {-29.0585,    -43.2167},
      {-72.0785,    0.181581},
      {-36.0366,    -21.6135},
      {-50.4808,     7.37447},
      {-50.5859,    -21.5882},
     {-0.135819,    -28.7293},
      {-65.0866,    -36.0625},
      {-21.4983,     7.31942},
      {-57.5687,    -43.2506},
      {-43.0700,     14.5548}
    };
    //int citiesQ = sizeof(caseLAU15)/sizeof(caseLAU15[0]);
    TravellingSalesman* travellingSalesman = new TravellingSalesman(5,2.0f,3.0f,4.0f);
    char c;
    std::cin >> c;
    return 0;
}

