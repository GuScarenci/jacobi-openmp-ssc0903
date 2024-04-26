#include <stdio.h>
#include <stdlib.h>
#include "jacobiseq.h"

double* createConstants(int N){
    double* constants;
    constants = malloc(sizeof(int)*N);
    for(int i = 0;i<N;i++){
        constants[i] = rand();
    }
    return constants;
}


double** createMatrix(int N){
    double** matrix;
    matrix = malloc(sizeof(int*)*N);
    for(int i = 0;i<N;i++){
        matrix[i] = malloc(sizeof(int)*N);
        for(int j = 0;j<N;j++){
            matrix[i][j] = rand();
        }
    }
    return matrix;
}

void printMatrix(double** matrix,int N){
    for(int i = 0;i<N;i++){
        for(int j =0;j<N;j++){
            printf("%lf ",matrix[i][j]);
        }
        printf("\n");
    }
}

void printConstants(double* constants,int N){
    for(int i = 0;i<N;i++){
        printf("%lf ",constants[i]);
        printf("\n");
    }
}

int main(int argc, char *argv[]){
    if(argc != 4){
        printf("Wrong!");
        return 1;
    }
    int N = atoi(argv[1]);
    //int T = atoi(argv[2]);
    int seed = atoi(argv[1]);

    srand(seed);

    double** matrix = createMatrix(3);
    double matrix2[3][3] = {{3,1,1},{1,3,1},{2,1,4}};

    for(int i = 0;i<N;i++){
        for(int j =0;j<N;j++){
            matrix[i][j] = matrix2[i][j];
        }
    }


    printf("RowMethod:%d\n",verifyConvergenceByRowMethod(matrix,3));
    printf("ColumnMethod:%d\n",verifyConvergenceByColumnMethod(matrix,3));
    //double *constants = createConstants(N);


    printf("%d",N);
}