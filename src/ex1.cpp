#ifndef PASSI
#define PASSI 100000
#endif
#include <iostream>
#include <cstdio>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
using namespace std;
int N = sqrt(PASSI);
int M = sqrt(PASSI);
const double PI = 3.141592653589793238462643;
double** A;
double** B;
double t_Start;
double t_Stop;
void  matrixCalculate();

int main()
{
    matrixCalculate();
    return 0;
}

void matrixCalculate()
{
    A = new double*[N];
    B = new double*[N];
    for ( int i=0; i<N; i++ ){A[i] = new double[M];}
    for ( int i=0; i<N; i++ ){B[i] = new double[M];}

    t_Start = omp_get_wtime();

    int i,j;

    #pragma omp parallel for private(i,j) schedule(dynamic)
    for ( i=0; i<N; i++ ){
        for ( j=0; j+8<M; j+=8){

            #define pow_2(v) (v*v);
            #define pow_6(v) (v*v*v*v*v*v);
            #define pow(v, p) pow_##p(v);

            A[i][j+0] = 15.0*pow(i, 2)+5.0*PI*pow(j+0, 6);
            A[i][j+1] = 15.0*pow(i, 2)+5.0*PI*pow(j+1, 6);
            A[i][j+2] = 15.0*pow(i, 2)+5.0*PI*pow(j+2, 6);
            A[i][j+3] = 15.0*pow(i, 2)+5.0*PI*pow(j+3, 6);
            A[i][j+4] = 15.0*pow(i, 2)+5.0*PI*pow(j+4, 6);
            A[i][j+5] = 15.0*pow(i, 2)+5.0*PI*pow(j+5 ,6);
            A[i][j+6] = 15.0*pow(i, 2)+5.0*PI*pow(j+6, 6);
            A[i][j+7] = 15.0*pow(i, 2)+5.0*PI*pow(j+7, 6);
            
            B[i][j+0] = A[i][j+0]*2.2;
            B[i][j+1] = A[i][j+1]*2.2;
            B[i][j+2] = A[i][j+2]*2.2;
            B[i][j+3] = A[i][j+3]*2.2;
            B[i][j+4] = A[i][j+4]*2.2;
            B[i][j+5] = A[i][j+5]*2.2;
            B[i][j+6] = A[i][j+6]*2.2;
            B[i][j+7] = A[i][j+7]*2.2;
        }

         for (; j<M; j++){
            A[i][j] = 15.0*pow(i, 2)+5.0*PI*pow(j, 6);
            B[i][j] = A[i][j]*2.2;

         }

    }

    t_Stop = omp_get_wtime() - t_Start;
    //cout << "Parallel Multiplication Time: " << t_Stop << " seconds." << endl;
    cout<<t_Stop;
}