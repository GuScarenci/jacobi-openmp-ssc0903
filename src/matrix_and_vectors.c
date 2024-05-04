#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "matrix_and_vectors.h"

float* createConstants(int N,int randLimit){
    float* constants;
    constants = malloc(sizeof(float)*N);
    if (constants == NULL) {
        fprintf(stderr, "Error: No sufficient memory in constants allocation!\n");
        exit(1);
    }

    for(int i = 0;i<N;i++){
        if(randLimit == 0){
            constants[i] = (rand() - (RAND_MAX/2)) / 1000.0f; // celulas da matriz com uma magnitude menor
        }else{
            constants[i] =(float)(rand()%randLimit);
        }
    }
    return constants;
}

float* createMatrix(int N,int randLimit){

    //ALLOCATES MATRIX
    float* matrix = malloc(sizeof(float)*N*N);
    if (matrix == NULL) {
        fprintf(stderr, "Error: No sufficient memory in matrix allocation!\n");
        exit(1);
    }

    //CREATES RANDOM VALUES WHILE ENSURING DOMINANT DIAGONAL MATRIX
    for(int i = 0;i<N;i++){
        float sum = 0;
        for(int j = 0;j<N;j++){
            if(j != i){ 
                if(randLimit==0){
                    matrix[i*N + j] = (rand() - (RAND_MAX/2)) / 1000.0f;
                }else{
                    matrix[i*N + j] = (float)(rand()%randLimit);
                }
                sum += fabs(matrix[i*N + j]);
            }
        }
        matrix[i*N + i] = sum+1;
    }

    return matrix;
}

float* normalizeMatrix(float* matrix,int N){

    //ALLOCATES MATRIX
    float* normalizedMatrix = malloc(sizeof(float)*N*N);
    if (normalizedMatrix == NULL) {
        fprintf(stderr, "Error: No sufficient memory in normalized matrix allocation!\n");
        exit(1);
    }
    //END ALLOCATES MATRIX

    //NORMALIZES MATRIX
    for(int i = 0;i<N;i++){
        for(int j = 0;j<N;j++){
            if(j != i){ 
                normalizedMatrix[i*N + j] = matrix[i*N + j]/matrix[i*N+i];
            }
        }
        normalizedMatrix[i*N + i] = 0;
    }
    //END NORMALIZES MATRIX
    return normalizedMatrix;
}

float* getDiagonalFromMatrix(float* matrix, int N){
    float* diagonal;
    diagonal = malloc(sizeof(float)*N);
    if (diagonal == NULL) {
        fprintf(stderr, "Error: No sufficient memory in diagonal allocation!\n");
        exit(1);
    }
    for(int i = 0;i<N;i++){
        diagonal[i] = matrix[i*N+i];
    }  
    return diagonal;
}

float* normalizeConstants(float* vectorToNormalize,float* vectorDivisor, int N){
    float* normalizedConstants;
    normalizedConstants = malloc(sizeof(float)*N);
    if (normalizedConstants == NULL) {
        fprintf(stderr, "Error: No sufficient memory in diagonal allocation!\n");
        exit(1);
    }

    for(int i = 0;i<N;i++){
        normalizedConstants[i] = vectorToNormalize[i]/vectorDivisor[i];
    }  

    return normalizedConstants;
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
        if(fabs(a[i] - b[i]) > 0.5){
            printf("Results DON'T match!\n");
            printf("%f %f\n",a[i],b[i]);
            printf("%f\n",a[i]-b[i]);
            printf("%f\n\n",(a[i]-b[i])/b[i]);
        }
    }
    printf("Result match!\n");
}   
