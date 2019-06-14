#include<regx52.h>
#include<intrins.h>

sbit RS = P1^0;
sbit RW = P1^1;
sbit EN = P2^5;
sbit fm = P2^3;
#define RS_CLR RS=0
#define RS_SET RS=1
#define RW_CLR RW=0
#define RW_SET RW=1
#define EN_CLR EN=0
#define EN_SET EN=1
#define DataPort P0

unsigned char COM = 50, Sec, Min, Hour, time;
void Beep(void);
void Alarm(void);
bit LCD_Check_Busy(void);
void Delay(unsigned int xms);
void LCD_Clear(void);
void Init(void);
void LCD_Write_Com(unsigned char com);
void LCD_Write_Data(unsigned char Data);
//void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s);
void LCD_Write_Char(unsigned char x, unsigned char y, unsigned char Data);

void main(void)
{
	while(LCD_Check_Busy());
	Init();
	while(1)
	{
		if(time % 5 == 0)
		{
			LCD_Write_Char(4, 0, 0x30 + (Hour / 10));
			LCD_Write_Char(5, 0, 0x30 + (Hour % 10));
			LCD_Write_Char(6, 0, ':');
			LCD_Write_Char(7, 0, 0x30 + (Min / 10));
			LCD_Write_Char(8, 0, 0x30 + (Min % 10));
			LCD_Write_Char(9, 0, ':');
			LCD_Write_Char(10, 0, 0x30 + (Sec / 10));
			LCD_Write_Char(11, 0, 0x30 + (Sec % 10));
		}
		switch(P3)
		{
			case 0xEF:
				while(P3 == 0xEF);
				if (Hour < 23){Hour++;Beep();break;}
				else{Hour = 0;Beep();break;}
			case 0xDF:
				while(P3 == 0xDF);
				if (Min < 59){Min++;Beep();break;}
				else{Min = 0;Beep();break;}
			case 0xBF:
				while(P3 == 0xBF);
				if (Sec < 59){Sec++;Beep();break;}
				else{Sec = 0;Beep();break;}
			//case 0x7F: 
			//	while(P3 == 0x7F);
				//if(hour == 0){hour = 1;Beep();break;}
				//else{hour = 0;Beep();break;}
		}
	}
}
void Clock(void) interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	time++;
	if(time == 20)
	{
		time = 0;
		Sec++;
		if(Sec == 60)
		{
			Sec = 0;
			Min++;
			if(Min == 60)
			{
				Min = 0;
				Hour++;
				if(Hour == 24)
					Hour = 0;
			}
		}
	}
}
void Beep(void)
{
	fm = 1;
	Delay(50);
	fm = 1;
}
void Alarm(void)
{
	unsigned int i, j;
	for(j = 0;j < 48;j++)
	{
			for(i = 0;i < 6; i++)
		{
			if(i < 4)
			{
				fm = 0;
				Delay(80);
				fm = 1;
				Delay(80);
			}
			else
			{
				Delay(80);
			}
		}
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
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;
	TR0=1;
	time = 0;
	Sec = 0;
	Min = 0;
	Hour = 0;
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