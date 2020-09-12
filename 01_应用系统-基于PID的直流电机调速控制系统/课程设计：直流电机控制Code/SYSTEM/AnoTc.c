#include "AnoTc.h"
#include	"Serial.h"//串口1(P3.0/RxD, P3.1/TxD)
#include "config.h"
#include <myself.h>

//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
	
u16 testdatatosend[50];	//发送数据缓存
void TestSendData(u16 *dataToSend , u8 length)//数据发送函数
{
	u8 i;
	for(i=0;i<length;i++)
	{
		S1SendData(dataToSend[i]);
		DelayNms(2);
		//while((USART1->SR&0X40)==0);
	}
}

void Test_Send_User(unsigned int data1, unsigned int data2, unsigned int data3)	
{	//发送用户数据，这里有3个数据，分别对应本教程3个数据
	unsigned char _cnt=0;
	unsigned char i;
	unsigned char sum = 0;	//以下为计算sum校验字节，从0xAA也就是首字节，一直到sum字节前一字节
	
	testdatatosend[_cnt++]=0xAA;//0xAA为帧头
	testdatatosend[_cnt++]=0x05;//0x05为数据发送源，具体请参考匿名协议，本字节用户可以随意更改
	testdatatosend[_cnt++]=0xAF;//0xAF为数据目的地，AF表示上位机，具体请参考匿名协议
	testdatatosend[_cnt++]=0xF1;//0xF1，表示本帧为F1用户自定义帧，对应高级收码的F1功能帧
	testdatatosend[_cnt++]=0;//本字节表示数据长度，这里先=0，函数最后再赋值，这样就不用人工计算长度了
 
	testdatatosend[_cnt++]=BYTE0(data1);//将要发送的数据放至发送缓冲区
	testdatatosend[_cnt++]=BYTE1(data1);
	
	testdatatosend[_cnt++]=BYTE0(data2);//将要发送的数据放至发送缓冲区
	testdatatosend[_cnt++]=BYTE1(data2);
	
	testdatatosend[_cnt++]=BYTE0(data3);
	testdatatosend[_cnt++]=BYTE1(data3);
 
	testdatatosend[4] = _cnt-5;//_cnt用来计算数据长度，减5为减去帧开头5个非数据字节
	

	for(i=0;i<_cnt;i++)
		sum += testdatatosend[i];
	
	testdatatosend[_cnt++]=sum;	//将sum校验数据放置最后一字节
 
	TestSendData(testdatatosend, _cnt);	//调用发送数据函数
}
 
 
 
 
//static void Loop_1000Hz(void)//1ms执行一次
//{
//	static u16 cnt = 0;
//	cnt++;
//	Test_Send_User(cnt,12,-123);//发送用户数据


//}
