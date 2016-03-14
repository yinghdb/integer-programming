#include <fstream>
#include <stdlib.h> 
#include "simplex.hpp"
#include <cmath>
using namespace std;

ifstream fin("data.in");
ofstream fout("ans.out");
/*�ж��Ƿ�������*/
bool isInt(float f){
	if(f>=0)
		return (f-(int)f)<1e-6||(f-(int)f)<-0.999999;
	else
		return (-(f-(int)f))<1e-6||-(f-(int)f)<-0.999999;
}
/*�ж������Ƿ�ȫ������*/
bool isInt(float f[],int n){
	for(int i=0;i<n;i++){
		if(!isInt(f[i]))return false;
	}
	return true;
}

void BBB(float& lb,float xs[],int n, int m, float target_vec[], float *old_mat, float b[], int op[]);

/*��֧���編�������滮*/
simplexAnswer BB(int n, int m, float target_vec[], float *old_mat, float b[], int op[]) {
	float lb = -999999;//Ԥ���½�
	float xs[n];
	simplexAnswer ret;
	BBB(lb,xs,n, m, target_vec, old_mat, b, op);
	ret.state=0;
	ret.answer=lb;
	ret.vec=new float[n];
	for(int i=0;i<n;i++){
		ret.vec[i]=xs[i];
	}
	return ret;
}
/*�ݹ麯��*/
void BBB(float& lb,float xs[],int n, int m, float target_vec[], float *old_mat, float b[], int op[]){
	simplexAnswer ans = Simplex(n, m, target_vec, old_mat, b, op);
	if(((ans.state == 1 || ans.state == 0) && ans.answer <= lb)||ans.state==2||ans.state==3)
		return;//���޽����Ŀ��ֵС��lowerbound�����֦Ϊ��֦
	else{
		if(isInt(ans.vec,n)){//��x*Ϊ�����⣬��Lowerbound=CTX*
			lb=ans.answer;
			for(int i=0;i<n;i++){
				xs[i]=ans.vec[i];
			}
		}
		else {//��X*Ϊ���������������֦
			float *mat1=new float[n*(m+1)];
			float *mat2=new float[n*(m+1)];
			float *b1=new float[m+1];
			float *b2=new float[m+1];
			int *op1=new int[m+1];
			int *op2=new int[m+1];
			for(int i=0;i<m*n;i++){
				mat1[i]=old_mat[i];
				mat2[i]=old_mat[i];
			}
			for(int i=m*n;i<(m+1)*n;i++){
				mat1[i]=mat2[i]=0;
			}
			for(int i=0;i<m;i++){
				b1[i]=b2[i]=b[i];
			}
			for(int i=0;i<m;i++){
				op1[i]=op2[i]=op[i];
			}
			for(int i=0;i<n;i++){
				if(!isInt(ans.vec[i])){
					mat1[m*n+i]=mat2[m*n+i]=1;
					b1[m]=floor(ans.vec[i]+1);
					b2[m]=floor(ans.vec[i]);
					op1[m]=2;
					op2[m]=0;
					BBB(lb,xs,n,m+1,target_vec,mat1,b1,op1);//��֦
					BBB(lb,xs,n,m+1,target_vec,mat2,b2,op2);//��֦
					delete []mat1;
					delete []mat2;
					delete []b1;
					delete []b2;
					delete []op1;
					delete []op2;
					break;
				}
			}

		}
		return;
	}

}
int main(){
	/*int n=2,m=2;
	float t[]={3,2};
	float mat[]={
		2,1,
		2,3,


	};
	float b[]={
		 9,
		 14,


	};
	int op[]={0,0};*/
	float mat[10000];
	float t[10000];
	float b[10000];
	int op[10000];
	int k , l , m , n;
	fin >> n >> m;
	for (int i = 0; i < n; i++)
		fin >> t[i];
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			fin >> mat[i * m + j];
	for (int i = 0; i < m; i++)
		fin >> b[i];
	for (int i = 0; i < m; i++)
		fin >> op[i];
	simplexAnswer ans=BB(n,m,t,mat,b,op);
	if (ans.state == 0 || ans.state == 1){
		fout<<ans.answer<<endl;
		for (int i = 0; i < n; i++){
			fout<<ans.vec[i]<<' ';
		}
	}
	if (ans.state == 2){
		fout << "the answer is unbounded!" << endl;
	}
	if (ans.state == 3){
		fout << "no feasible solution!" << endl;
	}
	system("pause");
	return 0;
}
