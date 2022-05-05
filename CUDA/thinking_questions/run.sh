echo "编译并运行向量相加程序"
nvcc vector_add.cu -o vector_add
vector_add
echo "编译并运行A*B+C运算 运行向量到矩阵每一行欧氏距离"
nvcc kernel.cu -o kernel
kernel

