#include<regx52.h>
#include<intrins.h>
void delay(unsigned int xms);
void assign(void);
unsigned char LedOut[4];
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};
//unsigned char code dispbit[4]={0xfe,0xfd,0xfb,0xf7};
unsigned int a = 0, b_Temp;
unsigned char Num;
sbit AB = P2^2;
sbit CLK = P2^3;
sbit SEG = P2^4;
void main(void)
{
	int i, c;
	EX0 = 1;
	EX1 = 1;
	IT0 = 1;
	IT1 = 1;
	TMOD = 0x06;
	TH0 = 0xFF;
	TL0 = 0xFF;
	ET0 = 1;
	TR0 = 1;
	EA = 1;
	assign();
	while(1)
	{
		for(i = 0;i < 4;i++)
		{
			SEG = 0;
			Num = LedOut[i];
			for(c = 0;c < 8;c++)
			{
				CLK = 0;
				AB = Num & 0x01;
				CLK = 1;
				Num >>= 1;
			}
			P2 = i;
			delay(3);
			SEG = 1;//数码管初始化，***消除残影***
		}
	}
}
void assign(void)//数码管段码赋值函数
{
	int j;
	b_Temp = a;
	for(j = 3;j >= 0;j--)
	{
		LedOut[j] = Disp_Tab[b_Temp % 10];
		b_Temp /= 10;
	}
}
void plus() interrupt 0
{
	EX0 = 0;
		if(a >= 0 && a < 9999)
		{
			a++;
			assign();
		}
	EX0 = 1;
}
void plus2() interrupt 1
{
	ET0 = 0;
		if(a >= 0 && a < 9999)
		{
			a++;
			assign();
		}
	ET0 = 1;
}
void minus() interrupt 2
{
	EX1 = 0;
	if(a > 0 && a <= 9999)
	{
		a--;
		assign();
	}
	EX1 = 1;
}
void delay(unsigned int xms)
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}