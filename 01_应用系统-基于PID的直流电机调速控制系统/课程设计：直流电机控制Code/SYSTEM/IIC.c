#include  "IIC.h"
#include	"intrins.h"
#include	<STC15F2K60S2.h>
#include	"Serial.h"
#define nop _nop_();
#define IIC_ADDR 0x2A//硬件地址
sbit SCL=P0^6;
sbit SDA=P0^7;
// ------------------------------------------------------------
// IO口模拟I2C通信
// ------------------------------------------------------------


/*****************以下是对IIC总线的操作子程序***/
/*****************启动总线**********************/
void IICStart( )
{
	SCL=0;                  //
	SDA=1;	
	nop;nop;	
	SCL=1;
	nop;nop;nop;nop;nop;nop;nop;//
	SDA=0;
	nop;nop;nop;nop;nop;nop;nop;
	SCL=0;
                  //
}

/*****************停止IIC总线****************/
void IICStop( )
{
	SCL=0;                  //
	SDA=0;	
	nop;nop;	
	SCL=1;
	nop;nop;nop;nop;nop;nop;nop;//
	SDA=1;
	nop;nop;nop;nop;nop;nop;nop;
	SCL=0;
}



/***************对IIC总线产生应答*******************/
void IICACK( )
{
	SCL=0;                  
	SDA=0;//
	nop;nop;
	SCL=1;
	nop;nop;nop;nop;nop;nop;nop;
	SCL=0;

}

/**************检查应答位*******************/
//等待应答信号到来
//返回值: 0,接受应答失败
//        1,接受应答成功
bit IICReadAck( )
{
	unsigned char ucErrTime=0;

	SCL=0;                  
	SDA=1;//读IO先置高电平	
	nop;nop;
	SCL=1;
	nop;nop;nop;nop;
	while(SDA==1)
	{
		ucErrTime++;
		if(ucErrTime>100)
		{

			IICStop( );
			return(0);//未接收到应答
		}
	}
	nop;nop;	
	SCL=0;
	return(1);
}

/*****************不对IIC总线产生应答***************/
void IICNoAck( )
{
	
	SCL=0;                  
	SDA=1;//
	nop;nop;
	SCL=1;
	nop;nop;nop;nop;nop;nop;nop;
	SCL=0;

}

/*******************向IIC总线写数据*********************/
void IICSendByte(unsigned char sendbyte )
{
	unsigned char data j=8;

	for(;j>0;j--)
	   {
			 SCL=0;
			 sendbyte<<=1;        //无论C51怎样实现这个操作，始终会使CY=sendbyte^7;
			 SDA=CY;
			 nop;nop;
			 SCL=1;
			 nop;nop;nop;nop;nop;nop;nop;			 
	   }
	SCL=0;
}

/**********************从IIC总线上读数据子程序**********/
unsigned char IICReadByte( )
{
	unsigned char cReceiveData=0,i=8;
	SCL=0;
	SDA=1;//读IO先置高电平	
	nop;nop;
	while(i--)
	   {
			 SCL=1;
			 nop;nop;nop;nop;
			 cReceiveData=(cReceiveData<<1)|SDA;
			 SCL=0;
			 nop;nop;nop;nop;nop;nop
			 nop;nop;nop;nop;nop;nop
	   }
	return(cReceiveData);
}


unsigned int Read2Byte(unsigned char RegisterAddress)//
{
	unsigned int RegisterData;//存放读到的2个字节
  IICStart( ); //启始信号
	IICSendByte((IIC_ADDR<<1)|0 );//发送器件地址+写命令

	if(IICReadAck( )==1)		//等待应答
	{		

		
		IICSendByte(RegisterAddress );	//写寄存器地址
		if(IICReadAck( )==0) return(0);		//等待应答
		IICStart( );
		IICSendByte((IIC_ADDR<<1)|1 );//发送器件地址+读命令
		if(IICReadAck( )==0) return(0);		//等待应答
		RegisterData=IICReadByte( )<<8;//读取数据
		IICAck( );	//发送ACK
		RegisterData|=IICReadByte( );  //读取数据
		IICNoAck( );	//发送nACK
		IICStop( );			//产生一个停止条件
		return RegisterData;		
	}
	else//无应答，亮测试LED报警
	{
//		TestLed=0;
		return 0;	
		
	}
}