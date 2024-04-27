#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "jacobiseq.h"
#include "jacobipar.h"
#include "matrix.h"

int main(int argc, char *argv[]){

    if(argc != 4){
        printf("Wrong number of arguments!\n");
        return 1;
    }

    int N = atoi(argv[1]);
    int T = atoi(argv[2]);
    int seed = atoi(argv[1]);

    double* matrix = createMatrix(N);
    double *constants = createConstants(N);
    omp_set_num_threads(T);
    srand(seed);

    double matrix2[3][3] = {{3,-1,-1},{-1,3,1},{2,1,4}};
    for(int i = 0;i<N;i++){
        for(int j =0;j<N;j++){
            matrix[i*N + j] = matrix2[i][j];
        }
    }
    double constants2[3] = {1,3,7};
    for(int i = 0;i<N;i++){
        constants[i] = constants2[i];
    }
    
    double delta = omp_get_wtime();
    double *results = jacobiseq(matrix,constants,N,0.001,100000);
    delta = (omp_get_wtime() - delta)*1000;
    printf("Sequential time: %lf ms\n",delta);

    printConstants(results,N);

    // double delta2 = omp_get_wtime();
    // double *results2 = jacobipar(matrix,constants,N,0.001,100000);
    // delta2 = (omp_get_wtime() - delta2)*1000;
    // printf("Parallel time: %lf ms\n",delta2);

    // printf("Speedup:%lf\n",(delta/delta2));

    // printf("%d\n",vectorCompare(results,results2,N));
    
    free(results);
    // free(results2);
    free(matrix);
    free(constants);
}
    //EXAMPLE TO MATRIX TO CHECK IF JACOBI IS WORKING!
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