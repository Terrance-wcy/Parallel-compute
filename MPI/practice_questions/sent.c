#include<stdio.h>
#include"mpi.h"
int main(int argc,char *argv[])
{
        int myid,size,value;
        MPI_Status status;

        MPI_Init(&argc,&argv);
        MPI_Comm_rank(MPI_COMM_WORLD,&myid);
        MPI_Comm_size(MPI_COMM_WORLD,&size);
       if(0==myid){
                fprintf(stderr,"please input a value ->");
                scanf("%d",&value);
                fprintf(stderr,"Process %d read from user %d\n",myid,value);

                MPI_Send(&value,1,MPI_INT,1,0,MPI_COMM_WORLD);
                fprintf(stderr,"Process %d send %d to Process %d \n",myid,value,myid+1);
        }
	else if (1==myid){
	MPI_Recv(&value,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
	fprintf(stderr,"Process %d receive %d from Process %d\n",myid,value,myid-1);
}
	MPI_Finalize();
}


