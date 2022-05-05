#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREAD 8

int main(){
	omp_set_num_threads(NUM_THREAD);
int val=8;
int i=0;
#pragma omp parallel for lastprivate(val)
for(i=0;i<3;i++)
{
	    printf("i1=%d val=%d\n",i,val);
	    if(i==2)
		      val=10000;
	    printf("i2=%d val=%d\n",i,val);
}
printf("val=%d\n",val);
return 0;
}
