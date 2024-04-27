#include <stdio.h>
#include <stdlib.h>
#include "jacobiseq.h"
#include "matrix.h"

int main(int argc, char *argv[]){

    if(argc != 4){
        printf("Wrong number of arguments!\n");
        return 1;
    }

    int N = atoi(argv[1]);
    //int T = atoi(argv[2]);
    int seed = atoi(argv[1]);

    srand(seed);
    double* matrix = createMatrix(N);
    double *constants = createConstants(N);

    double *results = jacobiseq(matrix,constants,N,0.00001,10000);
    printConstants(results,N);

    free(matrix);
    free(results);
    free(constants);
}
    //EXAMPLE
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