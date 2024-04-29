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
    float tolerance = 0.001;
    int maxIterations = 100000;
    srand(seed);

    float* matrix = createMatrix(N);
    float *constants = createConstants(N);
    omp_set_num_threads(T);
    omp_set_nested(1);

#ifdef JACOBIPAR
    float* results = jacobipar(matrix,constants,N,tolerance,maxIterations);
#else
    float* results = jacobiseq(matrix,constants,N,tolerance,maxIterations);
#endif

    //SHOWS EQUATION REQUESTED BY USER (WITH SEQUENTIAL AND PARALLEL)
    printf("Qual equação deseja ver? Escolha um número de 0 até %d.\n",N-1);
    int eqChoice = 0;

    scanf("%d",&eqChoice);

    double temp = 0;
    for(int j = 0;j<N;j++){
        temp += matrix[eqChoice*N+j]*results[j];
    }
    printf("Resultado calculado pelo sequencial:%lf\n",temp);
    printf("Resultado esperado:%lf\n\n",constants[eqChoice]);
    //END EQUATION REQUESTED BY USER SHOWING (WITH SEQUENTIAL AND PARALLEL)

    free(matrix);
    free(constants);
}
