#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 4
void main()
{
int tid,i=0;
int sum,SUM=0;
int a[4];
for(i=0;i<4;i++)
a[i]=i;
#pragma omp parallel num_threads(NUM_THREADS) private(i,tid) firstprivate(sum) shared(SUM)
{
tid=omp_get_thread_num();
#pragma omp for
	for(i=0;i<4;i++)
{
sum += a[i];
}
#pragma omp critical
{
SUM+=sum;
printf("thread %d :sum=%d SUM=%d\n",tid,sum,SUM);}
}}

