#ifndef PASSI
#define PASSI 1000000
#endif
#include <iostream>
#include <cstdio>
#include <omp.h>
#include <stdlib.h>
using namespace std;
const int N = PASSI;
int* A= NULL;
double t_Start;
double t_Stop;
void vectorSearchLinear(int);
int main()
{
    int search_num=100047675; //Enter a Value to search
    
    A = new int[N];
    int remplace=1;

    for(int i=0; i<N; i++){
        A[i]=i+1;
    }
     
    vectorSearchLinear(search_num);
    return 0;
}

void vectorSearchLinear(int key)
{
    bool found_it = false;
    const int chunk_size = (N / omp_get_max_threads());
    t_Start = omp_get_wtime();

    #pragma omp parallel shared (A,found_it)
    {
        int* chunk = &A[chunk_size * omp_get_thread_num()];
        #pragma omp cancellation point parallel

        for(int i=0; i<chunk_size && !found_it; i++)
        {
            if(__builtin_expect(chunk[i]==key, 0)){
                #pragma omp atomic write
                found_it = true;
                #pragma omp cancel parallel
            }
            #pragma omp cancellation point parallel
        }
    }

    t_Stop = omp_get_wtime() - t_Start;

   /* if (found_it){
        cout << "Found it in : " << t_Stop << " seconds."<< endl;
    }
    else
        cout << "Not Found. "<< t_Stop << " seconds."<<endl;*/

    cout<<t_Stop;
}