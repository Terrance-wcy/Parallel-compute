#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

double f(double);

int main(int argc, char *argv[])
{
	int rank, size;
	int n; 
	double pi; 
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if (0==rank) {
		printf("%d\n",size);
		printf("number of small rectangle:\n");
		scanf("%d",&n);
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); 
	int i;
	double x, h, sum=0.0;
	for (i=rank+1; i<=n; i+=size)
	{
		x = (i-0.5)/(double)n;
		sum += f(x);
	}
	sum = sum/n;
	MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
	if (0==rank) {
		printf("Approximately of pi is : %.16f\n",pi);
		fflush(stdout);
	}
	MPI_Finalize();
}

double f(double x) { return 4.0/(1.0+x*x); }
