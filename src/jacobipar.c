#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "jacobipar.h"

float* jacobipar(float* matrix,float* constants,int N,float errorTolerance){

    float* lastVariables = malloc(sizeof(float)*N);

    float* currentVariables = malloc(sizeof(float)*N);

    if (lastVariables == NULL || currentVariables == NULL) {
        fprintf(stderr, "Erro! Sem memória o suficiente!\n");
        exit(1);
    }

    //Chute inicial igual a B
    #pragma omp parallel for simd
    for(int i = 0;i<N;i++){
        lastVariables[i] = constants[i];
    }

    int stop = 0;
    int count = 0;

    do {

        //Cálculo do vetor X_k+1
        #pragma omp parallel for shared(lastVariables,currentVariables,matrix)
        for(int i = 0;i <N;i++){
            float sum = 0;

            #pragma omp simd reduction(+:sum)
            for(int j = 0; j < N; j++){
                sum += matrix[i * N + j] * lastVariables[j];
            }
            currentVariables[i] = (constants[i]-sum);
        }
        //Fim do cálculo do vetor X_k+1

        //Verificação do critério de convergência
        #pragma omp task shared(stop) firstprivate(currentVariables, lastVariables)
        {
            float mr = 1;
            float maxError = -1;
            float maxVariable = -1;
            
            for(int i = 0;i<N;i++){
                float currentError = fabsf(currentVariables[i] - lastVariables[i]);
                if (currentError > maxError) {
                    maxError = currentError;
                }
                float absCurrentVariable = fabsf(currentVariables[i]);
                if (absCurrentVariable > maxVariable) {
                    maxVariable = absCurrentVariable;
                }
            }

            mr = maxError/maxVariable;
            stop = !(mr>errorTolerance);
        }
        //Fim da verificação do critério de convergência
        
        //Atualização do vetor X_k com o vetor X_k+1
        float* temp = lastVariables;
        lastVariables = currentVariables;
        currentVariables = temp;
        //Fim da atualização do vetor X_k com o vetor X_k+1

        count++;

    } while (!stop);

    printf("Iterações: %d\n",count);

    free(currentVariables);
    return lastVariables; //Essa variável tem que ser liberada fora!
}