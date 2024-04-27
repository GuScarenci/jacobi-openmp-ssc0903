#include <stdlib.h>
#include <math.h>


double* jacobiseq(double* matrix,double* constants,int N,float errorTolerance,int maxIterations){

    double* lastVariables = malloc(sizeof(double)*N);
    double* error = malloc(sizeof(double)*N);

    for(int i = 0;i<N;i++){
        lastVariables[i] = 0;
        error[i] = 10;
    }

    double* currentVariables = malloc(sizeof(double)*N);
    int maxErrorReached = 0;

    int counter = 0;
    while (!maxErrorReached && counter < maxIterations) {

        for(int i = 0;i <N;i++){
            double sum = 0;
            for(int j =0;j<N;j++){
                if(i!=j){
                    sum += matrix[i*N+j]*lastVariables[j];
                }
            }
            currentVariables[i] = (constants[i]-sum)/matrix[i*N+i];
            error[i] = fabs(currentVariables[i]- lastVariables[i]);
        }

        maxErrorReached = 1;

        for(int i = 0;i<N;i++){
            lastVariables[i] = currentVariables[i];
            if (error[i] > errorTolerance){
                maxErrorReached = 0;
            };
        }
        counter++;
    }
    free(currentVariables);
    free(error);
    return lastVariables; //Free outside
}