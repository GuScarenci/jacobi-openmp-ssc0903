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

    float mr = 1;

    int count = 0;

    do {
        float maxError = -1;
        float maxVariable = -1;

        for(int i = 0;i <N;i++){
            float sum = 0;

            for(int j = 0; j < N; j++){
                    sum += matrix[i * N + j] * lastVariables[j];       
            }

            currentVariables[i] = (constants[i]-sum);

            float currentError = fabs(currentVariables[i] - lastVariables[i]);
            if (currentError > maxError) {
                maxError = currentError;
            }
            float absCurrentVariable = fabs(currentVariables[i]);
            if (absCurrentVariable > maxVariable) {
                maxVariable = absCurrentVariable;
            }
        }

        mr = maxError/maxVariable;
        
        float* temp = lastVariables;
        lastVariables = currentVariables;
        currentVariables = temp;
        count++;

    } while (mr>errorTolerance);

    printf("Interações: %d\n",count);

    free(currentVariables);
    return lastVariables; //Free outside
}