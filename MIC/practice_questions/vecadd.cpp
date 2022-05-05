#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <offload.h>

#define N 10

	__attribute__((target (mic)))
void offload_check(void)
{
	#ifdef __MIC__
	printf("check func:Run on the mic!\n");
	#else
	printf("check func;Run on the cpu!\n");
	#endif
}

	__attribute__((target (mic)))
void VectorAdd(float* A, float* B,float* C,int n)
{
	offload_check();
	#pragma omp parallel
	{
		#pragma omp for 
		for (int i=0;i<n;i++)
		{
			C[i] = A[i]+B[i];
		}
	}
}

int main( int argc,char** argv)
{
	int i;
	int size=N * sizeof(float);
	float *A,*B,*C; 
	A=(float*)malloc(size);  
	B=(float*)malloc(size);
	C=(float*)malloc(size);  
	for (int i=0;i<N;i++)
	{
		A[i] = i+1;
		B[i] = i+11;
	}  
	#pragma offload target(mic) in(A,B:length(N)) out(C:length(N))
	{
		VectorAdd(A,B,C,N);
	}
	for (int i=0;i<N;i++)
	{
		printf("%d: %4.2f + %4.2f = %4.2f  \n",i,A[i],B[i],C[i]);
	}
	free(A);
	free(B);
	free(C);
}
