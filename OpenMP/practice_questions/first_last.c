#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREAD 8

int main() {
    int i;
    int k = 100;
    omp_set_num_threads(NUM_THREAD);
    printf("This is an example about firstprivate\n");
#pragma omp parallel for firstprivate(k)
    for (i = 0; i < NUM_THREAD; i++)
    {
        k += i;
        printf("k=%d from thread %d \n", k, omp_get_thread_num());
    }
    printf("last k =%d\n", k);
    k = 100;

    printf("This is an example about lastprivate\n");
#pragma omp parallel for lastprivate(k)
    for (i = 0; i < NUM_THREAD; i++)
    {
        k += i;
        printf("k=%d from thread %d \n", k, omp_get_thread_num());
    }
    printf("last k =%d\n", k);
    k = 100;

    printf("This is an example about private\n");
#pragma omp parallel for private(k)
    for (i = 0; i < NUM_THREAD; i++)
    {
        k += i;
        printf("k=%d from thread %d \n", k, omp_get_thread_num());
    }
    printf("last k =%d\n", k);
    k = 100;
    omp_set_num_threads(NUM_THREAD);
    printf("This is an example about shared\n");
#pragma omp parallel for shared(k)
    for (i = 0; i < NUM_THREAD; i++)
    {
        k += i;
        printf("k=%d from thread %d \n", k, omp_get_thread_num());
    }
    printf("last k =%d\n", k);
}