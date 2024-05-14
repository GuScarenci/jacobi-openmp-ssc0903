#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "jacobipar.h"

float* jacobipar(float* a,float* b,int N,float errorTolerance){

    float* x = malloc(sizeof(float)*N);

    float* nextX = malloc(sizeof(float)*N);

    if (x == NULL || nextX == NULL) {
        fprintf(stderr, "Erro! Sem memória o suficiente!\n");
        exit(1);
    }

    //Chute inicial igual a B
    #pragma omp parallel for simd
    for(int i = 0;i<N;i++){
        x[i] = b[i];
    }

    int stop = 0;
    int count = 0;

    do {

        //Cálculo do vetor X_k+1
        #pragma omp parallel for shared(x,nextX,a)
        for(int i = 0;i <N;i++){
            float sum = 0;

            #pragma omp simd reduction(+:sum)
            for(int j = 0; j < N; j++){
                sum += a[i * N + j] * x[j];
            }
            nextX[i] = (b[i]-sum);
        }
        //Fim do cálculo do vetor X_k+1

        //Verificação do critério de convergência
        #pragma omp task shared(stop) firstprivate(nextX, x)
        {
            float mr = 1;
            float maxDif = -1;
            float maxX = -1;
            
            #pragma omp simd reduction(max:maxDif,maxX) 
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
            stop = !(mr>errorTolerance);
        }
        //Fim da verificação do critério de convergência
        
        //Atualização do vetor X_k com o vetor X_k+1
        float* temp = x;
        x = nextX;
        nextX = temp;
        //Fim da atualização do vetor X_k com o vetor X_k+1

        count++;

    } while (!stop);

    printf("Iterações: %d\n",count);

    free(nextX);
    return x; //Essa variável tem que ser liberada fora!
}