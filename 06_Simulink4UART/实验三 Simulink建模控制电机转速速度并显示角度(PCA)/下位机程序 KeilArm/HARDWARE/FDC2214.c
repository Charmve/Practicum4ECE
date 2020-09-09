#include  "FDC2214.h"
#include	<STC15F2K60S2.h>
#include	"Serial.h"
#include	"intrins.h"
sbit FDC2214SCL=P0^6;
sbit FDC2214SDA=P0^7;
unsigned long longFDC2214Data;
#define nop _nop_();

// ------------------------------------------------------------
// IO口模拟I2C通信

// ------------------------------------------------------------


/*****************以下是对IIC总线的操作子程序***/
/*****************启动总线**********************/
void FDC2214IICStart(void)
{
	FDC2214SCL=0;                  //
	FDC2214SDA=1;	
	nop;nop;	
	FDC2214SCL=1;
	nop;nop;nop;nop;nop;nop;nop;//
	FDC2214SDA=0;
	nop;nop;nop;nop;nop;nop;nop;
	FDC2214SCL=0;
                  //
}

/*****************停止IIC总线****************/
void FDC2214IICStop(void)
{
	FDC2214SCL=0;                  //
	FDC2214SDA=0;	
	nop;nop;	
	FDC2214SCL=1;
	nop;nop;nop;nop;nop;nop;nop;//
	FDC2214SDA=1;
	nop;nop;nop;nop;nop;nop;nop;
	FDC2214SCL=0;
}


/***************对IIC总线产生应答*******************/
void FDC2214IICACK(void)
{
	FDC2214SCL=0;                  
	FDC2214SDA=0;//
	nop;nop;
	FDC2214SCL=1;
	nop;nop;nop;nop;nop;nop;nop;
	FDC2214SCL=0;

}

/**************检查应答位*******************/
//等待应答信号到来
//返回值: 0,接受应答失败
//        1,接受应答成功
bit FDC2214IICReadAck(void)
{
	unsigned char ucErrTime=0;

	FDC2214SCL=0;                  
	FDC2214SDA=1;//读IO先置高电平	
	nop;nop;
	FDC2214SCL=1;
	nop;nop;nop;nop;
	while(FDC2214SDA==1)
	{
		ucErrTime++;
		if(ucErrTime>100)
		{

			FDC2214IICStop( );
			return(0);//未接收到应答
		}
	}
	nop;nop;	
	FDC2214SCL=0;
	return(1);
}

/*****************不对IIC总线产生应答***************/
void FDC2214IICNoAck(void)
{
	
	FDC2214SCL=0;                  
	FDC2214SDA=1;//
	nop;nop;
	FDC2214SCL=1;
	nop;nop;nop;nop;nop;nop;nop;
	FDC2214SCL=0;

}

/*******************向IIC总线写数据*********************/
void FDC2214IICSendByte(unsigned char sendbyte)
{
	unsigned char data j=8;

	for(;j>0;j--)
	   {
			 FDC2214SCL=0;
			 sendbyte<<=1;        //无论C51怎样实现这个操作，始终会使CY=sendbyte^7;
			 FDC2214SDA=CY;
			 nop;nop;
			 FDC2214SCL=1;
			 nop;nop;nop;nop;nop;nop;nop;			 
	   }
	FDC2214SCL=0;
}

/**********************从IIC总线上读数据子程序**********/
unsigned char FDC2214IICReadByte(void)
{
	unsigned char cReceiveData=0,i=8;
	FDC2214SCL=0;
	FDC2214SDA=1;//读IO先置高电平	
	nop;nop;
	while(i--)
	   {
			 FDC2214SCL=1;
			 nop;nop;nop;nop;
			 cReceiveData=(cReceiveData<<1)|FDC2214SDA;
			 FDC2214SCL=0;
			 nop;nop;nop;nop;nop;nop
			 nop;nop;nop;nop;nop;nop
	   }
	return(cReceiveData);
}


//连续读地址为：RegisterAddress的2个字节
unsigned int FDC2214Read2Bytes(unsigned char RegisterAddress)//
{
	unsigned int RegisterData;//存放读到的2个字节
  FDC2214IICStart(); //启始信号
	FDC2214IICSendByte((FDC2214_ADDR<<1)|0);//发送器件地址+写命令

	if(FDC2214IICReadAck()==1)		//等待应答
	{		

		FDC2214IICSendByte(RegisterAddress);	//写寄存器地址
		if(FDC2214IICReadAck()==0) return(0);		//等待应答

		FDC2214IICStart();
		FDC2214IICSendByte((FDC2214_ADDR<<1)|1);//发送器件地址+读命令
		if(FDC2214IICReadAck()==0) return(0);		//等待应答

		RegisterData=FDC2214IICReadByte()<<8;//读取数据
		FDC2214IICAck();	//发送ACK
		RegisterData|=FDC2214IICReadByte();  //读取数据
		FDC2214IICNoAck();	//发送nACK
		FDC2214IICStop();			//产生一个停止条件
		return RegisterData;		
	}
	else//无应答，亮测试LED报警
	{
//		TestLed=0;
		return 0;	
		
	}
}

///////////////////FDC2214设置函数////////////////////
/* *IIC写2个字节
 *RegisterAddress:寄存器地址
 *data1:数据1
 *data2:数据2
 *返回值:1      正常
 *       0  错误
*/
bit FDC2214SetParameters(unsigned char RegisterAddress,unsigned char ParaMSB,unsigned char ParaLSB)
{ 
  FDC2214IICStart(); //启始信号
	FDC2214IICSendByte((FDC2214_ADDR<<1)|0);//发送器件地址+写命令	
	if(FDC2214IICReadAck()==1)		//等待应答
	{		
		FDC2214IICSendByte(RegisterAddress);	//写寄存器地址
		if(FDC2214IICReadAck()==0) return(0);		//等待应答
		

		FDC2214IICSendByte(ParaMSB);//发送高位数据
		if(FDC2214IICReadAck()==0) return(0);		//等待应答

		FDC2214IICSendByte(ParaLSB);//发送低位数据
		if(FDC2214IICReadAck()==0) return(0);		//等待应答

		FDC2214IICStop();			//产生一个停止条件
		return (1);		//发送成功，返回1
	}
	else//无应答，亮测试LED报警
	{
//		TestLed=0;
		return 0;	
		
	}	
}


bit FDC2214Init(void)
{
  unsigned int res;
	//检测ID是否正确，再初始化寄存器
	res=FDC2214Read2Bytes(MANUFACTURER_ID);
	if(res==0x5449)
	{

//		//设置FDC2214寄存器
		//设置FDC2214SetParameters寄存器
		FDC2214SetParameters(RCOUNT_CH0,0x34,0xFB);//参考计数转换间隔时间(T=(RCOUNT_CH0*16)/Frefx)
		FDC2214SetParameters(RCOUNT_CH1,0x34,0xFB);
		FDC2214SetParameters(RCOUNT_CH2,0x34,0xFB);
		FDC2214SetParameters(RCOUNT_CH3,0x34,0xFB);
		
		FDC2214SetParameters(SETTLECOUNT_CH0,0x00,0x1B);//转换之前的稳定时间(T=(SETTLECOUNT_CHx*16)/Frefx)
		FDC2214SetParameters(SETTLECOUNT_CH1,0x00,0x1B);
		FDC2214SetParameters(SETTLECOUNT_CH2,0x00,0x1B);
		FDC2214SetParameters(SETTLECOUNT_CH3,0x00,0x1B);

		FDC2214SetParameters(CLOCK_DIVIDERS_C_CH0,0x20,0x02);//选择在0.01MHz ~ 10MHz的传感器频率
		FDC2214SetParameters(CLOCK_DIVIDERS_C_CH1,0x20,0x02);//Frefx = Fclk = 43.4MHz/2(2分频)
		FDC2214SetParameters(CLOCK_DIVIDERS_C_CH2,0x20,0x02);//CHx_REF_DIVIDER=2;CHx_FIN_SEL=2
		FDC2214SetParameters(CLOCK_DIVIDERS_C_CH3,0x20,0x02);
		
		FDC2214SetParameters(DRIVE_CURRENT_CH0,0x78,0x00);//0.146ma(传感器建立时间+转换时间的驱动电流)
		FDC2214SetParameters(DRIVE_CURRENT_CH1,0x78,0x00);
		FDC2214SetParameters(DRIVE_CURRENT_CH2,0x78,0x00);
		FDC2214SetParameters(DRIVE_CURRENT_CH3,0x78,0x00);
		
		FDC2214SetParameters(ERROR_CONFIG,0x00,0x00);//全部禁止错误报告
		//FDC2214SetParameters(MUX_CONFIG,0x52,0x0D);
		//FDC2214SetParameters(CONFIG,0x14,0x01);
		FDC2214SetParameters(MUX_CONFIG,0xC2,0x0D);//通道0,1,2 ,3;选择10Mhz为超过震荡槽震荡频率的最低设置，多通道，四通道
		
		FDC2214SetParameters(CONFIG,0x14,0x01);//激活模式，使用内部振荡器做参考频率，INTB引脚会随状态寄存器更新被置位

	}
	else 
	{
		return 0;
	}
	return 1;
}


unsigned long FCD2214ReadCH(unsigned char index)
{
	unsigned long xdata result;
	switch(index)
	{
		case 0:
		  result = FDC2214Read2Bytes(DATA_CH0)&0x0FFF;
		  result = (result<<16)|(FDC2214Read2Bytes(DATA_LSB_CH0));
			break;
		case 1:
			result = FDC2214Read2Bytes(DATA_CH1)&0x0FFF;
		  result = (result<<16)|(FDC2214Read2Bytes(DATA_LSB_CH1));
			break;
		case 2:
			result = FDC2214Read2Bytes(DATA_CH2)&0x0FFF;
		  result = (result<<16)|(FDC2214Read2Bytes(DATA_LSB_CH2));
			break;
		case 3:
			result = FDC2214Read2Bytes(DATA_CH3)&0x0FFF;
		  result = (result<<16)|(FDC2214Read2Bytes(DATA_LSB_CH3));
			break;
		default:break;
	}
	result =result&0x0FFFFFFF;
	return result;
}

float Cap_Calculate(unsigned char index)//计算电容值
{

	longFDC2214Data = FCD2214ReadCH(index);

//	Cap = 56645.763f/((float)longFDC2214Data);
//	return ((Cap*Cap)-33);
    //Cap = 232021045.248/(longFDC2214Data);
	longFDC2214Data=longFDC2214Data/1000;
	return (longFDC2214Data);//return (longFDC2214Data);
}
