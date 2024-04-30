#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jacobiseq.h"

float* jacobiseq(float* matrix,float* constants,int N,float errorTolerance){

    float* lastVariables = malloc(sizeof(float)*N);
    if (lastVariables == NULL) {
        fprintf(stderr, "Error: No sufficient memory!\n");
        exit(1);
    }
    float* currentVariables = malloc(sizeof(float)*N);
    if (currentVariables == NULL) {
        fprintf(stderr, "Error: No sufficient memory!\n");
        exit(1);
    }

    for(int i = 0;i<N;i++){
        lastVariables[i] = constants[i];
    }

    int converged = 0;

    do {
        converged = 1;
        float maxError = -1;
        float maxVariable = -1;

        for(int i = 0;i <N;i++){
            float sum = 0;

            for(int j = 0; j < N; j++){ // Summing all j < i
                sum += matrix[i * N + j] * lastVariables[j];
            }

            for(int j = i + 1; j < N; j++){ // Summing all j > i
                sum += matrix[i * N + j] * lastVariables[j];
            }

            currentVariables[i] = (constants[i]-sum);

            maxError = fmax(maxError,fabs(currentVariables[i]-lastVariables[i]));
            maxVariable = fmax(maxVariable,fabs(currentVariables[i]));
        }

        float mr = maxError/maxVariable;
        converged = !(mr>errorTolerance);
        
        float* temp = lastVariables;
        lastVariables = currentVariables;
        currentVariables = temp;

    } while (!converged);

    //printf("Numero de iterações:%d\n",count);

    free(currentVariables);
    return lastVariables; //Free outside
}