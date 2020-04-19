#ifndef PASSI
#define PASSI 100000
#endif
#include <iostream>
#include <cstdio>
#include <omp.h>
#include <stdlib.h>
using namespace std;
const int N = PASSI;
double* A;
double* B;
double* C;
double t_Start;
double t_Stop;
void  vectorSum();
void  deleteVector();

int main()
{
    vectorSum();
    deleteVector();
    return 0;
}

void vectorSum()
{
    A = new double[N];
    B = new double[N];
    C = new double[N];
    int i, result=0.0;

    for (i=0; i < N; i++)
    A[i] = B[i] = (double)i;

    t_Start = omp_get_wtime();

    #pragma omp parallel for private(i) schedule(static)
    for (i=0; i < N; i++) {
        C[i] = A[i] + B[i];
    }

    #pragma omp parallel for default(shared)\
     private(i) schedule(static)\
     reduction(+:result)  
    for (i=0; i < N; i++)
        result += (C[i] * A[i]);

    t_Stop = omp_get_wtime() - t_Start;
    //cout << "Parallel Execution Time: " << t_Stop << " seconds." << endl;
    cout<<t_Stop;
}

void deleteVector()
{
  delete [] A;
  delete [] B;
  delete [] C;
}