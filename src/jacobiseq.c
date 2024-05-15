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
#include "jacobiseq.h"

float* jacobiseq(float* a,float* b,int N,float errorTolerance){
    
    float* x = malloc(sizeof(float)*N);

    float* nextX = malloc(sizeof(float)*N);

    if (x == NULL || nextX == NULL) {
        fprintf(stderr, "Erro! Sem memória o suficiente!\n");
        exit(1);
    }

    //Chute inicial igual a B
    for(int i = 0;i<N;i++){
        x[i] = b[i];
    }

    int count = 0;
    float mr = 1;

    do {

        //Cálculo do vetor X_k+1
        for(int i = 0;i <N;i++){
            float sum = 0;
            for(int j = 0; j < N; j++){
                sum += a[i * N + j] * x[j];
            }
            nextX[i] = (b[i]-sum);
        }
        //Fim do cálculo do vetor X_k+1

        //Verificação do critério de parada
        float maxDif = -1;
        float maxX = -1;
            
        for(int i = 0;i<N;i++){
            float currentDif = fabsf(nextX[i] - x[i]);
            if (currentDif > maxDif) {
                maxDif = currentDif;
            }
            float currentAbsX = fabsf(nextX[i]);
            if (currentAbsX > maxX) {
                maxX = currentAbsX;
            }
        }
        mr = maxDif/maxX;
        //Fim da verificação do critério de parada

        //Atualiza o o vetor X_k com o vetor X_k+1
        float* temp = x;
        x = nextX;
        nextX = temp;
        //Fim da atualização do vetor X_k com o vetor X_k+1

        count++;
        
    } while (mr>errorTolerance);

    printf("Iterações: %d\n",count);

    free(nextX);
    return x; //Essa variável tem que ser liberada fora!
}