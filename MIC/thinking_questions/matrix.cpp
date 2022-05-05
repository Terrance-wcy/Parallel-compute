#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <offload.h>

#define N 5
#define M 5

 __attribute__((target(mic)))
void offload_check(void)
{
 #ifdef __MIC__
 printf("check func:Run on the mic!\n");
 #else
 printf("check func;Run on the cpu!\n");
 #endif
}

 __attribute__((target(mic)))
void MatAdd(float* A, float* B,int n,int m)
{
offload_check();
#pragma omp parallel //num_threads(set_omp_num_threads)
 {
#pragma omp for
  for (int i = 0; i < n; i++)
  {
   for (int j = 0; j < m; j++) {
    B[i*m+j] = A[i*m+j] + B[i*m+j];
   }
  }
 }
}

 __attribute__((target (mic)))
void MatMulti(float* A,float* B,float* C,int n,int m)
{
 offload_check();
 #pragma omp parallel //num_threads(set_omp_num_threads)
 {
  #pragma omp for 
  for (int i = 0; i < n; i++)
  {
   for (int j = 0; j < n; j++)
   {
    C[i*n+j] = 0.0;
    for (int k = 0;k < m; k++)
    {
     C[i*n+j] += A[i*m+k] * B[k*n+j];    
    }
   }
  }
 }
}

int main( int argc,char** argv)
{
 int i,j;
 float* n1;
 float *A,*B,*C,*D; 

 A = (float*)malloc(sizeof(float*)*N*M);  
 B = (float*)malloc(sizeof(float*)*M*N);  
 C = (float*)malloc(sizeof(float*)*N*N);  
 D = (float*)malloc(sizeof(float*)*N*N);  
 
 for (int i = 0; i < N; i++)
 {
  for (int j = 0;j < M; j++)
  {
   A[i*M+j] = i+j;
   B[j*N+i] = 1+i+j;
  }
 }  

 for (int i = 0; i < N; i++)
 {
  for (int j = 0;j < N; j++)
  {
   C[i*N+j] = 2+i+j;
  }
 } 
 
 for (int i = 0; i < N; i++){
  for (int j = 0; j < N; j++) {
   D[i*N+j] = 0;
  }
 }
 
 int NN = N*N;
 int NM = N*M;
 #pragma offload target(mic) in(A:length(NN)) in(B:length(NM)) out(D:length(NN))
 {
  MatMulti(A,B,D,N,M);
 }
 printf("A*B=\n");
 for (int i = 0; i < N; i++)
 {
  for (int j = 0;j < N; j++)
   printf("%f ", D[i*N+j]);
  printf("\n");
 }
 
// offload_check();
 #pragma offload target(mic) in(C:length(NN)) inout(D:length(NN))
 {
  MatAdd(C,D,N,N);
 }
 printf("A*B+C=\n");
  for (int i = 0; i < N; i++)
  {
   for (int j = 0;j < N; j++)
    printf("%f ", D[i*N+j]);
   printf("\n");
  }

 free(A);
 free(B);
 free(C);
 free(D);
}
