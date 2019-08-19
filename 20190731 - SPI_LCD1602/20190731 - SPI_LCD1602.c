//频率测量 + 1602A显示屏
#include<regx52.h>
#include<intrins.h>
#define RS_CLR RS=0
#define RS_SET RS=1
#define RW_CLR RW=0
#define RW_SET RW=1
#define EN_CLR EN=0
#define EN_SET EN=1
#define DataPort P0
#define delayNOP(); {_nop_();};
sbit RS = P2^6;//P3^5;
sbit RW = P2^5;//P2^5;
sbit EN = P2^7;//P3^4;
sbit key1 = P3^1;
sbit key2 = P3^0;
sbit SCK = P1^0;//p1.0口模拟时钟输出
sbit MOSI = P1^1;//p1.1口模拟主机输出
sbit MISO = P1^2;//p1.2口模拟主机输入
sbit SS1 = P1^3;//p1.3口模拟片选
sbit FUCK = P1^7;
//unsigned char code Char_Num[]="0123456789";
unsigned int COM = 2;
unsigned char ch = 0x30;
bit LCD_Check_Busy(void);
void Init(void);
void LCD_Clear(void);
void LCD_Write_Com(unsigned char com);
void LCD_Write_Data(unsigned char Data);
//void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s);
void LCD_Write_Char(unsigned char x, unsigned char y, unsigned char Data);
void LCD_Tag(void);
//void SPI_Send_Byte(unsigned char Data);
//unsigned char SPI_Recv_Byte(void);
unsigned char SPI_Send_Recv_Byte(unsigned char Data);
void Delay(unsigned int xms);
void main(void)
{
	unsigned char recv_temp, i = 5;
	while(LCD_Check_Busy());
	Init();
	P3 = 0x0f;
	while(1)
	{
		if(!key1)
		{
			Delay(20);
			if(!key1)
			{
				while(!key1);
				if(ch < 256)
					ch += 1;
			}
		}
		else if(!key2)
		{
			Delay(20);
			if(!key2)
			{
				while(!key2);
				if(ch > 0)
					ch -= 1;
			}
		}
		recv_temp = SPI_Send_Recv_Byte(ch);
		recv_temp = SPI_Send_Recv_Byte(ch);
		//Delay(100);
		ch = recv_temp;
		LCD_Write_Char(0, 0, ch);
		LCD_Write_Char(0, 1, recv_temp);
	}
}
void Init(void)
{
	LCD_Write_Com(0x38);
	Delay(COM);
	LCD_Write_Com(0x08);
	Delay(COM);
	LCD_Write_Com(0x06);
	Delay(COM);
	LCD_Clear();
	LCD_Write_Com(0x0C);
	Delay(COM);
	SS1 = 1;
	SCK = 0;
	FUCK = 0;
}
bit LCD_Check_Busy(void)
{
	RS_CLR;
	RW_SET;
	EN_CLR;
	_nop_();
	EN_SET;
	return (bit)(DataPort & 0x80);
}
void LCD_Clear(void)
{
	LCD_Write_Com(0x01);
	Delay(COM);
}
void LCD_Write_Com(unsigned char com)
{
	while(LCD_Check_Busy());
	RS_CLR;
	RW_CLR;
	DataPort = com;
	EN_SET;
	_nop_();
	EN_CLR;
}
void LCD_Write_Data(unsigned char Data)
{
	while(LCD_Check_Busy());
	RS_SET;
	RW_CLR;
	DataPort = Data;
	EN_SET;
	_nop_();
	EN_CLR;	
}
/*
void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s)
{
	if(y == 0)
	{
		LCD_Write_Com(0x80 + x);
	}
	else
	{
		LCD_Write_Com(0xC0 + x);
	}
	while(*s)
	{
		LCD_Write_Data(*s);
		s++;
	}
}
*/
void LCD_Write_Char(unsigned char x, unsigned char y, unsigned char Data)
{
	if(y == 0)
	{
		LCD_Write_Com(0x80 + x);
	}
	else
	{
		LCD_Write_Com(0xC0 + x);
	}
	LCD_Write_Data(Data);
}
/*
void LCD_Tag(void)
{
	;
}
*/
/*
//--------------------------------------------------------------------------------------------------
// 函数名称： SPISendByte
// 入口参数： Data
// 函数功能： 发送一个字节
//--------------------------------------------------------------------------------------------------
void SPI_Send_Byte(unsigned char Data)
{
	unsigned char n;//向SDA上发送一位数据字节，共八位  
	SCK = 1;//时钟置为高电平
	SS1 = 0;//选择从机
	for(n = 0;n < 8;n++)
	{
		delayNOP();
		SCK = 0;
		if((Data & 0x80) == 0x80)//若发送数据的最高位为1，则发送1
			MOSI = 1;
		else
			MOSI = 0;//最高位数据为0，发送0
		delayNOP();
		Data = Data << 1;//发送数据左移一位
		SCK = 1;//时钟置为高电平
	}
}
//--------------------------------------------------------------------------------------------------
// 函数名称： SPIreceiveByte
// 返回接收的数据
// 函数功能： 接收一字节子程序
//--------------------------------------------------------------------------------------------------
unsigned char SPI_Recv_Byte(void)
{
	unsigned char n;
	unsigned char data_recv;
	SCK = 1;
	SS1 = 0;
	for(n = 0;n < 8;n++)
	{
		delayNOP();
		SCK = 0;
		delayNOP();
		data_recv = data_recv << 1;
		if(MISO == 1)
			data_recv = data_recv | 0x01;
		else
			data_recv = data_recv & 0xfe;
		SCK = 1;
	}
	return(data_recv);
}
*/
unsigned char SPI_Send_Recv_Byte(unsigned char Data)
{
	unsigned char n;
	unsigned char data_recv = 0;
	SS1 = 0;
	for(n = 0;n < 9;n++)
	{
		SCK = 0;//时钟置为低电平
		{
		if((Data & 0x80) == 0x80)//若发送数据的最高位为1，则发送1
			MOSI = 1;
		else
			MOSI = 0;//最高位数据为0，发送0
			Data = _crol_(Data, 1);//发送数据左移一位
		}
		//Delay(1);
		delayNOP();
		SCK = 1;
		if(n > 0)
		{
			FUCK = 1;
			data_recv = _crol_(data_recv, 1);
			if(MISO)
				data_recv = data_recv | 0x01;
			else
				data_recv = data_recv & 0xfe;
			FUCK = 0;
		}
		//Delay(1);
		delayNOP();
	}
	SS1 = 1;
	return(data_recv);
}
void Delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i = xms;i > 0;i--)
		for(j = 112;j > 0;j--);
}