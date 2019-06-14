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
sbit RS = P1^0;
sbit RW = P1^1;
sbit EN = P2^5;
sbit ADD_A = P3^2;
sbit ADD_B = P3^3;
sbit ADD_C = P3^4;
sbit Pulse = P3^5;
bit Range_Mode, tag_flag = 0, dis_flag, first_flag;
unsigned char code Char_Num[]="0123456789", Division[8] = {1, 2, 4, 8, 16, 32, 64, 128};
unsigned long int Fre[8] = {0}, temp, duty;
double fre_temp, duty_temp;
unsigned int Multi = 1800, t = 0;
unsigned char sum_flag, Dvs = 7, COM = 100;
bit LCD_Check_Busy(void);
void Timer_Init_H(void);
void Timer_Init_L(void);
void Init(void);
void LCD_Clear(void);
void LCD_Write_Com(unsigned char com);
void LCD_Write_Data(unsigned char Data);
void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s);
void LCD_Write_Char(unsigned char x, unsigned char y, unsigned char Data);
void LCD_Tag(void);
void Display(void);
void Delay(unsigned int xms);
void main(void)
{
	unsigned char i;
	Init();
	Timer_Init_H();
	Display();
	while(1)
	{
		if(t == 1200)
			LCD_Tag();
		if(!Range_Mode)
		{
			if(t == 5)
			{
				if(dis_flag)
				{
					if(!first_flag)
						temp = Fre[sum_flag] * 8 * Division[Dvs];
					Display();
				}
				if(Dvs)
				{
					if(Fre[sum_flag] > 3E4)
						{
							if(Fre[sum_flag] > 6E4 && Dvs < 7)
							{
								Dvs++;
								Timer_Init_H();
							}
							else
							{
								dis_flag = 1;
								sum_flag++;
							}
						}
					else
					{
						if(Dvs > 0)
							Dvs--;	
						Timer_Init_H();
					}
				}
				else
				{
					if(Fre[sum_flag] >= 12)
					{
						if(Fre[sum_flag] > 3E4)
						{
							if(Dvs < 7)
								Dvs++;
							Timer_Init_H();
						}
						else
						{
							dis_flag = 1;
							sum_flag++;
						}
					}
					else
						Timer_Init_L();
				}
				if(sum_flag == 8)
				{
					sum_flag = 0;
					first_flag = 1;
				}
			}
		}
		else if(Range_Mode)
		{
			for(i = 0;i < 8;i++)
			{
				while(!Pulse);
				while(Pulse);
				TR0 = 1;
				while(!Pulse);
				duty_temp = (double)t * 256 + (double)TL0 - 4;
				while(Pulse);
				TR0 = 0;
				fre_temp = (double)t * 256 + (double)TL0 - 1;
				duty_temp /= fre_temp;
				duty = (unsigned long int)(1E5 - duty_temp * 1E4 * 0.9936);
				fre_temp = 2304E7 / fre_temp;
				Fre[i] = (unsigned long int)fre_temp;
				if(Fre[i] >= 12E5)
				{
					Timer_Init_H();
					break;
				}
				if(!first_flag)
					temp = Fre[i] * 8;
				TH0 = 0;
				TL0 = 0;
				t = 0;
				Display();
				if(Fre[i] >= 2E5)
					Delay(100);
			}
			first_flag = 1;
		}
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
	LCD_Write_String(0, 0, "FREQ:         Hz");
}
void Timer_Init_H(void)
{
	unsigned char i;
	ADD_A = (bit)(Dvs & 0x01);
	ADD_B = (bit)(Dvs & 0x02);
	ADD_C = (bit)(Dvs & 0x04);
	TMOD = 0x52;
	TH0 = 0x80;
	TL0 = 0x80;
	TH1 = 0;
	TL1 = 0;
	t = 0;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
	TR1 = 1;
	sum_flag = 0;
	first_flag = 0;
	dis_flag = 0;
	Range_Mode = 0;
	for(i = 0;i < 8;i++)
		Fre[i] = 0;
	LCD_Write_String(0, 1, "           ");
}
void Timer_Init_L(void)
{
	unsigned char i;
	TMOD = 0x02;
	TR0 = 0;
	TR1 = 0;
	TH0 = 0;
	TL0 = 0;
	ET0 = 1;
	EA = 1;
	t = 0;
	sum_flag = 0;
	first_flag = 0;
	dis_flag = 0;
	Range_Mode = 1;
	for(i = 0;i < 8;i++)
		Fre[i] = 0;
	LCD_Write_String(0, 1, "DUTY:");
	LCD_Write_String(10, 1, "%");
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
	while(!TI);
	TI = 0;
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
	while(!TI);
	TI = 0;
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
void Display(void)
{
	unsigned char i;
	if(first_flag)
	{
		for(i = 0;i < 8;i++)
			temp += Fre[i];
		temp *= Division[Dvs];
	}
	for(i = 12;i >= 5;i--)
	{
		if(Range_Mode)
		{
			if(i == 7)
			{
				LCD_Write_String(i, 0, ".");
				LCD_Write_String(i, 1, ".");
				continue;
			}
			else if(i == 5)
			{
				if(temp)
					LCD_Write_Char(i, 0, Char_Num[temp % 10]);
				else if(!temp)
					LCD_Write_String(i, 0, " ");
				if(duty)
				{
					LCD_Write_Char(i, 1, Char_Num[duty % 10]);
					duty /= 10;
				}
				else if(!duty)
					LCD_Write_String(i, 0, " ");
			}
			else
			{
				LCD_Write_Char(i, 0, Char_Num[temp % 10]);
				if(i < 10)
				{
					LCD_Write_Char(i, 1, Char_Num[duty % 10]);
					duty /= 10;
				}
			}
		}
		else
		{
			if(temp)
				LCD_Write_Char(i, 0, Char_Num[temp % 10]);
			else
				LCD_Write_Char(i, 0, Char_Num[10]);
		}
		temp /= 10;
	}
	temp = 0;
	duty = 0;
	dis_flag = 0;
}
void LCD_Tag(void)
{
	if(tag_flag)
	{
		LCD_Write_String(12, 1, "CH:");
		tag_flag = 0;
	}
	else
	{
		LCD_Write_String(12, 1, "   ");
		tag_flag = 1;
	}
	LCD_Write_Char(15, 1, Char_Num[Dvs + 1]);
}
void Delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i = xms;i > 0;i--)
		for(j = 112;j > 0;j--);
}
void timer0(void) interrupt 1
{
	t++;
	if(!Range_Mode)
	{
		if(t == Multi)
		{
			TR0 = 0;
			TR1 = 0;
			t = 0;
			Fre[sum_flag] = TH1 * 256 + TL1;
			TL0++;
			TH1 = 0;
			TL1 = 0;
			TR1 = 1;
			TR0 = 1;
		}
	}
}