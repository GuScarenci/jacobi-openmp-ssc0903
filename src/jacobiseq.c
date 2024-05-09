#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jacobiseq.h"

float* jacobiseq(float* matrix,float* constants,int N,float errorTolerance){
    
    float* lastVariables = malloc(sizeof(float)*N);

    float* currentVariables = malloc(sizeof(float)*N);

    if (lastVariables == NULL || currentVariables == NULL) {
        fprintf(stderr, "Erro! Sem memória o suficiente!\n");
        exit(1);
    }

    //Chute inicial igual a B
    for(int i = 0;i<N;i++){
        lastVariables[i] = constants[i];
    }

    int count = 0;
    float mr = 1;

    do {

        //Cálculo do vetor X_k+1
        for(int i = 0;i <N;i++){
            float sum = 0;
            for(int j = 0; j < N; j++){
                sum += matrix[i * N + j] * lastVariables[j];
            }
            currentVariables[i] = (constants[i]-sum);
        }
        //Fim do cálculo do vetor X_k+1

        //Verificação do critério de convergência
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
        //Fim da verificação do critério de convergência

        //Atualiza o o vetor X_k com o vetor X_k+1
        float* temp = lastVariables;
        lastVariables = currentVariables;
        currentVariables = temp;
        //Fim da atualização do vetor X_k com o vetor X_k+1

        count++;
        
    } while (mr>errorTolerance);

    printf("Iterações: %d\n",count);

    free(currentVariables);
    return lastVariables; //Essa variável tem que ser liberada fora!
}