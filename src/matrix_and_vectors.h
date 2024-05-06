#ifndef __matrix_H__ 
#define __matrix_H__

float* createConstants(int N,int randLimit);

float* createMatrix(int N,int randLimit,float* diagonal);

float* getDiagonalFromMatrix(float* matrix, int N);

float* normalizeConstants(float* vectorToNormalize,float* vectorDivisor, int N);

void printMatrix(float* matrix,int N);

void printConstants(float* constants,int N);

void vectorCompare(float* a,float* b,int N);

#endif
