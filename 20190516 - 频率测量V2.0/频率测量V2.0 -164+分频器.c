//比较精准的频率测量程序
#include<regx52.h>
sbit Pulse = P3^5;
sbit SEG_EN = P1^7;
bit Range_Mode;
unsigned char code Disp_Tab_2[] = {0x77,0x14,0xb3,0xb6,0xd4,0xe6,0xe7,0x34,0xf7,0xf6,0x00}, Division[8] = {1, 2, 4, 8, 16, 32, 64, 128};
unsigned long int Fre[8] = {0}, temp;
double double_temp;
unsigned int Multi = 1800, t;
unsigned char Offset = 0x02, sum_flag, dis_flag, first_flag, Dvs = 7;//时间补偿
void Timer_Init_H(void);
void Timer_Init_L(void);
void Display(void);
void Delay(unsigned int i);
void main(void)
{
	unsigned char i;
	SCON = 0x00;//串口方式0输出
	Timer_Init_H();
	Display();
	while(1)
	{
		P1 = Dvs;
		if(!Range_Mode)
		{
			if(!t)
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
							if(Fre[sum_flag] > 6E4)
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
				while(Pulse);
				TR0 = 0;
				double_temp = 2304E8 / ((double)t * 256 + (double)TL0 - 1);
				Fre[i] = (unsigned long int)double_temp;
				if(Fre[i] >= 12E6)
				{
					Dvs++;
					Timer_Init_H();
				}
				if(!first_flag)
					temp = Fre[i] * 8;
				TH0 = 0;
				TL0 = 0;
				t = 0;
				Display();
				if(Fre[i] >= 2E6)
					Delay(230);
			}
			first_flag = 1;
		}
	}
}
void Timer_Init_H(void)
{
	unsigned char i;
	TMOD = 0x52;
	TH0 = 0x80;
	TL0 = 0x80;
	TH1 = 0;
	TL1 = 0;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
	TR1 = 1;
	t = 0;
	sum_flag = 0;
	first_flag = 0;
	dis_flag = 0;
	Range_Mode = 0;
	for(i = 0;i < 8;i++)
		Fre[i] = 0;
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
	Dvs = 0;
	for(i = 0;i < 8;i++)
		Fre[i] = 0;
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
	SEG_EN = 1;
	for(i = 0;i < 8;i++)
	{
		if(i == 6 && Range_Mode)
			SBUF = Disp_Tab_2[temp % 10] | 0x08;
		else
			SBUF = Disp_Tab_2[temp % 10];
		while(!TI);
		TI = 0;
		temp /= 10;
	}
	SEG_EN = 0;
	temp = 0;
	dis_flag = 0;
}
void Delay(unsigned int i)
{
    unsigned char j;
    for(; i > 0; i--)
        for(j = 112; j > 0; j--);
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