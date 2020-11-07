#include <iostream>
#include <cstdlib>
#include <math.h>
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
    unsigned int tamano;
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
        tamano=0;
    }
    unsigned int getTamano(){
        return tamano;
    }
    void push (float*** d, int p){
        ultimo ->siguiente = new Nodo(d, p);
        ultimo = ultimo->siguiente;
        tamano++;
        ordenar();
        
    }
    
    float*** get(int index){
        Nodo*temp = cabeza->siguiente;
        for(int i = 0; i<index; i++){
            temp = temp->siguiente;
        }
        return temp->dato;
    }
    
    int getPrioridad(int index){
        Nodo*temp = cabeza->siguiente;
        for(int i = 0; i<index; i++){
            temp = temp->siguiente;
        }
        return temp->prioridad;
    }
    
    void pop(){
        Nodo *temp;
        int tempPrioridad;
        float*** tempDato;
        
        temp = cabeza;
        tempDato = temp->dato;
        tempPrioridad = temp->prioridad;
        
        cabeza = cabeza->siguiente;
        //delete temp;
        tamano=tamano-1;
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
        float** cities;
        float*** individuals;
        float*** newGen;
        Cola* lista;
    public:
        TravellingSalesman(float matrix[][2],int citiesQ,int populationSize,float elitismRatio,float mutationRatio,float sporadicRatio) : GeneticAlgorithm(populationSize, elitismRatio, mutationRatio, sporadicRatio){
            citiesQuantity = citiesQ;
            lista = new Cola();
            cities = new float*[citiesQuantity];
            for(int i = 0; i < citiesQuantity; i++){
                cities[i] = new float [2];
                for(int j = 0; j < 2; j++){
                    cities[i][j] = matrix[i][j];
                }
            }
            individuals = new float**[populationSize];
            for(int i = 0; i < populationSize; i++){
                individuals[i] = new float*[citiesQuantity];
                for(int j = 0; j < citiesQuantity; j++){
                    individuals[i][j] = new float [2];
                    for(int k = 0; k < 2; k++){
                        individuals[i][j][k] = cities[j][k];
                    }
                }
            }
            newGen = new float**[populationSize];
            for(int i = 0; i < populationSize; i++){
                newGen[i] = new float*[citiesQuantity];
                for(int j = 0; j < citiesQuantity; j++){
                    newGen[i][j] = new float [2];
                    for(int k = 0; k < 2; k++){
                        newGen[i][j][k] = cities[j][k];
                    }
                }
            }
            randomSolutions(populationSize, individuals);
            fitness(individuals);
        }
        ~TravellingSalesman(){
            for(int i = 0; i < populationSize; i++){
                for(int j = 0; j < citiesQuantity; j++){
                    delete[] newGen[i][j];
                }
                delete[] newGen[i];
            }
            delete[] newGen;
            for(int i = 0; i < populationSize; i++){
                for(int j = 0; j < citiesQuantity; j++){
                    delete[] individuals[i][j];
                }
                delete[] individuals[i];
            }
            delete[] individuals;
            for(int i = 0; i < citiesQuantity; i++){
                delete[] cities[i];
            }
            delete[] cities;
            delete lista;
        }
        void reset(int populationSize,float elitismRatio,float mutationRatio,float sporadicRatio){
            GeneticAlgorithm::reset(populationSize, elitismRatio, mutationRatio, sporadicRatio);
        }
        void epoch(){
            randomSolutions(sporadicRatio, newGen);
            elitism();
            
            //selection,crossover,mutation
            mutation(26);
            for(int i = 0; i < populationSize; i++){
                std::cout << "induvidual " << i << std::endl;
                for(int j = 0; j < citiesQuantity; j++){
                    for(int k = 0; k < 2; k++){
                        std::cout << newGen[i][j][k] << " ";
                    }
                    std::cout << std::endl;
                }
            }
            //selection();
        }
        float getBestIndividual(){
            return lista->getPrioridad(0);;
        }
        void drawBestIndividual(float** points,unsigned int &numPoints){
            numPoints = citiesQuantity;
            float*** best = lista->get(0);
            
            for (int i =0; i<numPoints; i++) {
                for (int j=0; j<1; i++) {
                    points[i][j] = best[0][i][j];
                    std::cout<<points[i][j];
                }
            }
            points[citiesQuantity+1][0] = best[0][0][0];
            std::cout<<points[citiesQuantity+1][0];
            points[citiesQuantity+1][1] = best[0][0][1];
            std::cout<<points[citiesQuantity+1][0];
        }
        void randomSolutions(int size, float*** matrix){
            int randNum = 0;
            float temp = 0.0;
            for(int i = 0; i < size; i++){
                for(int j = 1; j < citiesQuantity; j++){
                    randNum = rand()%(citiesQuantity-1) + 1;
                    for(int k = 0; k < 2; k++){
                        temp = matrix[i][j][k];
                        matrix[i][j][k] = matrix[i][randNum][k];
                        matrix[i][randNum][k] = temp;
                    }
                }
            }
        }
        void fitness(float*** matriz){
            
            while (lista->getTamano() > 0) {
                lista->pop();
            }
            
            float coorX;
            float coorY;
            float funcionX;
            float funcionY;
               
            
            for (int p=0; p<populationSize; p++) {
                float recorrido = 0.0;
                float*** punteroIndiv = &matriz[p];
                
                float ciudadUnoX = matriz[p][0][0];
                float ciudadUnoY = matriz[p][0][1];
                for (int x=1; x<=citiesQuantity; x++) {
                   if (x==citiesQuantity) {
                       coorY = matriz[p][0][1];
                       coorX = matriz[p][0][0];
                       
                       funcionX = pow((coorX-ciudadUnoX), 2);
                       funcionY = pow ((coorY-ciudadUnoY), 2);
                       recorrido = recorrido + sqrt(funcionX+funcionY);
                       
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
        void elitism(){
            float*** temp;
            int index = sporadicRatio;
            for(int i = 0; i < elitismRatio; i++){
                temp = lista->get(i);
                for(int j = 0; j < citiesQuantity; j++){
                    for(int k = 0; k < 2; k++){
                        newGen[index+i][j][k] = temp[0][j][k];
                    }
                }
            }
        }
        float*** selection(){
            int randomUno = rand()%101;
            int randomDos =0;
            
            if (randomUno >= 35) {
                randomDos = rand()%10;
            } else if (35< randomUno && randomUno<=55){
                randomDos =10+rand()%(20-10);
            } else if (55< randomUno && randomUno<=71){
                randomDos =20+rand()%(30-20);
            }else if (71< randomUno && randomUno<=79){
                randomDos =30+rand()%(40-30);
            } else if(79< randomUno && randomUno<=85){
                randomDos =40+rand()%(50-40);
            } else if(85< randomUno && randomUno<=90){
                randomDos =50+rand()%(60-50);
            } else if(90< randomUno && randomUno<=94){
                randomDos =60+rand()%(70-60);
            }else if (94< randomUno && randomUno<=97){
                randomDos =70+rand()%(80-70);
            }else if(97< randomUno && randomUno<=99){
                randomDos =80+rand()%(90-80);
            } else if(99< randomUno && randomUno<=100){
                randomDos =90+rand()%(100-90);
            }

            
            float*** padre = lista->get(randomDos);
            std::cout<< "Padre: "<< padre << std::endl;
            return padre;
        }
    
    
    
        void crossover(){
            int index = elitismRatio + sporadicRatio;
            float*** padre;
            float*** madre;
            
            for (int i= index; i < lista->getTamano(); i++) {
                padre = selection();
                madre = selection();
            }
            
        }
        void mutation(int individual){
            int probabilidad = mutationRatio;
            int random = rand()%101;
            
            int ciudadUno= 1+rand()%((citiesQuantity+1)-1);
            int ciudadDos= 1+rand()%((citiesQuantity+1)-1);
            
            while (ciudadUno == ciudadDos) {
                ciudadDos= 1+rand()%((citiesQuantity+1)-1);
            }
            
            if (random < probabilidad) {
                float coorUnoX= newGen[individual][ciudadUno][0];
                float coorUnoY= newGen[individual][ciudadUno][1];
                
                
                newGen[individual][ciudadUno][0] = newGen[individual][ciudadDos][0];;
                newGen[individual][ciudadUno][1] = newGen[individual][ciudadDos][1];;
                
                newGen[individual][ciudadDos][0] = coorUnoX;
                newGen[individual][ciudadDos][1] = coorUnoY;
            }
            
            
        }

};

int main(){
    float matrix[15][2] = {
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
    int citiesQ = sizeof(matrix)/sizeof(matrix[0]);
    TravellingSalesman* travellingSalesman = new TravellingSalesman(matrix,citiesQ,100,15.0f,30.0f,10.0f);
    travellingSalesman->epoch();
    std::cout<<travellingSalesman->getBestIndividual();
    char c;
    std::cin >> c;
    return 0;
}
