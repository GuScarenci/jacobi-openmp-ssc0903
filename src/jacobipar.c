#include <stdlib.h>
#include <math.h>
#include <omp.h>

double* jacobipar(double* matrix,double* constants,int N,float errorTolerance,int maxIterations,int T){

    double* lastVariables = malloc(sizeof(double)*N);
    double* error = malloc(sizeof(double)*N);
    double* currentVariables = malloc(sizeof(double)*N);

    #pragma omp parallel num_threads(T)
    omp_set_nested(1);
    {
        #pragma omp parallel for
        for(int i = 0;i<N;i++){
            lastVariables[i] = 0;
            error[i] = 10;
        }

        #pragma omp single
        {
            int maxErrorReached = 0;

            int counter = 0;
            while (!maxErrorReached && counter < maxIterations) {

                #pragma omp parallel for
                for(int i = 0;i <N;i++){
                    double sum = 0;

                    #pragma omp parallel for
                    for(int j =0;j<N;j++){
                        if(i!=j){
                            sum += matrix[i*N+j]*lastVariables[j];
                        }
                    }
                    currentVariables[i] = (constants[i]-sum)/matrix[i*N+i];
                    error[i] = fabs(currentVariables[i]- lastVariables[i]);
                }

                maxErrorReached = 1;

                #pragma omp parallel for
                for(int i = 0;i<N;i++){
                    lastVariables[i] = currentVariables[i];
                    if (error[i] > errorTolerance){
                        maxErrorReached = 0;
                    };
                }
                counter++;
            }
        }//end #pragma omp single
    }
    free(currentVariables);
    free(error);
    return lastVariables; //Free outside
}