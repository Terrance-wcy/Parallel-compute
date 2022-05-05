#include "omp.h"
#include <stdio.h>
#include <math.h>
void main(int argc, char *argv[])
{
          int i;
      
          omp_set_num_threads(NUM_THREADS)
        #pragma omp parallel for
	for(i-0,i<=NUM_THREADS;i++)
	{
		printf("hello world from thread %d \n",omp_get_thread_num())
        }   
	printf("Hello from master thread %d \n,omp_get_thread_num());
          
}
