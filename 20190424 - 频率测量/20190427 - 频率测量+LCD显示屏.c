//频率测量 + 1602A显示屏
#include<regx52.h>
#include<intrins.h>
#include<stdio.h>
#define RS_CLR RS=0
#define RS_SET RS=1
#define RW_CLR RW=0
#define RW_SET RW=1
#define EN_CLR EN=0
#define EN_SET EN=1
#define DataPort P2
sbit RS = P1^5;
sbit RW = P1^6;
sbit EN = P1^7;
//sbit Pulse = P3^5;
unsigned long int Fre;
unsigned int t = 0;
unsigned char Offset = 0x3D;//当计数器即将溢出时更新计数器的时间补偿
unsigned char COM = 5;//显示屏指令延迟时间/ms
unsigned char Char_Num[]="0123456789 Hz.";
bit LCD_Check_Busy(void);
/****
void Beep(void);
void Alarm(void);
****/
void Delay(unsigned int xms);
void LCD_Clear(void);
void Init(void);
void LCD_Write_Com(unsigned char com);
void LCD_Write_Data(unsigned char Data);
void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s);
void LCD_Write_Char(unsigned char x, unsigned char y, unsigned char Data);
void Display(unsigned long int temp);
void main(void)
{
	while(LCD_Check_Busy());
	Init();
	LCD_Write_String(0, 0, "Hello");
	while(1)
	{
		if(TH1 >= 0xE0)//计数器即将溢出时更新计数器
		{
			Fre += TH1 * 256 + TL1;
			if(TL0 >= Offset)
			TL0 -= Offset;
			TH1 = 0x00;
			TL1 = 0x00;
		}
		if(t == 0)
			Display(Fre);//每秒只刷新一次频率显示
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
	TMOD = 0x52;
	SCON = 0x00;
	TH0 = 0x80;
	TL0 = 0x80;
	TH1 = 0;
	TL1 = 0;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
	TR1 = 1;
}
void timer0(void) interrupt 1
{
	t++;
	if(t == 7201)
	{
		t = 0;
		Fre += TH1 * 256 + TL1;
		TH1 = 0;
		TL1 = 0;
	}
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
void Display(unsigned long int temp)
{
	unsigned char i;
	temp = Fre;
	for(i = 9;i >= 3;i--)
	{
		LCD_Write_Char(i, 0, Char_Num[temp % 10]);
		temp /= 10;
	}
	LCD_Write_Char(10, 0, Char_Num[10]);
	LCD_Write_Char(11, 0, Char_Num[11]);
	LCD_Write_Char(12, 0, Char_Num[12]);
	Delay(200);
	Fre = 0;
}
void Delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}