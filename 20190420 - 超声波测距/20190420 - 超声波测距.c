/***Ultrasonic Ranging Module designed by Ametoki 2019/04/20***/
#include<regx52.h>
#include<intrins.h>
#include<stdio.h>
#define RS_CLR RS=0
#define RS_SET RS=1
#define RW_CLR RW=0
#define RW_SET RW=1
#define EN_CLR EN=0
#define EN_SET EN=1
#define DataPort P0
sbit RS = P1^0;
sbit RW = P1^1;
sbit EN = P2^5;
sbit US_VCC = P1^4;
sbit Trig = P1^5;
sbit Echo = P1^6;
sbit US_GND = P1^7;

unsigned char COM = 100;
unsigned int Distance;
unsigned char Char_Num[]="0123456789cm.";
bit LCD_Check_Busy(void);
void Delay(unsigned int xms);
void LCD_Clear(void);
void Init(void);
void LCD_Write_Com(unsigned char com);
void LCD_Write_Data(unsigned char Data);
void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s);
void LCD_Write_Char(unsigned char x, unsigned char y, unsigned char Data);
void Check_Distance(void);
void main(void)
{
	unsigned int a, b;
	while(LCD_Check_Busy());
	Init();
	while(1)
	{
		Check_Distance();
		LCD_Write_Char(4, 0, Char_Num[Distance % 10000 / 1000]);
		LCD_Write_Char(5, 0, Char_Num[Distance % 1000 / 100]);
		LCD_Write_Char(6, 0, Char_Num[Distance % 100 / 10]);
		LCD_Write_Char(7, 0, Char_Num[12]);
		LCD_Write_Char(8, 0, Char_Num[Distance % 10]);
		LCD_Write_Char(10, 0, Char_Num[10]);
		LCD_Write_Char(11, 0, Char_Num[11]);
		Delay(200);
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
void Delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}
void LCD_Clear(void)
{
	LCD_Write_Com(0x01);
	Delay(COM);
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
	TMOD=0x01;
	TH0=0;
	TL0=0;
	EA=1;
	ET0=1;
	US_VCC = 0;
	US_GND = 0;
	US_VCC = 1;
	Trig = 0;
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
void Check_Distance(void)//计算函数
{
	unsigned int T, Sum = 0, y, x;
	for(y = 0;y < 5;y++)
	{
		Trig = 1;
		for(x = 0;x < 10;x++)
			_nop_();
		Trig = 0;
		while(!Echo);//当ECHO脚为1，开始记时
		TR0 = 1;
		while(Echo);//当ECHO脚为0，关闭计时
		TR0 = 0;
		//LCD_Clear();
		T = TH0 * 256 + TL0;
		TH0 = 0;
		TL0 = 0;
		Sum += T * 0.18772;// 100;	//单位毫米
		Delay(y + 7);
	}
	Distance = Sum / 5;
}
void Out(void) interrupt 1
{
	TR0 = 0;
	TH0 = 0;
	TL0 = 0;
	LCD_Clear();
	LCD_Write_String(3, 1, "Over Range!");
	Delay(300);
	LCD_Clear();
	Delay(300);
} 
