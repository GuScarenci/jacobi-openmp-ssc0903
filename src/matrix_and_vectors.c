#include <stdio.h> 
#include <stdlib.h>
#include <math.h>

double* createConstants(int N){
    double* constants;
    constants = malloc(sizeof(double)*N);
    for(int i = 0;i<N;i++){
        constants[i] = rand()%1000; // celulas da matriz limitadas a 1000
    }
    return constants;
}

double* createMatrix(int N){
    double* matrix = malloc(sizeof(double*)*N*N);

    for(int i = 0;i<N;i++){
        double sum = 0;

        for(int j = 0;j<N;j++){
            if(j != i){ 
                matrix[i*N + j] = rand()%1000; // celulas dos coeficientes limitados a 1000
                sum += fabs(matrix[i*N + j]);
            }
        }
        matrix[i*N + i] = sum+1;
    }
    return matrix;
}

void printMatrix(double* matrix,int N){
    for(int i = 0;i<N;i++){
        for(int j =0;j<N;j++){
            printf("%lf \t",matrix[i*N + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printConstants(double* constants,int N){
    for(int i = 0;i<N;i++){
        printf("%lf ",constants[i]);
        printf("\t");
    }
     printf("\n");
}

void vectorCompare(double* a,double* b,int N){
    for(int i=0;i<N;i++){
        if(fabs(a[i] - b[i]) > 0.00001){
            printf("Results DON'T match!\n");
            return;
        }
    }
    printf("Result match!\n");
}   
