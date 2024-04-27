#ifndef __matrix_H__ 
#define __matrix_H__

double* createConstants(int N);

double* createMatrix(int N);

void printMatrix(double* matrix,int N);

void printConstants(double* constants,int N);

int vectorCompare(double* a,double* b,int N);

#endif
