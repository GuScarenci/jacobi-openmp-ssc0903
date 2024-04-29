#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

float* jacobipar(float* matrix,float* constants,int N,float errorTolerance,int maxIterations){

    float* lastVariables = malloc(sizeof(float)*N);
    if (constants == NULL) {
        fprintf(stderr, "Error: No sufficient memory!\n");
        exit(1);
    }
    float* currentVariables = malloc(sizeof(float)*N);
    if (constants == NULL) {
        fprintf(stderr, "Error: No sufficient memory!\n");
        exit(1);
    }

    #pragma omp simd //performs a little better than "parallel for" for tested cases
    for(int i = 0;i<N;i++){
        lastVariables[i] = 0;
    }

    int counter = 0;
    int convergenceProved = 1;
    
    do {
        convergenceProved = 1;

        #pragma omp parallel for shared(lastVariables, currentVariables) reduction(&&:convergenceProved)
        for (int i = 0; i < N; i++) {

             float sum = 0;

            #pragma omp task shared(matrix, lastVariables, sum)
            {
                #pragma omp simd reduction(+:sum)
                for (int j = 0; j < i; j++) {
                    sum += matrix[i * N + j] * lastVariables[j];
                }
            }

            #pragma omp task shared(matrix, lastVariables, sum)
            {
                #pragma omp simd reduction(+:sum)
                for (int j = i + 1; j < N; j++) {
                    sum += matrix[i * N + j] * lastVariables[j];
                }
            }

            #pragma omp taskwait // Wait for all tasks to complete

            currentVariables[i] = (constants[i] - sum) / matrix[i * N + i];

            convergenceProved &= !(fabs(currentVariables[i] - lastVariables[i]) > errorTolerance);
        }

        float* temp = lastVariables;
        lastVariables = currentVariables;
        currentVariables = temp;

        counter++;
    } while (!convergenceProved && counter < maxIterations);

    free(currentVariables);
    return lastVariables; //Free outside
}