#include<regx52.h>
#include<intrins.h>
void delay(unsigned int xms);
void assign(void);
void timer0_init(void);
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};
//unsigned char code dispbit[4]={0xfe,0xfd,0xfb,0xf7};
unsigned int a = 0, b = 0, t, s;
unsigned char Num;
sbit CLK = P2^0;
sbit AB = P2^1;
sbit LED = P3^7;
void main(void)
{
	int i, c;
	EX0 = 1;
	EX1 = 1;
	IT0 = 1;
	IT1 = 1;
	EA = 1;
	LED = 1;
	while(1)
	{
		Num = Disp_Tab[a];
		for(i = 0;i < 2;i++)
		{
			for(c = 0;c < 8;c++)
			{
				CLK = 0;
				AB = Num & 0x80;
				CLK = 1;
				Num = _cror_(Num, 1);
			}
			Num = Disp_Tab[b];
		}
		delay(500);
	}
}
void plus() interrupt 0
{
	EX0 = 0;
	LED = 0;
	timer0_init();
	t = 0;
	s = 1000;
	a++;
	if(a == 10)
	{
		a = 0;
		if(b < 9)
			b++;
	}
	EX0 = 1;
}
void minus() interrupt 2
{
	EX1 = 0;
	LED = 0;
	timer0_init();
	t = 1000;
	s = 0;
	if(a > 0 || b > 0)
	{
		if(a == 0 && b > 0)
		{
			a = 9;
			b--;
		}
		else if(a > 0)
			a--;
	}
	EX1 = 1;
}
void delay(unsigned int xms)
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}
void timer0_init(void)
{
	TMOD = 0x01;
	TH0 = 0x3c;
	TL0 = 0xb0;
	ET0 = 1;
	TR0 = 1;
}
void timer0()interrupt 1
{
	
	TH0 = 0x3c;
	TL0 = 0xb0;
	t++;
	s++;
	if(t == 20 || s == 0)
	{
		TR0 = 0;
		LED = 1;
		t = 0;
		s = 0;
	}
}