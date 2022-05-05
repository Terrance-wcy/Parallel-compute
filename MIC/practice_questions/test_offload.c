#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <offload.h>
int main (int argc,char* argv[])
{
	float pi =0.0F;
	int count =10000;
	int i;
	#pragma offload target (mic:0)
	#pragma omp parallel for reduction(+:pi)
	for(i=0;i<count;i++)
	{
	float t =(float)((i+0.5f)/count);
	pi += 4.0f/(1.0f+t*t);
	}
	pi /=count;
	printf("pi=%f\n",pi);
}
