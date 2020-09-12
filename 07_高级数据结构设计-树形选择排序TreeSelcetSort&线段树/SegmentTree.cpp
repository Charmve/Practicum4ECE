/**********************2.线段覆盖问题（60分）***********************
Describe： 
		给定N个整数区间[Ai,Bi]，其中1<=Ai<Bi<=1000，可事先存在数组中。
	求：1到1000这1000个整数各自在N个区间中出现过几次。注意算法时间复杂
	度，逐个区间判断计数的方法不给分。（提示：百度线段树）
Author：Zhang Wei 
Time： 23/12/2017 
*******************************************************************/

#include <iostream> 
#include <stdlib.h>

using namespace std;

#define ERROR 0
#define OK 1 
#define MAXNUM 1000
#define N 1000

struct SegTreeNode
{
    int val;
    int count;
}segTree[MAXNUM];//定义线段树结点 

/*******************************************************************
功能：构建线段树
root：当前线段树的根节点下标
arr: 用来构造线段树的数组
qarr： 储存n个区间的数组
n：  区间个数 
istart：数组的起始位置
iend：数组的结束位置
********************************************************************/
void build(int root, int arr[], int qarr[], int n, int istart, int iend){
	//构建线段树
	int i; 
    if(istart == iend)//叶子节点
    {
        segTree[root].val = arr[istart];//根节点 
        segTree[root].count = 0;//初始化计数为零 
        for(i=0; i < 2*n ;i+=2){
        	if(qarr[i] < segTree[root].val && segTree[root].val < qarr[i+1]) {
        		segTree[root].count++;
			}	
		}  	
		cout << segTree[root].val <<"'s count is " << segTree[root].count << endl;
	}
		
    else
    {
        int mid = (istart + iend) / 2; 
        build(root*2+1, arr, qarr, n, istart, mid);//递归构造左子树
        build(root*2+2, arr, qarr, n, mid+1, iend);//递归构造右子树
        
        //根据左右子树根节点的值，更新当前根节点的值
        segTree[root].val = min(segTree[root*2+1].val, segTree[root*2+2].val);
    }
}//build

/*******************************************************************
功能：线段树的区间查询
root：当前线段树的根节点下标
[nstart, nend]: 当前节点所表示的区间
[qstart, qend]: 此次查询的区间
*******************************************************************/
int query(int root, int nstart, int nend, int qstart, int qend)
{
    //查询区间和当前节点区间没有交集
    if(qstart > nend || qend < nstart)
        return ERROR;
    //当前节点区间包含在查询区间内
    if(qstart <= nstart && qend >= nend)
        return segTree[root].val;
    //分别从左右子树查询，返回两者查询结果的较小值
    int mid = (nstart + nend) / 2;
    return min(query(root*2, nstart, mid, qstart, qend),
               query(root*2+1, mid + 1, nend, qstart, qend));

}//query

void CreatArr(int a[], int n){
	//以回车结束输入，建立n个区间
	char s;
	int i; 
	for (i = 0; i < 2*n && s != '\n'; i+=2)
	{
		scanf("%d %d",&a[i],&a[i+1]);//相邻两个元素表示n个区间范围 [1，2]、[3,4]...[i,i+1] 
		s = getchar();
	}
}//CreatArr

int main(void) 
{
	int i,n,arr[N]; 
	printf("需要多少个区间："); 
	scanf("%d ",&n);
	CreatArr(arr,n);//n个整数区间 
	cout<<endl<< "你所选择的区间是："<<endl; 
	for (i = 0; i < 2*n ; i+=2)
		cout << "[" << arr[i] << ", " << arr[i+1] << "] " ;
	cout << endl << endl;
	
	int Interval_Arr[1000];
	for(i = 0; i < 1000; ) 
		{
			Interval_Arr[i] = ++i;		
		}
	build(0, Interval_Arr, arr, n, 1, 1000);//1~1000构建一棵线段树
	
	return 0;	 
} 
