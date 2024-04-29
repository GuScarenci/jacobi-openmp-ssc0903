#include <stdio.h> 
#include <stdlib.h>
#include <math.h>

double* createConstants(int N){
    double* constants;
    constants = malloc(sizeof(double)*N);
    if (constants == NULL) {
        fprintf(stderr, "Error: No sufficient memory in constants allocation!\n");
        exit(1);
    }

    for(int i = 0;i<N;i++){
        constants[i] =(double)rand()/100000; // celulas da matriz com uma magnitude menor
    }
    return constants;
}

double* createMatrix(int N){
    double* matrix = malloc(sizeof(double)*N*N);
    if (matrix == NULL) {
        fprintf(stderr, "Error: No sufficient memory in matrix allocation!\n");
        exit(1);
    }

    for(int i = 0;i<N;i++){
        double sum = 0;
        for(int j = 0;j<N;j++){
            if(j != i){ 
                matrix[i*N + j] = (double)rand()/100000; // celulas da matriz com uma magnitude menor
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
