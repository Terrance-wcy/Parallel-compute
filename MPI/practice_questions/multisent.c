#include <stdio.h>
#include "mpi.h"
int main(int argc, char *argv[])
{
	int rank, value, size;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	do {
		if (rank==0) {
			fprintf(stderr, "\n\nPlease give new value = ");
			scanf("%d",&value);
			fprintf(stderr, "\n%d read <-from ( %d )",rank,value);
			if (size>1) {
				MPI_Send(&value, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
				fprintf(stderr, "\n%d send ( %d ) to-> %d", rank,value,rank+1);
			}
		}
		

	else{
		MPI_Recv(&value, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
		fprintf(stderr, "\n%d receive( %d )<-from %d",rank, value, rank-1);
		if (rank<size-1) {
			MPI_Send(&value, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
			fprintf(stderr, "\n%d send ( %d ) to-> %d", rank, value, rank+1);
		}
       	}
	MPI_Barrier(MPI_COMM_WORLD);
      }while(value>0);
      MPI_Finalize();
      printf("\n");
}
 

