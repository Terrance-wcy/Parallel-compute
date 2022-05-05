#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void main()
{	
	int i;
	int n=2019;
	int ic=0;
	
#pragma omp parallel for shared(ic,n) private(i)
	for(i=0;i<n;i++)
{
	#pragma omp atomic
		ic=2-ic;
	}

	printf("Counter=%d:\n",ic);
	
	}
