//Trabalho 1 de Computação de Alto Desempenho - SCC00903
//
//  Docente:
//      Paulo Sérgio Lopes de Souza
//  Alunos:
//      Beatriz Lomes da Silva -  12548038
//      Gustavo Moura Scarenci de Carvalho Ferreira -12547792
//      Hugo Hiroyuki Nakamura - 12732037
//      Matheus Henrique Dias Cirillo - 12547750

#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "matrix_and_vectors.h"

#define LIMIT_RAND 10

void initiateMatrixAndVectors(float* a, float* b,int N,float* diagonal){
    for(int i = 0;i<N;i++){

        //Preenchimento a matriz com números aleatórios a depender de randLimit garantindo diagonal dominante
        float sum = 0;
        for(int j = 0;j<N;j++){
            if(j != i){ 
                a[i*N + j] = (float)(rand()%LIMIT_RAND);
                sum += fabsf(a[i*N + j]);
            }
        }
        a[i*N+i] = sum + 1;
        //Fim do preenchimento da matriz

        //Armazena a diagonal original
        diagonal[i] = a[i*N+i];

        //Normalização da matriz
        for(int j = 0;j<N;j++){
            if(j != i){ 
                a[i*N + j] = a[i*N + j]/diagonal[i];
            }
        }
        a[i*N + i] = 0;
        //Fim da normalização da matriz

        //Preenchimento das constantes com números aleatórios a depender de randLimit
        b[i] = (float)(rand()%LIMIT_RAND);
        //Fim do preenchimento das constantes com números aleatórios a depender de randLimit

        //Normaliza as constantes
        b[i] = b[i]/diagonal[i];
    }
}