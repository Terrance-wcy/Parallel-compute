#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
	int i;
	#pragma omp parallel for schedule(static,2)
		for(i=0;i<10;i++)
		{
			printf("thread id:%d \t value:%d \n",omp_get_thread_num(),i);
		}
	return 0;
}
