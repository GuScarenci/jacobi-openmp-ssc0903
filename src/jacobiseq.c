#include <stdlib.h>
#include <math.h>

double* jacobiseq(double* matrix,double* constants,int N,float errorTolerance,int maxIterations){

    double* lastVariables = malloc(sizeof(double)*N);
    double* currentVariables = malloc(sizeof(double)*N);

    for(int i = 0;i<N;i++){
        lastVariables[i] = 0;
    }

    int counter = 0;
    int convergenceProved = 1;

    do {
        convergenceProved = 1;
        for(int i = 0;i <N;i++){
            double sum = 0;

            for(int j = 0; j < i; j++){ // Summing all j < i
                sum += matrix[i * N + j] * lastVariables[j];
            }

            for(int j = i + 1; j < N; j++){ // Summing all j > i
                sum += matrix[i * N + j] * lastVariables[j];
            }
            
            currentVariables[i] = (constants[i]-sum)/matrix[i*N+i];
            convergenceProved &= !(fabs(currentVariables[i]- lastVariables[i]) > errorTolerance);
        }

        double* temp = lastVariables;
        lastVariables = currentVariables;
        currentVariables = temp;

        counter++;
    } while (!convergenceProved && counter < maxIterations);

    free(currentVariables);
    return lastVariables; //Free outside
}