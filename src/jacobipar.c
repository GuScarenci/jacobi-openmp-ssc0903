#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "jacobipar.h"

float* jacobipar(float* matrix,float* constants,int N,float errorTolerance){

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

    #pragma omp parallel for simd
    for(int i = 0;i<N;i++){
        lastVariables[i] = constants[i];
    }

    int stop = 0;
    int count = 0;
    do {

        #pragma omp parallel for shared(lastVariables,currentVariables,matrix)
        for(int i = 0;i <N;i++){
            float sum = 0;

            #pragma omp simd reduction(+:sum)
            for(int j = 0; j < N; j++){
                sum += matrix[i * N + j] * lastVariables[j];
            }
            currentVariables[i] = (constants[i]-sum);
        }

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
        
        float* temp = lastVariables;
        lastVariables = currentVariables;
        currentVariables = temp;
        count++;

    } while (!stop);

    printf("Iterações: %d\n",count);

    free(currentVariables);
    return lastVariables; //Free outside
}