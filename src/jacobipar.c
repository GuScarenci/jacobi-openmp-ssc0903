#include <stdio.h>

#include <stdlib.h>
#include <math.h>
#include <omp.h>

double* jacobipar(double* matrix,double* constants,int N,float errorTolerance,int maxIterations){

    double* lastVariables = malloc(sizeof(double)*N);
    double* currentVariables = malloc(sizeof(double)*N);

    #pragma omp simd //performs a little better than "parallel for" for tested cases
    for(int i = 0;i<N;i++){
        lastVariables[i] = 0;
    }

    int counter = 0;
    int convergenceProved = 1;
    
    do {
        convergenceProved = 1;
        #pragma omp parallel for shared(lastVariables, currentVariables, matrix, constants) reduction(&& : convergenceProved)
        for (int i = 0; i < N; i++) {

            double sum1 = 0;
            double sum2 =0;

            #pragma omp task shared(matrix, lastVariables)
            {
                #pragma omp simd reduction(+:sum1)
                for (int j = 0; j < i; j++) {
                    sum1 += matrix[i * N + j] * lastVariables[j];
                }
            }

            #pragma omp task shared(matrix, lastVariables)
            {   
                #pragma omp simd reduction(+:sum2)
                for (int j = i + 1; j < N; j++) {
                    sum2 += matrix[i * N + j] * lastVariables[j];
                }
            }

            #pragma omp taskwait // Wait for all tasks to complete

            double sum = sum1 + sum2;

            currentVariables[i] = (constants[i] - sum) / matrix[i * N + i];

            #pragma omp critical
            {
                if (fabs(currentVariables[i] - lastVariables[i]) > errorTolerance) {
                    convergenceProved = 0;
                }
            }
        }

        double* temp = lastVariables;
        lastVariables = currentVariables;
        currentVariables = temp;

        counter++;
    } while (!convergenceProved && counter < maxIterations);

    free(currentVariables);
    return lastVariables; //Free outside
}