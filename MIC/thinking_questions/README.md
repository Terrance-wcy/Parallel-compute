1.矩阵运算 D=A*B+C

源代码：matrix.cpp

生成文件：matrix.out

脚本文件：run.sh 运行此脚本可直接完成编译源代码和运行程序

run.sh内容：

	icpc matrix.cpp -openmp -o matrix.out
	./matrix.out



运行结果：

A[ i ] [ j ]=i+j    //71行 i,j都是从0开始 5*5矩阵

B[ i ] [ j ]=1+i+j    //72行

C[ i ] [ j ]=2+i+j    //80行

先输出A*B

再输出A*B+C

