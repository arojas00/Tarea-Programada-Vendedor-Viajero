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
            crossover3();
            for(int i = 0; i < populationSize; i++){
                //std::cout<<"individual "<<i<<std::endl;
                for(int j = 0; j < citiesQuantity; j++){
                    for(int k = 0; k < 2; k++){
                        individuals[i][j][k] = newGen[i][j][k];
                        //std::cout<<newGen[i][j][k]<<" ";
                    }
                    //std::cout<<std::endl;
                }
            }
            fitness(individuals);
            std::cout<<getBestIndividual()<<std::endl;
            currentEpoch++;
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
        void crossover(){
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
        void crossover2(){
            float*** father;
            float*** mother;
            for (int i = sporadicRatio+elitismRatio; i < populationSize-1; i++){
                do{
                    father = selection();
                    mother = selection();
                }while(father == mother);
                std::cout<<"father "<<father<<std::endl;
                for(int j = 0; j < citiesQuantity; j++){
                    std::cout<<j<<" ";
                    for(int k = 0; k < 2; k++){
                        std::cout<<father[0][j][k]<<" ";
                    }
                    std::cout<<std::endl;
                }
                std::cout<<"mother "<<mother<<std::endl;
                for(int j = 0; j < citiesQuantity; j++){
                    std::cout<<j<<" ";
                    for(int k = 0; k < 2; k++){
                        std::cout<<mother[0][j][k]<<" ";
                    }
                    std::cout<<std::endl;
                }
                int j = 1;
                searchCity(mother[0][j][0],mother[0][j][1],i,j,father,mother);
                std::cout<<"individual "<<i<<std::endl;
                for(int j = 0; j < citiesQuantity; j++){
                    std::cout<<j<<" ";
                    for(int k = 0; k < 2; k++){
                        std::cout<<newGen[i][j][k]<<" ";
                    }
                    std::cout<<std::endl;
                }
                i++;
                std::cout<<"individual "<<i<<std::endl;
                for(int j = 0; j < citiesQuantity; j++){
                    std::cout<<j<<" ";
                    for(int k = 0; k < 2; k++){
                        std::cout<<newGen[i][j][k]<<" ";
                    }
                    std::cout<<std::endl;
                }
            }
            
        }
        void crossover3(){
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
                orderCrossover(i,father,mother,randomCut1,randomCut2);
                /*std::cout<<"father "<<father<<std::endl;
                for(int j = 0; j < citiesQuantity; j++){
                    std::cout<<j<<" ";
                    for(int k = 0; k < 2; k++){
                        std::cout<<father[0][j][k]<<" ";
                    }
                    std::cout<<std::endl;
                }
                std::cout<<"mother "<<mother<<std::endl;
                for(int j = 0; j < citiesQuantity; j++){
                    std::cout<<j<<" ";
                    for(int k = 0; k < 2; k++){
                        std::cout<<mother[0][j][k]<<" ";
                    }
                    std::cout<<std::endl;
                }
                std::cout<<"start "<<randomCut1<<std::endl;
                std::cout<<"end "<<randomCut2<<std::endl;
                std::cout<<"individual "<<i<<std::endl;
                for(int j = 0; j < citiesQuantity; j++){
                    std::cout<<j<<" ";
                    for(int k = 0; k < 2; k++){
                        std::cout<<newGen[i][j][k]<<" ";
                    }
                    std::cout<<std::endl;
                }*/
                mutation(i);
                i++;
                orderCrossover(i,mother,father,randomCut1,randomCut2);
                /*std::cout<<"individual "<<i<<std::endl;
                for(int j = 0; j < citiesQuantity; j++){
                    std::cout<<j<<" ";
                    for(int k = 0; k < 2; k++){
                        std::cout<<newGen[i][j][k]<<" ";
                    }
                    std::cout<<std::endl;
                }*/
                mutation(i);
            }
            
        }
        void orderCrossover(int i,float*** parentOne,float*** parentTwo,int randomStart, int randomEnd){
            int index;
            for(int j = randomStart ; j < randomEnd; j++){
                for(int k = 0 ; k < 2; k++){
                    newGen[i][j][k] = parentOne[0][j][k];
                }
            }
            index = randomEnd;
            for(int j = randomEnd ; j < citiesQuantity; j++){
                for(int k = randomStart ; k < randomEnd; k++){
                    if(parentTwo[0][j][0] != parentOne[0][k][0] && parentTwo[0][j][1] != parentOne[0][k][1]){
                        newGen[i][index][0] = parentTwo[0][j][0];
                        newGen[i][index][1] = parentTwo[0][j][1];
                    }else{
                        index--;
                        break;
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
                        if(parentTwo[0][j][0] != parentOne[0][k][0] && parentTwo[0][j][1] != parentOne[0][k][1]){
                            newGen[i][index][0] = parentTwo[0][j][0];
                            newGen[i][index][1] = parentTwo[0][j][1];
                        }else{
                            index--;
                            break;
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
                
                while(randomStart >= randomEnd && randomEnd == randomStart+diference){
                    randomStart = rand()%(citiesQuantity-1) + 1;
                    randomEnd = rand()%(citiesQuantity-1) + 1;
                }
                
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
    TravellingSalesman* travellingSalesman = new TravellingSalesman(matrix,citiesQ,100,5.0f,60.0f,5.0f);
    for(int i = 0;i < 100;i++)
        travellingSalesman->epoch();
    char c;
    std::cin >> c;
    return 0;
}
