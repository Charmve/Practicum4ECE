/**********************1.锦标赛排序算法（40分）***********************
Describe： 
	自主编写函数实现树形选择排序。要求实现任意个整数的排序。
Author：Zhang Wei 
Time： 15/12/2017 
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MAX_VALUE 99
#define N 100

/**   
     * ---------------------------------------   
     * 树形选择排序 ：  
     * 对于简单排序来说，主要是进行n-1趟元素的比较，每趟比较n-2次，  
     * 每趟比较取出一个最小值(也可以是最大值)，逐步使列表有序。  
     * 但是第一趟的比较是可以为后续的比较提供信息的，使后续的比较次数大大减少，  
     * 而后续的比较又可以为更后续的比较提供信息，这样就减少了比较的次数，减少了  
     * 时间复杂度。  
     *   
     * 实现原理：  
     * 第一步，首先对n个记录进行两两比较，得到较小的n/2个数再依次比较，依次类推  
     * 直到得到一个最小值,这是一个构造完全二叉树的过程，根节点即为最小元素，叶子节点为列表元素。  
     * 构造的此树的存储结构可以用数组表示方法，数组长度为2n-1。填充此树，比如  
     * 列表元素为：19    38     65    97   76    13    27   49  
     * 构造的树为：                13  
     *                     38               13  
     *                38       65       13       27  
     *              19  38   65  97   76  13   27  49  
     * 13为根结点位最小值，列表元素为叶子节点  
     *   
     * 第二步，移走最小元素，此时可重新为数组a的第一个位置赋值为此最小值，  
     * 之后如果找出次小值则可以为第二个位置赋值，......  
     *   
     * 第三步，找出次小值，找出最小值在叶子节点的位置，从该节点开始，和其兄弟节点  
     * 进行比较，修改从叶子节点到根节点的元素值，比较完毕后，根节点为次小值。  
     * 第三步比较是利用了第一次比较提供的信息，因为第一步已经得到了两两比较的  
     * 较小值，只要拿第一次与最小值比较的元素(即最小值的兄弟节点)与它们比较即可得最小值。  
     * 即拿上述例子的76与27比较，然后27与38比较得到次小值27。  
     * 重复第二和第三步，排序完成。  
     *   
     * PS:这里把移出去的叶子节点都要重设为最大值，可对此方法进行稍微改动  
     * 可传一个最大值进来，这里是整型所以用了Integer.MAX_VALUE  
     */  
	     
int TreeSelectionSort(int mData[],int len) 
{  
    int TreeLong = len * 4;  
    int MinValue = -10000;  
    int tree[TreeLong]; // 树的大小  
    int baseSize;  
    int i;  
    int max;  
    int maxIndex;  
    int treeSize;  
    baseSize = 1;  
      
    while (baseSize < len)   
        baseSize *= 2;   
    treeSize = baseSize * 2 - 1;  
      
    for (i = 0; i < len; i++)   
        tree[treeSize - i] = mData[i];  
      
    for (; i < baseSize; i++)  
        tree[treeSize - i] = MinValue;  
      
    // 构造一棵树  
    for (i = treeSize; i > 1; i -= 2) {  
        tree[i / 2] = (tree[i] > tree[i - 1] ? tree[i] : tree[i - 1]);  
    }  
    
    len -= 1;  
    while (len != -1) {  
        max = tree[1];  
        mData[len--] = max;  
        maxIndex = treeSize;  
        while (tree[maxIndex] != max)  
            maxIndex--;  
          
        tree[maxIndex] = MinValue;  
        while (maxIndex > 1) {  
            if (maxIndex % 2 == 0) {  
                tree[maxIndex / 2] = (tree[maxIndex] > tree[maxIndex + 1] ? tree[maxIndex]  
                        : tree[maxIndex + 1]);  
            } 
			else {  
                tree[maxIndex / 2] = (tree[maxIndex] > tree[maxIndex - 1] ? tree[maxIndex]  
                        : tree[maxIndex - 1]);  
            }  
            maxIndex /= 2;  
        }  
    }  
    //return mData;  
}

/*void TreeSelectionSort(int a[],int len)
{  
    int treeSize = 2 * len - 1;  //完全二叉树的节点数    
    int low = 0;
	int i,j;    
    int tree[treeSize];    //临时的树存储空间    
    //由后向前填充此树，索引从0开始    
    for(i = len-1,j=0 ;i >= 0; --i,j++)
	    //填充叶子节点    
        tree[treeSize-1-j] = a[i];        
        
    for(j = treeSize-1;j>0;j-=2) 
		//填充非终端节点    
        tree[(j-1)/2] = (tree[j-1] < tree[j] ? tree[j-1]:tree[j]);        
        
    //不断移走最小节点    
    int minIndex;    
    while(low < len){    
        int min = tree[0];    //最小值    
        a[low++] = min;    
        minIndex = treeSize-1;       //找到最小值的索引    
        while(tree[minIndex] != min)    
            minIndex--;    
       
        tree[minIndex] = MAX_VALUE;  //设置一个最大值标志    
        //找到其兄弟节点    
        while(minIndex > 0){      //如果其还有父节点    
            if(minIndex % 2 == 0){   //如果是右节点    
                tree[(minIndex-1)/2] = (tree[minIndex-1] < tree[minIndex]) 
				? tree[minIndex-1]:tree[minIndex];    
                minIndex = (minIndex-1)/2;    
            }
			else{                   //如果是左节点    
                tree[minIndex / 2] = (tree[minIndex] < tree[minIndex+1])    
                 ? tree[minIndex]:tree[minIndex+1];    
                 minIndex = minIndex/2;    
            }    
        }    
            
    }    
}    
*/

/* Function to print an array */
void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int CreatArr(int a[]){
	//以回车结束输入，建立一个待排序数组 
	char s;
	int i,c = 0; 
	for (i = 0; i < N && s != '\n';i++)
	{
		scanf("%d",&a[i]);
		s = getchar();
		c++;
	}
	return c;
}

	//输入输出格式：
	//输入: n arr(1),arr(2),arr(3)...arr(n)
	//输出： Orignial array:
	//       arr(1),arr(2),arr(3)...arr(n)
	//       Sorted array:
	//       .... 
	/*
			2
			3 4 6 2 5 7 10
			Orignial array:
			3 4 6 2 5 7 10
			Sorted array:
			2 3 4 5 6 7 10
			
			109 34 27 238 35 94
			Orignial array:
			109 34 27 238 35 94
			Sorted array:
			27 34 35 94 109 238 
			
    */

void main() 
{  
	// TODO Auto-generated method stub  
	//int arr[] = { 38, 62, 35, 77, 55, 14, 35, 98 };
	int n,l,j;
	int arr[N];
	
	scanf("%d ",&n); //测试次数 
	for(j = 0; j < n; j++)
	{
		l = CreatArr(arr);	
		//int l = sizeof(arr)/sizeof(arr[0]);
	
		printf("Orignial array: \n");
		printArray(arr, l);
		
		TreeSelectionSort(arr,l);  
	    printf("Sorted array: \n");
	    printArray(arr, l);
		
		printf("\n");  
	}
}  

