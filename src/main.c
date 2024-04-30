#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#include "jacobiseq.h"
#include "jacobipar.h"
#include "matrix_and_vectors.h"

int main(int argc, char *argv[]){

    //TREATS ARGUMENTS
    if(argc != 4){
        printf("Wrong number of arguments!\n");
        return 1;
    }
    int N = atoi(argv[1]);
    int T = atoi(argv[2]);
    int seed = atoi(argv[3]);
    //END TREATS ARGUMENTS

    float tolerance = 0.001;
    srand(seed);

    //CREATES MATRIX
    float* matrix = createMatrix(N);
    float* normalizedMatrix = normalizeMatrix(matrix,N);
    float* diagonal = getDiagonalFromMatrix(matrix,N);
    float* constants = createConstants(N);
    float* normalizedConstants = normalizeConstants(constants,diagonal,N);
    //END CREATES MATRIX

    omp_set_num_threads(T);

    //JACOBI
#ifdef JACOBIPAR
    float* results = jacobipar(normalizedMatrix,normalizedConstants,N,tolerance);
#else
    float* results = jacobiseq(normalizedMatrix,normalizedConstants,N,tolerance);
#endif
    //END JACOBI

    //SHOWS EQUATION RESULT REQUESTED BY USER
    // printf("Qual equação deseja ver? Escolha um número de 0 até %d.\n",N-1);

    float* temp = (float*)malloc(sizeof(float)*N);
    for(int eqChoice = 0;eqChoice<N;eqChoice++){
        //scanf("%d",&eqChoice);
        temp[eqChoice] = 0;
        for(int j = 0;j<N;j++){
            temp[eqChoice] += matrix[eqChoice*N+j]*results[j];
        }
    }
    vectorCompare(temp,constants,N);
    //printf("Resultado calculado pelo sequencial:%lf\n",temp);
    //printf("Resultado esperado:%lf\n\n",constants[eqChoice]);
    //END EQUATION RESULT REQUESTED BY USER

    free(matrix);
    free(constants);
}
