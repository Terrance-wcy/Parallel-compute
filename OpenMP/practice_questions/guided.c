#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREAD 2
int main()
{
	int i;
	omp_set_num_threads(NUM_THREAD);
	#pragma omp parallel for schedule(guided,2)
	for(i=0;i<30;i++)
	{
		printf("thread id：%d \t value:%d \n",omp_get_thread_num(),i);
	}
	return 0;
}
