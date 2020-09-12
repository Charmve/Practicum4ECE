/*
 * TCS3200Ä£¿é
 * 
 * ÓÃÍ¾£ºTCS3200ÑÕÉ«²âÊÔ,¶ÁÈ¡RGBÖµ,LCD1602ÏÔÊ¾R,G,BÖµ
 * 
 * ×÷Õß					ÈÕÆÚ				±¸×¢
 * Huafeng Lin			2010/12/10			ĞÂÔö
 * Huafeng Lin			2010/12/11			ĞŞ¸Ä
 * 
 */

//½ÓÏßËµÃ÷£º
//Ä£¿éS2-----µ¥Æ¬»úP1.1
//Ä£¿éS3-----µ¥Æ¬»úP1.0
//Ä£¿éOUT----µ¥Æ¬»úP3.5(¼ÆÊıÆ÷1ÊäÈë)
//Ä£¿éVCC----µ¥Æ¬»úVCC
//Ä£¿éGND----µ¥Æ¬»úGND

#include<REG52.H>	
#include<math.h>       //Keil library  
#include<stdio.h>      //Keil library	
#include<INTRINS.H>

#define uchar unsigned char
#define uint  unsigned int	
#define DataPort P2	   //LCD1602 Êı¾İ¶Ë¿Ú
	
sbit    LCM_RS=P0^2;   //LCD1602 ¿ØÖÆ¶Ë¿Ú		
sbit    LCM_RW=P0^1;   //LCD1602 ¿ØÖÆ¶Ë¿Ú	
sbit    LCM_EN=P0^0;   //LCD1602 ¿ØÖÆ¶Ë¿Ú

/**Òı½Å¶¨Òå**/  
sbit s2=P1^1;        //TCS3200 S2 
sbit s3=P1^0;        //TCS3200 S3
                     //TCS3200 S0 Ä£¿éÄÚ²¿Ä¬ÈÏÉÏÀ­
                     //TCS3200 S1 Ä£¿éÄÚ²¿Ä¬ÈÏÉÏÀ­
                     //TCS3200 OE Ä£¿éÄÚ²¿½ÓµØ
sbit test_pin=P1^2;  //ÓÃÊ¾²¨Æ÷¿´Õâ¸öÒı½Å£¬¿ÉÖªµÀ¶¨Ê±Æ÷ÖĞ¶ÏÆµÂÊ
//±äÁ¿¡¢³£Á¿¶¨Òå
uchar ge,shi,bai ;
uchar rp=3,gp=3,bp=6; //¶¨Òå±ÈÀıÒò×Ó£¬¾ßÌå»·¾³¿ÉÒÔĞŞ¸Ä
uchar count;          //ÑÕÉ«±êÖ¾Î»£¨0:ºì 1:ÂÌ 2:À¶£©

//ÏÔÊ¾Êı×é
uchar disp_R[3];  //ºì
uchar disp_G[3];  //ÂÌ
uchar disp_B[3];  //À¶

//********¶¨Òåº¯Êı*****************************
void    delay(unsigned int k);
void    InitLcd();
void    WriteDataLCM(uchar dataW);
void    WriteCommandLCM(uchar CMD,uchar Attribc);
void    DisplayOneChar(uchar X,uchar Y,uchar DData);

//*********LCD1602³õÊ¼»¯**********************
void InitLcd()				
{			
	WriteCommandLCM(0x38,1);	
	WriteCommandLCM(0x08,1);	
	WriteCommandLCM(0x01,1);
	WriteCommandLCM(0x06,1);	
	WriteCommandLCM(0x0c,1);
}

//**********¼ì²âÃ¦ĞÅºÅ************************
void WaitForEnable(void)	
{					
	DataPort=0xff;		
	LCM_RS=0;LCM_RW=1;_nop_();
	LCM_EN=1;_nop_();_nop_();
	while(DataPort&0x80);	
	LCM_EN=0;				
}
					
//**********Ğ´ÃüÁîÖÁLCD***********************
void WriteCommandLCM(uchar CMD,uchar Attribc)
{					
	if(Attribc)WaitForEnable();	
	LCM_RS=0;LCM_RW=0;_nop_();
	DataPort=CMD;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}	
				
//**********Ğ´Êı¾İÖÁLCD************************
void WriteDataLCM(uchar dataW)
{					
	WaitForEnable();		
	LCM_RS=1;LCM_RW=0;_nop_();
	DataPort=dataW;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}
					
//*********Ğ´Ò»¸ö×Ö·ûÊı¾İµ½Ö¸¶¨µÄÄ¿±ê***********
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM(X,0);		
	WriteDataLCM(DData);		
}

//**********ÑÓÊ±º¯Êı***************
void delay(unsigned int k)	
{						
	unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
		for(j=0;j<121;j++)			
		{;}
	}						
}						            

/*******************************************
* º¯ÊıÃû³Æ: t0_init()
* º¯Êı¹¦ÄÜ: ¶¨Ê±Æ÷0³õÊ¼»¯
* Èë¿Ú²ÎÊı: ÎŞ
* ³ö¿Ú²ÎÊı: ÎŞ
/********************************************/
void t0_init()
{
	TMOD=0x51;        //T1¼ÆÊı T0¶¨Ê± ¹¤×÷·½Ê½1
	
	TH1=0x00;        //¼ÆÊı³õÖµ
	TL1=0x00;
	
	TH0=0xE0;
	TL0=0x00;        //11¡£0592M ¾§Õñ10ms
	EA=1;            //¿ªÖĞ¶Ï
	
	ET0=1;        
	TR0=1;           //Æô¶¯
	TR1=1;
}

//*********************************************
//ÊıÖµ×ª»»³ö¸öÊ®°ÙÇ§µÄASCIIÂë
//*********************************************
void conversion(uint temp_data)  
{  
    bai=temp_data/100+0x30 ;
    temp_data=temp_data%100;   //È¡ÓàÔËËã
    shi=temp_data/10+0x30 ;
    ge=temp_data%10+0x30;      //È¡ÓàÔËËã
}

/*******************************************
* º¯ÊıÃû³Æ: main()
/********************************************/
void main()
{
	delay(10); 
	InitLcd();      //lcd³õÊ¼»¯
	s2=0;           //³õÊ¼Éè¶¨S2Òı½Å
	s3=0;           //³õÊ¼Éè¶¨S3Òı½Å
	t0_init();      //¶¨Ê±¼ÆÊı³õÊ¹¯

	while(1)
	{
		DisplayOneChar(0, 0, 'T');
		DisplayOneChar(1, 0, 'C');
		DisplayOneChar(2, 0, 'S');
		DisplayOneChar(3, 0, '2');
		DisplayOneChar(4, 0, '3');
		DisplayOneChar(5, 0, '0');

		DisplayOneChar(10, 0, 'R'); 
		DisplayOneChar(11, 0, '['); 
		DisplayOneChar(12, 0, disp_R[0]); 
		DisplayOneChar(13, 0, disp_R[1]); 
		DisplayOneChar(14, 0, disp_R[2]);
		DisplayOneChar(15, 0, ']');	
	
		DisplayOneChar(0, 1, 'G'); 
		DisplayOneChar(1, 1, '['); 
		DisplayOneChar(2, 1, disp_G[0]); 
		DisplayOneChar(3, 1, disp_G[1]); 
		DisplayOneChar(4, 1, disp_G[2]);
		DisplayOneChar(5, 1, ']');
		
		DisplayOneChar(10, 1, 'B'); 
		DisplayOneChar(11, 1, '['); 
		DisplayOneChar(12, 1, disp_B[0]); 
		DisplayOneChar(13, 1, disp_B[1]); 
		DisplayOneChar(14, 1, disp_B[2]);
		DisplayOneChar(15, 1, ']');				
		
		delay(100) ;	
	}
}

/*******************************************
* º¯ÊıÃû³Æ: c10ms_out() 
* º¯Êı¹¦ÄÜ: ¶¨Ê±ÖĞ¶Ï0·şÎñ³ÌĞò
            ĞŞ¸ÄÑÕÉ«±êÖ¾disp_tc£¨0:ºì 1:ÂÌ 2:À¶£©
            ÉèÖÃS0 S1 S2 Ñ¡ÔñÂË²¨Æ÷
            ¼ÆËãÂö³å£¬¶ÁÈ¡É«Öµ
* Èë¿Ú²ÎÊı: ÎŞ
* ³ö¿Ú²ÎÊı: ÎŞ
/********************************************/
void c10ms_out() interrupt 1
{
	uint temp;
	test_pin=!test_pin; //²âÊÔ¶¨Ê±Æ÷ÖĞ¶ÏÆµÂÊÒı½Å£¬¿ÉÒÔÓÃÊ¾²¨Æ÷¹Û²ì
	TR0=0;              //¹Ø±Õ¶¨Ê±
	TR1=0;              //¹Ø±Õ¼ÆÊı
	//   count+1ÊµÏÖÏÈ¼ì²âÂÌÉ«,ÔÙ¼ì²âÀ¶É«,È»ºó¼ì²âºìÉ«,Ñ­»·¼ì²â       
	if(count==0)
	{
		count++;    
		s2=1;s3=1;             //Ñ¡ÔñÂË²¨Æ÷ÎªÂÌÉ«     
		
		temp=(8<<TH1)+TL1;    //¼ÆËãÕâ¶ÎÊ±¼äÄÚ TCS230 µÄÊä³öÂö³åÊı	
		temp/=rp;			
		conversion(temp);
		disp_R[2]=ge;         //ÒòÎªÕâ´ÎµÄÖĞ¶Ï£¬ÊÇÉÏ´ÎÑ¡ÔñÂË²¨Æ÷µÄÊıÖµ
		disp_R[1]=shi;
		disp_R[0]=bai;
	}	
	else if(count==1)
	{            
		count++;
		s2=1;s3=0;            //Ñ¡ÔñÂË²¨Æ÷ÎªÀ¶É«
		temp=(8<<TH1)+TL1;    //¼ÆËãÕâ¶ÎÊ±¼äÄÚ TCS230 µÄÊä³öÂö³åÊı	
		temp/=gp;			
		conversion(temp);
		disp_G[2]=ge;         //ÒòÎªÕâ´ÎµÄÖĞ¶Ï£¬ÊÇÉÏ´ÎÑ¡ÔñÂË²¨Æ÷µÄÊıÖµ
		disp_G[1]=shi;
		disp_G[0]=bai;
	}	
	else if(count==2)
	{            
		count=0;
		s2=0;s3=0;            //Ñ¡ÔñÂË²¨Æ÷ÎªºìÉ«
		
		temp=(8<<TH1)+TL1;    //¼ÆËãÕâ¶ÎÊ±¼äÄÚ TCS230 µÄÊä³öÂö³åÊı	
		temp/=bp;		
		conversion(temp);
		disp_B[2]=ge;         //ÒòÎªÕâ´ÎµÄÖĞ¶Ï£¬ÊÇÉÏ´ÎÑ¡ÔñÂË²¨Æ÷µÄÊıÖµ
		disp_B[1]=shi;
		disp_B[0]=bai;	
	}
	
	//¶¨Ê±Æ÷¼ÆÊıÆ÷ÖØ¸³³õÖµ
	TH0=0xE0;
	TL0=0x00; //11¡£0592M ¾§Õñ£¬Îª10ms
	TL1=0x00;//¼ÆÊıÆ÷ÇåÁã
	TH1=0x00;//¼ÆÊıÆ÷ÇåÁã
	TR0=1;   //´ò¿ª¶¨Ê±Æ÷
	TR1=1;   //´ò¿ª¼ÆÊıÆ÷
}
