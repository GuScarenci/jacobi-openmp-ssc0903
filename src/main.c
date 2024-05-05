#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#include "jacobiseq.h"
#include "jacobipar.h"
#include "matrix_and_vectors.h"

int main(int argc, char *argv[]){

    //TREATS ARGUMENTS
    if(argc < 4 || argc > 6){
        printf("Wrong number of arguments!\n");
        return 1;
    }
    int N = atoi(argv[1]);
    int T = atoi(argv[2]);
    int seed = atoi(argv[3]);
    int eq = 0;
    int randLimit = 0;

    if(argc>4){
        eq = atoi(argv[4]);
    }

    if(argc>5){
        randLimit = atoi(argv[5]);
    }
    //END TREATS ARGUMENTS

    float tolerance = 0.001;
    srand(seed);

    //CREATES MATRIX
    float* matrix = createMatrix(N,randLimit);
    float* normalizedMatrix = normalizeMatrix(matrix,N);

    float* diagonal = getDiagonalFromMatrix(matrix,N);

    float* constants = createConstants(N,randLimit);
    float* normalizedConstants = normalizeConstants(constants,diagonal,N);
    free(diagonal);

    // free(matrix);
    // free(constants);
    //END CREATES MATRIX

    omp_set_num_threads(T);

    double delta = omp_get_wtime();

    //JACOBI
#ifdef JACOBIPAR
    float* results = jacobipar(normalizedMatrix,normalizedConstants,N,tolerance);
#else
    float* results = jacobiseq(normalizedMatrix,normalizedConstants,N,tolerance);
#endif
    //END JACOBI

    delta = (omp_get_wtime() - delta)*1000;
    printf("JacobiTime: %lfms\n",delta);

    free(normalizedMatrix);
    free(normalizedConstants);

    //SHOWS IF RESULTS MATCH
    // float* temp = (float*)malloc(sizeof(float)*N);
    // for(int eqChoice = 0;eqChoice<N;eqChoice++){
    //     temp[eqChoice] = 0;
    //     for(int j = 0;j<N;j++)
    //         temp[eqChoice] += matrix[eqChoice*N+j]*results[j];
    // }
    // vectorCompare(temp,constants,N);
    //END SHOWS IF RESULTS MATCH

    //SHOWS EQUATION RESULT REQUESTED BY USER
    // matrix = createMatrix(N,randLimit);
    // constants = createConstants(N,randLimit);
    float temp = 0;
    for(int j = 0;j<N;j++){
        temp += matrix[eq*N+j]*results[j];
    }
    printf("Resultado calculado:\t%lf\n",temp);
    printf("Resultado esperado:\t%lf\n",constants[eq]);
    //END EQUATION RESULT REQUESTED BY USER

    free(matrix);
    free(constants);
}
