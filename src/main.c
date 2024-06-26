//Trabalho 1 de Computação de Alto Desempenho - SCC00903
//
//  Docente:
//      Paulo Sérgio Lopes de Souza
//  Alunos:
//      Beatriz Lomes da Silva -  12548038
//      Gustavo Moura Scarenci de Carvalho Ferreira -12547792
//      Hugo Hiroyuki Nakamura - 12732037
//      Matheus Henrique Dias Cirillo - 12547750

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
    float* a = malloc(sizeof(float)*N*N);
    float* diagonal = malloc(sizeof(float)*N);
    float* b = malloc(sizeof(float)*N);
    if (a == NULL || diagonal == NULL || b == NULL) {
        fprintf(stderr, "Erro! Sem memória suficiente para alocação dos vetores!\n");
        exit(1);
    }
    initiateMatrixAndVectors(a,b,N,diagonal);
    //Fim da criação de matrizes e constantes

    omp_set_num_threads(T);

    //Jacobi
    double delta = omp_get_wtime();
#ifdef JACOBIPAR //Isso nos permite escolher entre jacobi paralelo e sequencial durante a compilação, ao compilar com a flag -DJACOBIPAR definimos JACOBIPAR e o código usará a versão paralela
    float* results = jacobipar(a,b,N,TOLERANCE);
#else //Se não definirmos JACOBIPAR nas flags, o código usará a versão sequencial
    float* results = jacobiseq(a,b,N,TOLERANCE);
#endif //Referência: https://gcc.gnu.org/onlinedocs/cpp/Ifdef.html and https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
    delta = (omp_get_wtime() - delta);
    printf("JacobiTime: %lfs\n",delta);
    //Fim Jacobi

    //Saída da equação escolhida pelo usuário!
    float temp = 0;
    for(int j = 0;j<N;j++){
        if(j!=eq){
            temp += diagonal[eq]*a[eq*N+j]*results[j];
        }else{
            temp += diagonal[eq]*results[j];
        }
    }
    printf("Resultado calculado:\t%lf\n",temp);
    printf("Resultado esperado:\t%lf\n",b[eq]*diagonal[eq]);
    //Fim da saída da equação escolhida pelo usuário

    free(a);
    free(diagonal);
    free(b);
}
