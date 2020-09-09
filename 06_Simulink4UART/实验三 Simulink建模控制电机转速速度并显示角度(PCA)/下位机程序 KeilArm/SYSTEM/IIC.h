#ifndef		__IIC_H
#define		__IIC_H






//void Delay(unsigned char DelayCount);
void IICStart( );
void IICStop( );
bit  IICReadAck( );
void IICNoAck( );
void IICAck( );
void IICSendByte(unsigned char sendbyte);
unsigned char IICReadByte( );
//bit IICWriteReadData(unsigned char ControlByte,unsigned char Address,unsigned char *DataBuff, unsigned char ByteQuantity );
//bit IICWriteReadManyData(unsigned char ControlByte,unsigned char Address,unsigned char *DataBuff, unsigned char ByteQuantity );//读写大于1个缓冲区的(16字节)的数据



#endif