#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define NUM_THREADS 3
void print_time(int TID,char *comment)
{
	time_t tp;
	time(&tp);
	printf("Thread %d %s barrier at %s",TID,comment,ctime(&tp));
}
int main()
{
	int tid;
#pragma omp parallel num_threads(NUM_THREADS) private(tid)
	{
		tid=omp_get_thread_num();
	if(tid<omp_get_num_threads()/2)
		system("sleep 5");
	print_time(tid,"before");
#pragma omp barrier
	print_time(tid,"after");
	}
	return 0;
}
