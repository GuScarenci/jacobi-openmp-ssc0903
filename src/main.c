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
    int randLimit = 10;

    if(argc>4){
        eq = atoi(argv[4]);
    }

    if(argc>5){
        randLimit = atoi(argv[5]);
    }
    //END TREATS ARGUMENTS

    float tolerance = 0.001;
    srand(seed);

    //CREATES NORMALIZED MATRIX AND CONSTANTS
    float* diagonal;
    diagonal = malloc(sizeof(float)*N);
    if (diagonal == NULL) {
        fprintf(stderr, "Error: No sufficient memory in diagonal allocation!\n");
        exit(1);
    }
    float* matrix = createMatrix(N,randLimit,diagonal);
    float* constants = createConstants(N,randLimit);
    float* normalizedConstants = normalizeConstants(constants,diagonal,N);
    //END CREATES NORMALIZED MATRIX AND CONSTANTS

    omp_set_num_threads(T);

    double delta = omp_get_wtime();

    //JACOBI
#ifdef JACOBIPAR
    float* results = jacobipar(matrix,normalizedConstants,N,tolerance);
#else
    float* results = jacobiseq(matrix,normalizedConstants,N,tolerance);
#endif
    //END JACOBI
    free(normalizedConstants);

    delta = (omp_get_wtime() - delta);
    printf("JacobiTime: %lfs\n",delta);


    //SHOWS EQUATION RESULT REQUESTED BY USER
    float temp = 0;
    for(int j = 0;j<N;j++){
        if(j!=eq){
            temp += diagonal[eq]*matrix[eq*N+j]*results[j];
        }else{
            temp += diagonal[eq]*results[j];
        }
    }
    printf("Resultado calculado:\t%lf\n",temp);
    printf("Resultado esperado:\t%lf\n",constants[eq]);
    //END EQUATION RESULT REQUESTED BY USER

    free(diagonal);
    free(matrix);
    free(constants);
}
