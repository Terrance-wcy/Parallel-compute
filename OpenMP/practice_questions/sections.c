#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void main()
{	
#pragma omp parallel sections num_threads(4)
	{
	#pragma omp section
		{
			printf("section 1 :in thread %d\n",omp_get_thread_num());
		}
	#pragma omp section
		{
			printf("section 2 :in thread %d\n",omp_get_thread_num());
		}
	#pragma omp section
		{
			printf("section 3 :in thread %d\n",omp_get_thread_num());
		}
	#pragma omp section
		{
			printf("section 4 :in thread %d\n",omp_get_thread_num());
		}
	}
	
}