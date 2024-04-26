#include <stdio.h>
#include <stdlib.h>

int jacobiseq(double** matrix,int N){
    return 0;
}

int verifyConvergenceByRowMethod(double** matrix,int N){
    

    for(int i = 0;i<N;i++){

        double alpha = 0;
        double sum = 0;
        double divisor = 0;

        for(int j=0;j<N;j++){
            if(j == i){
                divisor = matrix[i][j]; 
            }else{
                sum += abs(matrix[i][j]);
            }
        }

        alpha = sum/divisor;
        if(alpha >= 1){
            return 0;
        }
    }
    return 1;
}

int verifyConvergenceByColumnMethod(double** matrix,int N){
    

    for(int j = 0;j<N;j++){

        double alpha = 0;
        double sum = 0;
        double divisor = 0;

        for(int i=0;i<N;i++){
            if(j == i){
                divisor = matrix[i][j]; 
            }else{
                sum += abs(matrix[i][j]);
            }
        }

        alpha = sum/divisor;
        if(alpha >= 1){
            return 0;
        }
    }
    return 1;
}