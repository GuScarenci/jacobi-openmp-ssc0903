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

    #pragma omp simd
    for(int i = 0;i<N;i++){
        lastVariables[i] = constants[i];
    }

    int converged = 0;

    do {
        converged = 1;
        float maxError = -1;
        float maxVariable = -1;

        //#pragma omp parallel for reduction(max:maxError,maxVariable)
        for(int i = 0;i <N;i++){
            float sum = 0;

            //#pragma omp task
            {
                //#pragma omp simd reduction(+:sum)
                for(int j = 0; j < i; j++){ // Summing all j < i
                    sum += matrix[i * N + j] * lastVariables[j];
                }
            }

            //#pragma omp task
            {
                //#pragma omp simd reduction(+:sum)
                for(int j = i + 1; j < N; j++){ // Summing all j > i
                    sum += matrix[i * N + j] * lastVariables[j];
                }
            }

            //#pragma omp taskwait
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