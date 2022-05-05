#include <stdio.h>
#include <math.h>

int main (int argc,char*argv[])
{
	float pi =0.0f;
	int count =10000;
	int i;
	for(i=0;i<count;i++)
	{
		float t = (float)((i+0.5f)/count);
		pi += 4.0f/(1.0f+t*t);
	}
	pi /= count;
	printf("Pi = %f\n",pi);
}
