
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<cuda_runtime.h>
#include<device_launch_parameters.h>
using namespace std;
 
#define M 33
#define N 33
#define I 33
#define block_size 32
 
#define size_a M*I*sizeof(float)
#define size_b I*N*sizeof(float)
#define size_c M*N*sizeof(float)
 
__global__ void matrix_kernel(float *d_a,float *d_b,float *d_c,int wA,int wB);//wA,wB是A,B矩阵的宽度，也就是列数
void showResult(float *c,int h,int Kw,int w);
void init(float *a,int h,int Kw,int w,int value);//Kw为扩充后矩阵宽度，而w为实际矩阵宽度
//考虑将矩阵补齐，就是行列都变成32*32块的整数倍,函数功能是补齐并初始化,还要分配空间
float* supply(int h,int w,int value);//参数为行，宽，就可以得到扩充后的大小,value为需要初始化的值
int main()
{
 
	float *h_a,*h_b,*h_c;//声明主机指针
	float *d_a,*d_b,*d_c;//声明设备指针
 
	//在考虑补齐的时候,实际行数列数都发生了变化,记住最后的矩阵大小为row_c*col_c,但是实际矩阵大小为M*N即可
	int row_a=32*((M+32-1)/32);//补齐后a的行列，但是从[32*M/32,M)为初始化数值，而[M,row_a)则应初始化为零
	int col_a=32*((I+32-1)/32);//[I,col_a)初始化为零
	cout<<"a矩阵扩充后的行列分别为："<<row_a<<" "<<col_a<<" "<<(M+31)/32<<" "<<32*((M+32-1)/32)<<endl;
	int row_b=col_a;//[I,row_b)行，初始化为0
	int col_b=32*((N+32-1)/32);//[N,col_b）列初始化为0
	int row_c=row_a;
	int col_c=col_b;
	cout<<"c矩阵扩充后的行列分别为："<<row_c<<" "<<col_c<<" "<<(M+31)/32<<" "<<32*((M+32-1)/32)<<endl;
	cout<<"b矩阵扩充后的行列分别为："<<row_b<<" "<<col_b<<" "<<(M+31)/32<<" "<<32*((M+32-1)/32)<<endl;
	int size_a_a=row_a*col_a*sizeof(float);//补齐之后的矩阵所占用空间
	int size_b_b=row_b*col_b*sizeof(float);
	int size_c_c=row_c*col_c*sizeof(float);
	cout<<"a,b,c矩阵扩充后所占用空间："<<size_a_a<<" "<<size_b_b<<" "<<size_c_c<<endl;
 
	h_a=supply(row_a,col_a,1);//分配空间并初始化
	//显示扩充后a矩阵
	showResult(h_a,row_a,col_a,col_a);
	h_b=supply(row_b,col_b,2);
	showResult(h_b,row_b,col_b,col_b);
	h_c=supply(row_c,col_c,0);
	showResult(h_c,row_c,col_c,col_c);
	/*
	h_a=(float*)malloc(size_a);init(h_a,M,I,1);//主机上分配空间并初始化
	h_b=(float*)malloc(size_b);init(h_b,I,N,2);showResult(h_a,M,I);cout<<"a矩阵输出完成！"<<endl;showResult(h_b,I,N);cout<<"b矩阵输出完成"<<endl;
	h_c=(float*)malloc(size_c);memset(h_c,0,size_c);
	*/
	if(h_a==NULL||h_b==NULL||h_c==NULL)
	{
		fprintf(stderr,"分配主机内存失败!\n");
		exit(EXIT_FAILURE);
	}
 
	//设备上分配空间并检查
	cudaError_t err=cudaSuccess;//设定成功标志码
	err=cudaMalloc((void**)&d_a,size_a_a);
	cudaMalloc((void**)&d_b,size_b_b);
	cudaMalloc((void**)&d_c,size_c_c);cudaMemset(d_c,0,size_c_c);
	/*
	err=cudaMalloc((void**)&d_a,size_a);
	cudaMalloc((void**)&d_b,size_b);
	cudaMalloc((void**)&d_c,size_c);cudaMemset(d_c,0,size_c);//只对c初始化为0
	*/
	if(err!=cudaSuccess)
	{
		fprintf(stderr,"分配设备内存a失败！\n");
		exit(EXIT_FAILURE);
	}
	//加入时间计时代码
	cudaEvent_t start,stop;//声明事件
	cudaEventCreate(&start);//创建事件
	cudaEventCreate(&stop);
	cudaEventRecord(start,0);//sample使用NULL流，应该和这里一样反正都是默认流的意思
	//#################GPU工作部分######################矩阵大小变化了，后面这些东西都要变
	cudaMemcpy(d_a,h_a,size_a_a,cudaMemcpyHostToDevice);//主机向device拷贝数据，对于错误的检查，就不加了，太麻烦
	cudaMemcpy(d_b,h_b,size_b_b,cudaMemcpyHostToDevice);
	dim3 dimsA(col_a,row_a);//这里始终是第一个参数为列，第二个参数为行
	dim3 dimsB(col_b,row_b);
	//dim3 grids((N+block_size-1)/block_size,(M+block_size-1)/block_size);//按照我的补齐思路其实也不用这样写，因为始终是齐的
														//这里一定要记住了，每次都犯错误，对于cuda中的dim3写法，三个元素（x,y,z）对应的就是列，行，高，
														//所以说在这里写个grid（32,64）表示32列，64行
	dim3 grids(col_c/block_size,row_c/block_size);
	dim3 threads(block_size,block_size);
	//调用kernel函数计算内积
	matrix_kernel<<<grids,threads>>>(d_a,d_b,d_c,dimsA.x,dimsB.x);
	err=cudaMemcpy(h_c,d_c,size_c_c,cudaMemcpyDeviceToHost);//将结果数据拷贝回主机内存
	if(err!=cudaSuccess)
	{
		fprintf(stderr,"拷贝数据回主机失败！\n");
		exit(EXIT_FAILURE);
	}
	cudaEventRecord(stop,0);//将记录结束时间放入GPU工作队列中
	cudaEventSynchronize(stop);//CPU等待GPU完成所有工作后，再继续执行
	float totalTime=0.0f;//定义存储时间的变量，使用float型
	cudaEventElapsedTime(&totalTime,start,stop);
	//至此所有工作全部完成，剩下就是输出和释放了
	printf("GPU计算时间为：%f\n",totalTime);
	showResult(h_c,M,col_c,N);//显示输出结果
	cout<<M<<'\t'<<N<<endl;
	showResult(h_c,row_c,col_c,col_c);//显示全部的大矩阵
	cout<<row_c<<'\t'<<col_c<<endl;
	//释放资源
	free(h_a);free(h_b);free(h_c);
	cudaFree(d_a);cudaFree(d_b);cudaFree(d_c);
	cudaEventDestroy(start);cudaEventDestroy(stop);	
	return 0;
}
//这里核函数的线程调度为block（32,32） grid（20，10），即grid是20列，10行 
__global__ void matrix_kernel(float *d_a,float *d_b,float *d_c,int wA,int wB)
{
	//block index
	int bx=blockIdx.x,by=blockIdx.y;
	//thread index
	int tx=threadIdx.x,ty=threadIdx.y;
	//这里只是考虑的是对齐的情况，对于每一个block而言，先得到block的初始指针位置aBegin实际上表示的是一种位置的偏移量，指针加上偏移量就得到值，本身不使用指针更方便
	int aBegin=by*block_size*wA;//其实就是by*32*I，I就是A的宽度，这就很显然了
	int aEnd=aBegin+wA-1;//其实aBegin和aEnd就是一行的首尾位置
	int aStep=block_size;//这里注意a的步幅是在for循环中的跨度，因为每次只是载入一个块，所以整行要分多个块，就是这块之间的距离
	int bBegin=bx*block_size;//画图看的话，很清楚。分清楚，对于不同的block有不同的aBegin和bBegin，与for循环中跨度区别
	int bStep=block_size*wB;
	float Csub=0;//每个线程都会计算出一个行列的相乘值出来，放在Csub中
	for(int a=aBegin,b=bBegin;a<aEnd;a+=aStep,b+=bStep)
	{
		__shared__ float as[block_size][block_size];
		__shared__ float bs[block_size][block_size];
		as[ty][tx]=d_a[a+ty*wA+tx];//这里其实本质上这样看：a+tx其实就是在多少列上，这样看也行，怎么看都行，或者看为a为起始位置，加上ty*wA其实就是再下移ty行的位置，在加上tx就可以了
		bs[ty][tx]=d_b[b+ty*wB+tx];//bs与as取值的规律本质上是一样的。仅仅只是矩阵不同，而导致宽度不同而已
		__syncthreads();//保证数据都载入shared
		//第二个for循环计算乘累加
		for(int i=0;i<block_size;i++)
		{
			Csub+=as[ty][i]*bs[i][tx];
		}
		__syncthreads();//做到这里就表示一个分块的小矩阵计算完成，再进入下一个分块的小矩阵
	}
	//当所有循环做完后，每个线程都得到一个Csub值，写入d_c相应位置即可
	int c=wB*by*block_size+bx*block_size;//c在这里其实表示的是每个block的起始位置，要得到每个thread的在grid中的位置，还要加上在block中的相对位置
	d_c[c+ty*wB+tx]=Csub;
}
//初始化数据函数,我采取的策略是对矩阵进行两次赋值，每次赋值的行列不相同，
void init(float *a,int h,int Kw,int w,int value)//w始终是扩充后矩阵的宽，也就是列数，h为高，也就是行数,
{
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
			a[i*Kw+j]=value;
	}
}
//显示结果函数
void showResult(float *c,int h,int Kw,int w)//h高度就是行，w宽度就是列
{
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			cout<<c[i*Kw+j]<<'\t';
		}
		cout<<endl;
	}
}
//补齐矩阵函数
float* supply(int h,int w,int value)
{
	float *p=(float*)malloc(h*w*sizeof(float));
	init(p,h,w,w,0);//先将全矩阵初始化为0,
	init(p,M,w,I,value);//再讲实际需要运算的部分初始化为value，这里需要扩充矩阵的宽度，和需要实际赋值的矩阵宽度
	return p;
}
