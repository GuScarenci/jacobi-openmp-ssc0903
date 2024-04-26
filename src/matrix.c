#include <stdio.h>
#include <stdlib.h>

double* createConstants(int N){
    double* constants;
    constants = malloc(sizeof(int)*N);
    for(int i = 0;i<N;i++){
        constants[i] = rand();
    }
    return constants;
}


double* createMatrix(int N){
    double* matrix = malloc(sizeof(int*)*N*N);

    for(int i = 0;i<N;i++){
        for(int j = 0;j<N;j++){
            matrix[i*N + j] = rand();
        }
    }
    return matrix;
}

void printMatrix(double* matrix,int N){
    for(int i = 0;i<N;i++){
        for(int j =0;j<N;j++){
            printf("%lf ",matrix[i*N + j]);
        }
        printf("\n");
    }
}

void printConstants(double* constants,int N){
    for(int i = 0;i<N;i++){
        printf("%lf ",constants[i]);
        printf("\n");
    }
}
