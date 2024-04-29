#include <stdio.h> 
#include <stdlib.h>
#include <math.h>

float* createConstants(int N){
    float* constants;
    constants = malloc(sizeof(float)*N);
    if (constants == NULL) {
        fprintf(stderr, "Error: No sufficient memory in constants allocation!\n");
        exit(1);
    }

    for(int i = 0;i<N;i++){
        constants[i] =(float)(rand()%1000); // celulas da matriz com uma magnitude menor
    }
    return constants;
}

float* createMatrix(int N){
    float* matrix = malloc(sizeof(float)*N*N);
    if (matrix == NULL) {
        fprintf(stderr, "Error: No sufficient memory in matrix allocation!\n");
        exit(1);
    }

    for(int i = 0;i<N;i++){
        float sum = 0;
        for(int j = 0;j<N;j++){
            if(j != i){ 
                matrix[i*N + j] = (float)(rand()%1000); // celulas da matriz com uma magnitude menor
                sum += fabs(matrix[i*N + j]);
            }
        }
        matrix[i*N + i] = sum+1;
    }
    return matrix;
}

void printMatrix(float* matrix,int N){
    for(int i = 0;i<N;i++){
        for(int j =0;j<N;j++){
            printf("%lf \t",matrix[i*N + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printConstants(float* constants,int N){
    for(int i = 0;i<N;i++){
        printf("%lf ",constants[i]);
        printf("\t");
    }
     printf("\n");
}

void vectorCompare(float* a,float* b,int N){
    for(int i=0;i<N;i++){
        if(fabs(a[i] - b[i]) > 0.00001){
            printf("Results DON'T match!\n");
            return;
        }
    }
    printf("Result match!\n");
}   
