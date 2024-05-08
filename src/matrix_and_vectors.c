#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "matrix_and_vectors.h"

void initiateMatrixAndVectors(float* matrix, float* constants,int N,int randLimit,float* diagonal){
    for(int i = 0;i<N;i++){

        //Preenchimento a matriz com números aleatórios a depender de randLimit garantindo diagonal dominante
        float sum = 0;
        for(int j = 0;j<N;j++){
            if(j != i){ 
                if(randLimit==-1){
                    matrix[i*N + j] = (rand() - (RAND_MAX/2)) / 1000.0f;
                }else if(randLimit ==0){
                    matrix[i*N + j] = (float)(rand());
                }else{
                    matrix[i*N + j] = (float)(rand()%randLimit);
                }
                sum += fabsf(matrix[i*N + j]);
            }
        }
        matrix[i*N+i] = sum + 1;
        //Fim do preenchimento da matriz

        //Armazena a diagonal original
        diagonal[i] = matrix[i*N+i];

        //Normalização da matriz
        for(int j = 0;j<N;j++){
            if(j != i){ 
                matrix[i*N + j] = matrix[i*N + j]/diagonal[i];
            }
        }
        matrix[i*N + i] = 0;
        //Fim da normalização da matriz

        //Preenchimento das constantes com números aleatórios a depender de randLimit
        if(randLimit==-1){
            constants[i] = (rand() - (RAND_MAX/2)) / 1000.0f;
        }else if(randLimit ==0){
            constants[i] = (float)(rand());
        }else{
            constants[i] = (float)(rand()%randLimit);
        }
        //Fim do preenchimento das constantes com números aleatórios a depender de randLimit

        //Normaliza as constantes
        constants[i] = constants[i]/diagonal[i];
    }
}