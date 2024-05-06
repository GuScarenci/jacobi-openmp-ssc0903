#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "jacobipar.h"

float* jacobipar(float* matrix,float* constants,int N,float errorTolerance){

    float* sum = malloc(sizeof(float)*N);
    if (sum == NULL) {
        fprintf(stderr, "Error: No sufficient memory!\n");
        exit(1);
    }

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

    #pragma omp parallel for simd
    for(int i = 0;i<N;i++){
        sum[i] = 0;
    }

    float mr = 1;

    int stop = 0;

    do {
        #pragma omp parallel for simd
        for(int i = 0;i<N;i++){
            sum[i] = 0;
        }
        
        float maxError = -1;
        float maxVariable = -1;

        #pragma omp parallel for shared(lastVariables,currentVariables,sum) collapse(2)
        for(int i = 0;i <N;i++){
            for(int j = 0; j < N; j++){
                sum[i] += matrix[i * N + j] * lastVariables[j];
            }
        }


        #pragma omp task shared(stop)
        {
            for(int i = 0;i<N;i++){
                currentVariables[i] = (constants[i]-sum[i]);
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

    } while (!stop);

    free(currentVariables);
    return lastVariables; //Free outside
}