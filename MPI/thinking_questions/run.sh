echo "编译并运行随机生成4*4矩阵并计算矩阵乘积"
mpicc juzhen.c -o juzhen.out
mpirun -np 4 juzhen 
echo "编译并运行任意阶矩阵乘积"
mpicc juzhen2.c -o juzhen2.out
cmd=juzhen2
(echo "2 1"
sleep 1
echo "1 3"
sleep 1
echo "2 2"
sleep 1
echo "3 3 3"
)|$cmd
echo "编译并运行Jacobi迭代"
mpicc jacobi.c -o jacobi.out
mpirun -np 4 jacobi.out
