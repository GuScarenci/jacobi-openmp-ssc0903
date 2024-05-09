#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#include "jacobiseq.h"
#include "jacobipar.h"
#include "matrix_and_vectors.h"

#define TOLERANCE 0.001

int main(int argc, char *argv[]){

    //Tratamento de argumentos
    if(argc < 4 || argc > 5){
        printf("Argumentos errados!\n");
        return 1;
    }
    int N = atoi(argv[1]);
    int T = atoi(argv[2]);
    int seed = atoi(argv[3]);
    int eq = 0;
    if(argc>4){
        eq = atoi(argv[4]);
    }
    //Fim do tratamento de argumentos

    //Criação de matrizes e constantes
    srand(seed);
    float* matrix = malloc(sizeof(float)*N*N);
    float* diagonal = malloc(sizeof(float)*N);
    float* constants = malloc(sizeof(float)*N);
    if (matrix == NULL || diagonal == NULL || constants == NULL) {
        fprintf(stderr, "Erro! Sem memória suficiente para alocação dos vetores!\n");
        exit(1);
    }
    initiateMatrixAndVectors(matrix,constants,N,diagonal);
    //Fim da criação de matrizes e constantes

    omp_set_num_threads(T);

    //Jacobi
    double delta = omp_get_wtime();
#ifdef JACOBIPAR //this allows us to choose between parallel and sequential jacobi during compilation, by compiling with the flag -DJACOBIPAR we define JACOBIPAR and the code will use the parallel version
    float* results = jacobipar(matrix,constants,N,TOLERANCE);
#else //if we don't define JACOBIPAR in the flags, the code will use the sequential version
    float* results = jacobiseq(matrix,constants,N,TOLERANCE);
#endif //references: https://gcc.gnu.org/onlinedocs/cpp/Ifdef.html and https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
    delta = (omp_get_wtime() - delta);
    printf("JacobiTime: %lfs\n",delta);
    //Fim Jacobi

    //Saída da equação escolhida pelo usuário!
    float temp = 0;
    for(int j = 0;j<N;j++){
        if(j!=eq){
            temp += diagonal[eq]*matrix[eq*N+j]*results[j];
        }else{
            temp += diagonal[eq]*results[j];
        }
    }
    printf("Resultado calculado:\t%lf\n",temp);
    printf("Resultado esperado:\t%lf\n",constants[eq]*diagonal[eq]);
    //Fim da saída da equação escolhida pelo usuário

    free(matrix);
    free(diagonal);
    free(constants);
}
