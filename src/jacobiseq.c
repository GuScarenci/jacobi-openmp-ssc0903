#include <stdlib.h>
#include <math.h>


double* jacobiseq(double* matrix,double* constants,int N,float errorTolerance,int maxIterations){

    double* lastVariables = malloc(sizeof(double)*N);

    for(int i = 0;i<N;i++){
        lastVariables[i] = 0;
    }

    double* currentVariables = malloc(sizeof(double)*N);
    int maxErrorReached = 0;

    int counter = 0;
    maxErrorReached = 1;
    
    do {

        for(int i = 0;i <N;i++){
            double sum = 0;
            for(int j =0;j<N;j++){
                if(i!=j){
                    sum += matrix[i*N+j]*lastVariables[j];
                }
            }
            currentVariables[i] = (constants[i]-sum)/matrix[i*N+i];
            if (fabs(currentVariables[i]- lastVariables[i]) > errorTolerance){
                maxErrorReached = 0;
            };
        }

        double* temp = lastVariables;
        lastVariables = currentVariables;
        currentVariables = temp;
        counter++;
    } while (!maxErrorReached && counter < maxIterations);

    free(currentVariables);
    return lastVariables; //Free outside
}