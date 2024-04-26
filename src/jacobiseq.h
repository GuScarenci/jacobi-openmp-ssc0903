#ifndef __jacobiseq_H__ 
#define __jacobiseq_H__

double* jacobiseq(double* matrix,double* constants,int N,float tolerance,int maxIterations);

void makeIterativeFormula(double* matrix,double* constants,int N);

void ensureConvergenceByRowMethod(double* matrix,int N);

void ensureConvergenceByColumnMethod(double* matrix,int N);

int verifyConvergenceByRowMethod(double* matrix,int N);

int verifyConvergenceByColumnMethod(double* matrix,int N);

#endif