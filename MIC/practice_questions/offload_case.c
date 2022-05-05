#include<stdio.h>
#include<stdlib.h>
#define LEN (10)
int main(int argc, char* argv[])
{
	float* arr;
	int i;
	arr = (float*)malloc(LEN * sizeof(float));
	for (i = 0; i < LEN; ++i) {
		arr[i] = i;
	}
	i = 0;
#pragma offload target (mic:0) in(arr:length(LEN))
	for (i = 0; i < LEN; ++i) {
		printf("on mic:arr[%d]=%f\n", i, arr[i]);
		arr[i] = 0;
	}for (i = 0; i < LEN; ++i) {
		printf("on cpu:arr[%d]=%f\n", i, arr[i]);
}
	return 0;
}
