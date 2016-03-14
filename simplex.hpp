#ifndef _simplex_hpp_
#define _simplex_hpp_
#include <stdio.h>
#include <iostream>

using namespace std;

struct simplexAnswer{
    int state; //解的状态：0 有唯一最优解，1 有无穷多最优解，2 有无界解，3 无可行解
    float answer;
    float *vec;
};

typedef struct simplexAnswer simplexAnswer;
//大M法
//输入的目标函数为求max，约束条件左端必须为非负数，所有的变量保证为非负数
//输入，n 变量个数， m 控制条件个数， *target_vec 目标函数向量, old_mat 约束条件左端, b 约束条件右端
//op 约束条件的符号向量，0 小于等于，1 等于，2大于等于
simplexAnswer Simplex(int n, int m, float target_vec[], float *old_mat, float b_old[], int op[]){
    int M = -100000;
    simplexAnswer ret; //返回值
    float **mat;  //新的条件约束矩阵
    float *b;
    int i, j, k;
    int sc, rg; //松弛变量，人工变量
    //cb
    /*
    float *cb = new float[m];
    for (i = 0; i<m; i++){
    cb[i] = 0;
    }*/

    b = new float[m];

    for (i = 0; i < m; i++){
        b[i] = b_old[i];
    }

    sc = 0;
    for (i = 0; i<m; i++){
        if (op[i] != 1)  //如果不是等号，则需要添加松弛变量
            sc++;
    }
    rg = 0;
    for (i = 0; i<m; i++){
        if (op[i] != 0)  //如果不是小于等于，则需要添加人工变量
            rg++;
    }
    //基
    int *ji = new int[m];

    mat = new float*[m];
    for (i = 0; i<m; i++){
        mat[i] = new float[m + sc + rg];
    }
    //复制原约束条件
    for (i = 0; i<m; i++){
        for (j = 0; j<n; j++){
            mat[i][j] = old_mat[i*n + j];
        }
    }
    //添加松弛变量
    k = n;
    for (i = 0; i<m; i++){
        for (j = n; j<n + sc; j++){
            mat[i][j] = 0;
        }
        if (op[i] == 0){
            ji[i] = k;
            mat[i][k] = 1;
            k++;
        }
        else if (op[i] == 2){
            mat[i][k] = -1;
            k++;
        }
    }
    k = n + sc;
    //添加人工变量
    for (i = 0; i<m; i++){
        for (j = n + sc; j<n + sc + rg; j++){
            mat[i][j] = 0;
        }
        if (op[i] == 1){
            ji[i] = k;
            mat[i][k] = 1;
            k++;
        }
        else if (op[i] == 2){
            ji[i] = k;
            mat[i][k] = 1;
            k++;
        }
    }
    //检验数
    float *jianyan = new float[m + sc + rg];
    for (i = 0; i<m + sc + rg; i++){
        jianyan[i] = 0;
        if (i<n){
            jianyan[i] += target_vec[i];
        }
        else if (i >= n + sc) {
            jianyan[i] += M;
        }
        for (j = 0; j < m; j++){
            if (ji[j] >= n + sc){
                jianyan[i] -= mat[j][i] * M;
            }
        }
    }


    //开始循环
    int flag;
    float maxJ; //最大的检验数
    int swapin; //换入变量
    float minB;
    int swapout; //换出行
    float tmp;
    while (1) {
        //判断检验数并且确定换入变量
        flag = 0;
        maxJ = 0;
        for (i = 0; i<n + sc + rg; i++){
            if (jianyan[i]>0){
                flag = 1;
                if (jianyan[i]>maxJ){
                    maxJ = jianyan[i];
                    swapin = i;
                }
            }
        }
        //如果flag=0,则检验数都小于等于0，跳出循环
        if (flag == 0)
            break;



        //确定换出变量
        swapout = -1;
        for (int i = 0; i<m; i++){
            if (mat[i][swapin]>0){
                if (swapout == -1){
                    minB = b[i] / mat[i][swapin];
                    swapout = i;
                }
                else if (b[i] / mat[i][swapin] < minB){
                    minB = b[i] / mat[i][swapin];
                    swapout = i;
                }
            }
        }
        //如果找不到换出变量则退出，说明有非基变量检验数大于0，但它所对应的系数列向量均小于等于0
        if (swapout == -1)
            break;
        //更新基
        ji[swapout] = swapin;
        //cb[swapout] = jianyan[swapin];
        //更新检验数和矩阵
        tmp = mat[swapout][swapin];
        for (i = 0; i<m + sc + rg; i++){
            mat[swapout][i] = mat[swapout][i] / tmp;
        }
        b[swapout] = b[swapout] / tmp;
        for (i = 0; i<m; i++){
            if (i != swapout){
                tmp = mat[i][swapin];
                b[i] = b[i] - b[swapout] * tmp;
                for (j = 0; j<m + sc + rg; j++){
                    mat[i][j] = mat[i][j] - mat[swapout][j] * tmp;
                }
            }
        }
        tmp = jianyan[swapin];
        for (i = 0; i<m + sc + rg; i++){
            jianyan[i] = jianyan[i] - mat[swapout][i] * tmp;
        }
    }


    //判断，如果换出变量为-1，则任意一个大于零的非基变量的检验数，其对应的系数是否都小于等于0，则解无界
    if (swapout == -1){
        ret.state = 2;
        for (i = 0; i<m; i++){
            //delete[] mat[i];
        }
        delete mat;
        return ret;
    }

    for (i = 0; i<m; i++){
        //delete[] mat[i];
    }
    delete mat;

    //最优解向量
    float *vec = new float[n];
    for (i = 0; i<n; i++){
        vec[i] = 0;
    }
    //最优解
    float answer = 0;
    for (i = 0; i<m; i++){
        if (ji[i]<n){
            vec[ji[i]] = b[i];
            jianyan[ji[i]] = -1;    //更改基变量的检验数，用于后续的方便
            answer += b[i] * target_vec[ji[i]];
        }
    }
    //判断是否无解
    for (i = 0; i < m; i++){
        int sum = 0;
        for (j = 0; j < n; j++){
            sum += vec[j]*old_mat[i*n + j];
        }
        if (op[i] == 0 && sum > b_old[i]){
            ret.state = 3;
            return ret;
        }
        else if (op[i] == 1 && sum != b_old[i]){
            ret.state = 3;
            return ret;
        }
        else if (op[i] == 2 && sum < b_old[i]){
            ret.state = 3;
            return ret;
        }
    }
    ret.state = 0;
    for (i = 0; i<n + sc + rg; i++){
        if (jianyan[i] == 0){    //说明有不是基变量的检验数为0，则有无穷多个最优解
            ret.state = 1;
        }
    }
    ret.answer = answer;
    ret.vec = vec;
    return ret;
}



#endif
