# integer-programming
- use the simplex to solve the problem of linear programming
- use the branch-and-bound to solve the problem of integer-programming

程序的输入数据是在data.in中，程序通过读入data.in中的数据然后运用分支定界来解决整数规划问题，最终将结果输出到ans.out中。  
线性规划的范式为：  
  Max Cx  
	s.t. AX <= b  
	      X >=0  
先来看输入文件  
  2 2 //第一个数字为变量数，第二个数字为约束数  
  3 2 //这一行代表目标函数上各变量对应的系数矩阵C  
  2 1 2 3 //这一行代表矩阵A的系数  
  9 14 //这一行代表矩阵b  
  0 0 //这一行代表每个约束中的不等情况，0代表小于等于，1代表等于，2代表大于等于  
然后是输出文件：  
	14 //代表最优解对应的目标函数值  
  4 1 //代表最优解  
