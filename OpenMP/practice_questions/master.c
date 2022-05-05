#include <stdio.h>
#include <stdlib.h>
#include<omp.h>
int main()
{
	int i=0;int a=0;
	int b[6];
	omp_set_num_threads(4);
#pragma omp parallel private(i) shared(a,b)
	{
#pragma omp master
	{
		a=a+10;
		printf("Master construct is executed by thread %d\n",omp_get_thread_num());
	}
#pragma omp barrier
#pragma omp for
		for(i=0;i<6;i++)
			b[i]=a;
	}
	printf("After parallel region:\n");
	for(i=0;i<0;i++)
		printf("b[%d]=%d\n",i,b[i]);
	return 0;

}
