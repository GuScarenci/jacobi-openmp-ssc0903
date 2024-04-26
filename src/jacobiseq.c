#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "jacobiseq.h"
#include "math.h"

double* jacobiseq(double* matrix,double* constants,int N){

    double* lastVariables = malloc(sizeof(double)*N);
    double* error = malloc(sizeof(double)*N);

    for(int i = 0;i<N;i++){
        lastVariables[i] = 0;
        error[i] = 10;
    }

    double* currentVariables = malloc(sizeof(double)*N);
    int maximumErrorReached = 0;

    int counter = 0;
    int maxIterations = 1000;

    while (!maximumErrorReached && counter < maxIterations) {

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

        maximumErrorReached = 1;

        for(int i = 0;i<N;i++){
            lastVariables[i] = currentVariables[i];
            if (error[i] > 0.00001){
                maximumErrorReached = 0;
            };
        }
    }
    free(currentVariables);
    free(error);
    return lastVariables; //Free outside
}

void ensureConvergenceByRowMethod(double* matrix,int N){
    
    for(int i = 0;i<N;i++){

        double sum = 0;

        for(int j=0;j<N;j++){
            if(j != i){ 
                sum += fabs(matrix[i*N + j]);
            }
        }
        matrix[i*N + i] = sum+2;
    }
}

void ensureConvergenceByColumnMethod(double* matrix,int N){
    for(int j = 0;j<N;j++){
        double sum = 0;
        for(int i=0;i<N;i++){
            if(j != i){
                sum += fabs(matrix[i*N + j]);
            }
        }
        matrix[j*N + j] = sum+2;
    }
}

int verifyConvergenceByRowMethod(double* matrix,int N){

    for(int i = 0;i<N;i++){

        double alpha = 0;
        double sum = 0;
        double divisor = 0;

        for(int j=0;j<N;j++){
            if(j == i){
                divisor = matrix[i*N + j]; 
            }else{
                sum += fabs(matrix[i*N + j]);
            }
        }

        alpha = sum/divisor;
        if(alpha >= 1){
            return 0;
        }
    }
    return 1;
}

int verifyConvergenceByColumnMethod(double* matrix,int N){
    

    for(int j = 0;j<N;j++){

        double alpha = 0;
        double sum = 0;
        double divisor = 0;

        for(int i=0;i<N;i++){
            if(j == i){
                divisor = matrix[i*N + j]; 
            }else{
                sum += fabs(matrix[i*N + j]);
            }
        }

        alpha = sum/divisor;
        if(alpha >= 1){
            return 0;
        }
    }
    return 1;
}