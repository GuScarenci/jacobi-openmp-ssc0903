#include <stdio.h>
#include <stdlib.h>
#include "jacobiseq.h"
#include "matrix.h"

int main(int argc, char *argv[]){
    if(argc != 4){
        printf("Wrong!");
        return 1;
    }
    int N = atoi(argv[1]);
    //int T = atoi(argv[2]);
    int seed = atoi(argv[1]);

    srand(seed);

    double* matrix = createMatrix(3);
    double matrix2[3][3] = {{3,1,1},{1,3,1},{2,1,4}};

    for(int i = 0;i<N;i++){
        for(int j =0;j<N;j++){
            matrix[i*N + j] = matrix2[i][j];
        }
    }


    printf("RowMethod:%d\n",verifyConvergenceByRowMethod(matrix,3));
    printf("ColumnMethod:%d\n",verifyConvergenceByColumnMethod(matrix,3));
    ensureConvergenceByColumnMethod(matrix,3);
    ensureConvergenceByRowMethod(matrix,3);
    printf("RowMethod:%d\n",verifyConvergenceByRowMethod(matrix,3));
    printf("ColumnMethod:%d\n",verifyConvergenceByColumnMethod(matrix,3));
    double *constants = createConstants(N);

}