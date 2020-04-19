#ifndef PASSI
#define PASSI 1000000
#endif
#include <iostream>
#include <cstdio>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iomanip>
#include <random>
using namespace std;
const double PI = 3.141592653589793238462643;
const unsigned PAD=8;
int num_steps = PASSI;
double dX = 1.0/(double)num_steps;
double t_Start;
double t_Stop;
void piCiclicPadding();
void piCiclicReduction();
void piCiclicMonteCarlo();

int main(int argc, char **argv)
{
    if(*argv[1]=='1')
       piCiclicPadding();
    else if(*argv[1]=='2')
       piCiclicReduction();
    else if(*argv[1]=='3')
       piCiclicMonteCarlo();
    return 0;
}

void piCiclicPadding()
{
    int NUM_THREADS= omp_get_max_threads();
    double pi=0.0,sum[NUM_THREADS][PAD];
    t_Start = omp_get_wtime();

    #pragma omp parallel
    {
        const unsigned id=omp_get_thread_num();
        sum[id][0]=0.0;
        
        for (unsigned i=id;i<num_steps;i+=NUM_THREADS){
            double area=(i+0.5)*dX;
            sum[id][0]+= 4.0/(1.0+area*area);
        }
    }
    
    for (unsigned i=0;i<NUM_THREADS;++i){
        pi+=sum[i][0]*dX;
    }
    
    t_Stop = omp_get_wtime() - t_Start;
    /*cout << "---------------PADDING---------------"<<endl;
    printf("The computed value of Pi is %2.24f\n", pi);
    printf("The exact value of Pi is %2.24f\n", PI);
    cout << "Parallel Execution Time: " << t_Stop << " seconds." << endl;
    cout << endl;*/
    cout<<t_Stop;
}

void piCiclicReduction()
{
    double pi=0.0, sum = 0.0, area=0.0;
    t_Start = omp_get_wtime();


    #pragma omp parallel for reduction(+:sum) private(area) schedule(static)
    for (unsigned i=0; i < num_steps ; i++) {
        area=(i+0.5)*dX;
        sum+= 4.0/(1.0+area*area);
    }
     
    pi = dX * sum;
    
    t_Stop = omp_get_wtime() - t_Start;
   /* cout << "--------------REDUCTION--------------"<<endl;
    printf("The computed value of Pi is %2.24f\n", pi);
    printf("The exact value of Pi is %2.24f\n", PI);
    cout << "Parallel Execution Time: " << t_Stop << " seconds." << endl;
    cout << endl;*/
    cout<<t_Stop;

}


void piCiclicMonteCarlo()
{

    int i, count=0, seed;
    struct drand48_data drand_buf;
    double x, y;
    double t0, t1;

    t_Start = omp_get_wtime();
    #pragma omp parallel private(i, x, y, seed, drand_buf) shared(num_steps)
    {
        seed = 1202107158 + omp_get_thread_num() * 1999;
        srand48_r (seed, &drand_buf);

        #pragma omp for reduction(+:count)
        for (i=0; i<num_steps; i++) {
            drand48_r (&drand_buf, &x);
            drand48_r (&drand_buf, &y);
            if (x*x + y*y <= 1.0) count++;
        }
    }

    t_Stop = omp_get_wtime() - t_Start;
    const double pi = 4.0 * (double) count / (double) num_steps;
   /* cout << "-------------MONTECARLO-------------"<<endl;
    printf("The computed value of Pi is %2.24f\n", pi);
    printf("The exact value of Pi is %2.24f\n", PI);
    cout << "Parallel Execution Time: " << t_Stop << " seconds." << endl;
    cout << endl;*/
    cout<<t_Stop;
}