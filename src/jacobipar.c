#include <stdlib.h>
#include <math.h>
#include <omp.h>

double* jacobipar(double* matrix,double* constants,int N,float errorTolerance,int maxIterations){

    double* lastVariables = malloc(sizeof(double)*N);
    double* error = malloc(sizeof(double)*N);
    double* currentVariables = malloc(sizeof(double)*N);

    #pragma omp parallel
    omp_set_nested(1);
    {
        #pragma omp parallel for
        for(int i = 0;i<N;i++){
            lastVariables[i] = 0;
            error[i] = 10;
        }

        #pragma omp single
        {
            int errorStopCriteriumReached = 0;
            int counter = 0;
            while (!errorStopCriteriumReached && counter < maxIterations) {

                #pragma omp parallel for
                for(int i = 0;i <N;i++){
                    double sum = 0;

                    #pragma omp parallel for reduction(+:sum) firstprivate(i)
                    for(int j = 0;j<N;j++){
                        if(i!=j){
                            sum += matrix[i*N+j]*lastVariables[j];
                        }
                    }
                    currentVariables[i] = (constants[i]-sum)/matrix[i*N+i];
                    error[i] = fabs(currentVariables[i]- lastVariables[i]);
                }

                int maxErrorReached = 0;

                #pragma omp parallel for reduction(max:maxErrorReached)
                for(int i = 0;i<N;i++){
                    lastVariables[i] = currentVariables[i];
                    if (error[i] > maxErrorReached){
                        maxErrorReached = error[i];
                    };
                }

                if(maxErrorReached<errorTolerance){
                    errorStopCriteriumReached = 1;
                }
                counter++;
            }
        }//end #pragma omp single
    }

    free(currentVariables);
    free(error);
    return lastVariables; //Free outside
}