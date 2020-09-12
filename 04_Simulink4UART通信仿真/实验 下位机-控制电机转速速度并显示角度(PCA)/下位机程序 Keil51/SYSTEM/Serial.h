#ifndef		__SERIAL_H
#define		__SERIAL_H


#include	<config.h>


void Uart1Init(void);	//定时2作为波特率发生器，默认8位数据，
//void UartInit(void);
//串口1(P3.0/RxD, P3.1/TxD)
extern unsigned char cS1Rec[];
extern bit bS1Rec;
extern unsigned char receive;

void S1SendData(unsigned char dat);//查询方式发送
void S1SendString(unsigned char *s);
unsigned char S1ReceiveData(void);//查询方式接收
#endif