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
    int seed = atoi(argv[1]);
    //FINISHES ARGUMENTS TREATMENT

    //GENERATE MATRIX AND VECTORS
    double delta = omp_get_wtime();

    double* matrix = createMatrix(N);
    double *constants = createConstants(N);
    omp_set_num_threads(T);
    srand(seed);
    omp_set_nested(1);

    delta = (omp_get_wtime() - delta)*1000;
    printf("IO time: %lf ms\n",delta);
    //FINISH MATRIX AND VECTORS GENERATIONS
    
    //SEQUENTIAL JACOBI
    delta = omp_get_wtime();
    double *results = jacobiseq(matrix,constants,N,0.001,10000);
    delta = (omp_get_wtime() - delta)*1000;
    printf("Sequential time: %lf ms\n",delta);
    //FINISH SEQUENTIAL JACOBI

    //PARALLEL JACOBI
    double delta2 = omp_get_wtime();
    double *results2 = jacobipar(matrix,constants,N,0.001,10000);
    delta2 = (omp_get_wtime() - delta2)*1000;
    printf("Parallel time: %lf ms\n",delta2);
    //FINISH PARALLEL JACOBI

    //CALCULATES SPEEDUP AND EFFICIENCY
    printf("Speedup:%lf\n",(delta/delta2));
    printf("Eficiência:%lf\n",(delta/delta2)/T);
    //FINISHES SPEEDUP AND EFFICIENCY CALCULUS

    //SHOWS EQUATION REQUESTED BY USER (WITH SEQUENTIAL AND PARALLEL)
    printf("Qual equação deseja ver? Escolha um número de 0 até %d.\n",N-1);
    int eqChoice = -1;
    while(eqChoice<0 || eqChoice>=N){
        scanf("%d",&eqChoice);
        if(eqChoice<0 || eqChoice>=N){
            printf("Escolha um número de 0 até %d!\n",N-1);
        }
    }

    double temp = 0;
    for(int j = 0;j<N;j++){
        temp += matrix[eqChoice*N+j]*results[j];
    }
    printf("Resultado calculado pelo sequencial:%lf\n",temp);

    temp = 0;
    for(int j = 0;j<N;j++){
        temp += matrix[eqChoice*N+j]*results2[j];
    }

    printf("Resultado calculado pelo paralelo:%lf\n",temp);

    printf("Resultado esperado:%lf\n\n",constants[eqChoice]);
    //END EQUATION REQUESTED BY USER SHOWING (WITH SEQUENTIAL AND PARALLEL)


    //CHECKS IF BOTH METHODS GOT THE SAME RESULT
    vectorCompare(results,results2,N);
    //END IF BOTH METHODS GOT THE SAME RESULT CHECKING
    
    //FREE POINTERS
    free(results);
    free(results2);
    free(matrix);
    free(constants);
    //END POINTERS FREEING
}

//EXAMPLE OF MATRIX TO CHECK IF JACOBI IS WORKING! (only because i know the right result for these inputs)
// double matrix2[3][3] = {{3,-1,-1},{-1,3,1},{2,1,4}};
// for(int i = 0;i<N;i++){
//     for(int j =0;j<N;j++){
//         matrix[i*N + j] = matrix2[i][j];
//     }
// }
// double constants2[3] = {1,3,7};
// for(int i = 0;i<N;i++){
//     constants[i] = constants2[i];
// }
//EXAMPLE