#include <stdio.h>
#include <stdlib.h>

int jacobiseq(double* matrix,int N){
    return 0;
}

void ensureConvergenceByRowMethod(double* matrix,int N){
    
    for(int i = 0;i<N;i++){

        double alpha = 0;
        double sum = 0;
        double divisor = 0;

        for(int j=0;j<N;j++){
            if(j != i){ 
                sum += abs(matrix[i*N + j]);
            }
        }
        matrix[i*N + i] = sum+2;
    }
}

void ensureConvergenceByColumnMethod(double* matrix,int N){
    for(int j = 0;j<N;j++){

        double alpha = 0;
        double sum = 0;
        double divisor = 0;

        for(int i=0;i<N;i++){
            if(j != i){
                sum += abs(matrix[i*N + j]);
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
                sum += abs(matrix[i*N + j]);
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
                sum += abs(matrix[i*N + j]);
            }
        }

        alpha = sum/divisor;
        if(alpha >= 1){
            return 0;
        }
    }
    return 1;
}