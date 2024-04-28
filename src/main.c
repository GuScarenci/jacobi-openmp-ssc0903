#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

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
    double tolerance = 0.001;
    int maxIterations = 10000;

    double* matrix = createMatrix(N);
    double *constants = createConstants(N);
    omp_set_num_threads(T);
    srand(seed);
    omp_set_nested(1);

#ifdef JACOBIPAR
    jacobipar(matrix,constants,N,tolerance,maxIterations);
#else
    jacobiseq(matrix,constants,N,tolerance,maxIterations);
#endif

    free(matrix);
    free(constants);
}
