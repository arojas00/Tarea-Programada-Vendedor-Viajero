// Tarea programada 2 - Genetic Algorithm TSP - Javier Donato - Adrian Rojas

#include <iostream>
#include <cstdlib>
#include <math.h>
#include "GeneticAlgorithm.h"
#include "GeneticAlgorithmCases.cpp"

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
        Nodo(float*** dato, float prioridad){
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
        float tempPrioridad;
        
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
    
    float getPrioridad(int index){
        Nodo*temp = cabeza->siguiente;
        for(int i = 0; i<index; i++){
            temp = temp->siguiente;
        }
        return temp->prioridad;
    }
    
    void pop(){
        Nodo *temp;
        float tempPrioridad;
        float*** tempDato;
        
        temp = cabeza;
        tempDato = temp->dato;
        tempPrioridad = temp->prioridad;
        
        cabeza = cabeza->siguiente;
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
        // constructor de TravellingSalesman
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
        // destructor de TravellingSalesman
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
        // genera una epoca del algoritmo genetico
        void epoch(){
            randomSolutions(sporadicRatio, newGen);
            elitism();
            orderCrossover();
            for(int i = 0; i < populationSize; i++){
                for(int j = 0; j < citiesQuantity; j++){
                    for(int k = 0; k < 2; k++){
                        individuals[i][j][k] = newGen[i][j][k];
                    }
                }
            }
            fitness(individuals);
            currentEpoch++;
//            float*** best = lista->get(0);
//            std::cout << "epoch " << currentEpoch << std::endl;
//            for(int n = 0; n < citiesQuantity; n++){
//                for(int w = 0; w < 2; w++){
//                    std::cout << best[0][n][w] << " ";
//                }
//                std::cout << std::endl;
//            }


        }
        float getBestIndividual(){
            return lista->getPrioridad(0);
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
        // produce soluciones randomizadas para los individuos
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
        // primer intento de un algoritmo para crossover, no funciona correctamente
        void partiallyMappedCcrossover(){
            float*** father;
            float*** mother;
            int randomCut1, randomCut2;
            for (int i = sporadicRatio+elitismRatio; i < populationSize-1; i++) {
                randomCut1 = 0;
                randomCut2 = 0;
                father = selection();
                mother = selection();
                while(randomCut1 >= randomCut2){
                    randomCut1 = rand()%(citiesQuantity-1) + 1;
                    randomCut2 = rand()%(citiesQuantity-1) + 1;
                }
                for(int j = randomCut1 ; j < randomCut2; j++){
                    for(int k = 0 ; k < 2; k++){
                        newGen[i][j][k] = mother[0][j][k];
                        newGen[i+1][j][k] = father[0][j][k];
                    }
                }
                for(int j = 1 ; j < randomCut1; j++){
                    searchDuplicates(i,j,j,randomCut1,randomCut2,father);
                    searchDuplicates(i+1,j,j,randomCut1,randomCut2,mother);
                }
                for(int j = randomCut2 ; j < citiesQuantity; j++){
                    searchDuplicates(i,j,j,randomCut1,randomCut2,father);
                    searchDuplicates(i+1,j,j,randomCut1,randomCut2,mother);
                }
                mutation(i);
                i++;
            }
        }
        // busca si una ciudad ya fue colocada en el hijo para el PMC
        void searchDuplicates(int i,int j,int m,int startCut,int endCut,float*** parent){
            for(int k = startCut ; k < endCut; k++){
                if(parent[0][m][0] != newGen[i][k][0] && parent[0][m][1] != newGen[i][k][1]){
                    newGen[i][j][0] = parent[0][m][0];
                    newGen[i][j][1] = parent[0][m][1];
                }else{
                    searchDuplicates(i,j,k,startCut,endCut,parent);
                    break;
                }
            }
        }
        // segunfo intento de un algoritmo para crossover, no funciona correctamente
        void cycleCrossover(){
            float*** father;
            float*** mother;
            for (int i = sporadicRatio+elitismRatio; i < populationSize-1; i++){
                do{
                    father = selection();
                    mother = selection();
                }while(father == mother);
                int j = 1;
                searchCity(mother[0][j][0],mother[0][j][1],i,j,father,mother);
                i++;
            }
            
        }
        // busca la posicion de una ciudad en el otro padre para generar el CX
        void searchCity(float cityX,float cityY,int i,int j,float*** father,float*** mother){
            newGen[i][j][0] = cityX;
            newGen[i][j][1] = cityY;
            for(int k = 1;k < citiesQuantity;k++){
                if(cityX==father[0][k][0]&&cityY==father[0][k][1]){
                    for(int m = 1;m < citiesQuantity;m++){
                        if(mother[0][k][0]==father[0][m][0]&&mother[0][k][1]==father[0][m][1]){
                            newGen[i+1][j][0] = mother[0][m][0];
                            newGen[i+1][j][1] = mother[0][m][1];
                            if(j+1 < citiesQuantity){
                                for(int p = 1;p < citiesQuantity;p++){
                                    if(mother[0][m][0]==father[0][p][0]&&mother[0][m][1]==father[0][p][1]){
                                        searchCity(mother[0][p][0],mother[0][p][1],i,j+1,father,mother);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        // algoritmo implementado para el crossover, usa un segmento de un padre y sigue el orden del otro padre para completarse
        void orderCrossover(){
            float*** father;
            float*** mother;
            int randomCut1, randomCut2;
            for (int i = sporadicRatio+elitismRatio; i < populationSize-1; i++) {
                randomCut1 = 0;
                randomCut2 = 0;
                do{
                    father = selection();
                    mother = selection();
                }while(father == mother);
                while(randomCut1 >= randomCut2){
                    randomCut1 = rand()%(citiesQuantity-1) + 1;
                    randomCut2 = rand()%(citiesQuantity-1) + 1;
                }
                followSequence(i,father,mother,randomCut1,randomCut2);
                mutation(i);
                i++;
                followSequence(i,mother,father,randomCut1,randomCut2);
                mutation(i);
            }
        }
        // completa el resto del hijo con el orden del otro padre
        void followSequence(int i,float*** parentOne,float*** parentTwo,int randomStart, int randomEnd){
            int index;
            for(int j = randomStart ; j < randomEnd; j++){
                for(int k = 0 ; k < 2; k++){
                    newGen[i][j][k] = parentOne[0][j][k];
                }
            }
            index = randomEnd;
            for(int j = randomEnd ; j < citiesQuantity; j++){
                for(int k = randomStart ; k < randomEnd; k++){
                    if(parentTwo[0][j][0] == parentOne[0][k][0] && parentTwo[0][j][1] == parentOne[0][k][1]){
                        index--;
                        break;
                    }else{
                        newGen[i][index][0] = parentTwo[0][j][0];
                        newGen[i][index][1] = parentTwo[0][j][1];
                    }
                }
                if(index+1 < citiesQuantity){
                    index++;
                }else{
                    index = 1;
                }
            }
            for(int j = 1 ; j < randomEnd; j++){
                if(index!=randomStart){
                    for(int k = randomStart ; k < randomEnd; k++){
                        if(parentTwo[0][j][0] == parentOne[0][k][0] && parentTwo[0][j][1] == parentOne[0][k][1]){
                            index--;
                            break;
                        }else{
                            newGen[i][index][0] = parentTwo[0][j][0];
                            newGen[i][index][1] = parentTwo[0][j][1];
                        }
                    }
                    if(index+1 < citiesQuantity){
                        index++;
                    }else{
                        index = 1;
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
            return padre;
        }
        void swap(int individual, int ciudadUno, int ciudadDos){
            float coorUnoX= newGen[individual][ciudadUno][0];
            float coorUnoY= newGen[individual][ciudadUno][1];
            
            newGen[individual][ciudadUno][0] = newGen[individual][ciudadDos][0];;
            newGen[individual][ciudadUno][1] = newGen[individual][ciudadDos][1];;
            
            newGen[individual][ciudadDos][0] = coorUnoX;
            newGen[individual][ciudadDos][1] = coorUnoY;
        }
    
        void mutation(int individual){
            int probabilidad = mutationRatio;
            int random = rand()%101;
            int seleccionar = rand()%3;
            int diference = citiesQuantity/2;
            int randomStart =0;
            int randomEnd =0;
            if (random < probabilidad) {
                
                do{
                    randomStart = rand()%(citiesQuantity-1) + 1;
                    randomEnd = rand()%(citiesQuantity-1) + 1;
                }while(randomStart >= randomEnd);
                
                int randomCity = 0;
            
                if (seleccionar == 1) {
                    int ciudadUno= rand()%(citiesQuantity-1) + 1;
                    int ciudadDos= rand()%(citiesQuantity-1) + 1;
                    while (ciudadUno == ciudadDos) {
                        ciudadDos= rand()%(citiesQuantity-1) + 1;
                    }
                    swap(individual, ciudadUno, ciudadDos);
                } else if (seleccionar == 2) {
                    for(int j = randomStart; j < randomEnd; j++){
                        randomCity = rand()%(randomEnd-randomStart) + randomStart;
                        swap(individual,j, randomCity);
                    }
                } else if (seleccionar == 0) {
                    while (randomStart < randomEnd) {
                        swap(individual, randomStart, randomEnd);
                        randomStart++;
                        randomEnd--;
                    }
                }
            
            }
        }

};

int main(){
    int citiesQ = 0, index = 0;
    citiesQ = sizeof(caseLAU15)/sizeof(caseLAU15[0]);
    TravellingSalesman* travellingSalesman1 = new TravellingSalesman(caseLAU15,citiesQ,200,10.0f,60.0f,10.0f);
    std::cout << "Ejecutando epocas... " <<  std::endl;
    while(travellingSalesman1->getBestIndividual() > 284.38 ){
    //for(int i = 0; i < 10000; i++){
        travellingSalesman1->epoch();
        index++;
    }
    std::cout << "Caso 1: fitness en generacion " << index << " (" << travellingSalesman1->getBestIndividual() << ")" << std::endl;

    citiesQ = sizeof(caseWG59)/sizeof(caseWG59[0]);
    TravellingSalesman* travellingSalesman2 = new TravellingSalesman(caseWG59,citiesQ,200,5.0f,60.0f,10.0f);
    index = 0;
    std::cout << "Ejecutando epocas... " <<  std::endl;
    while(travellingSalesman2->getBestIndividual() > 1300){
    //for(int i = 0; i <10000; i++){
        travellingSalesman2->epoch();
        index++;
    }
    std::cout << "Caso 2: fitness en generacion " << index << " (" << travellingSalesman2->getBestIndividual() << ")" << std::endl;


    citiesQ = sizeof(caseSGB128)/sizeof(caseSGB128[0]);
    TravellingSalesman* travellingSalesman3 = new TravellingSalesman(caseSGB128,citiesQ,200,10.0f,60.0f,10.0f);
    index = 0;
    std::cout << "Ejecutando epocas... " <<  std::endl;
    while(travellingSalesman3->getBestIndividual() > 23000){
    //for(int i = 0; i <10000; i++){
        travellingSalesman3->epoch();
        index++;
    }
    std::cout << "caso 3: fitness en generacion " << index << " (" << travellingSalesman3->getBestIndividual() << ")" << std::endl;

//    citiesQ = sizeof(caseUSCA312)/sizeof(caseUSCA312[0]);
//    TravellingSalesman* travellingSalesman4 = new TravellingSalesman(caseUSCA312,citiesQ,200,10.0f,60.0f,10.0f);
//    index = 0;
//    std::cout << "Ejecutando epocas... " <<  std::endl;
//    for(int i = 0; i <10000; i++){
//        travellingSalesman4->epoch();
//        index++;
//    }
//    std::cout << "caso 4: fitness en generacion " << index << " (" << travellingSalesman4->getBestIndividual() << ")" << std::endl;
    std::cout << "Finalizado";
    char c;
    std::cin >> c;
    return 0;
}
