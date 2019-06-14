#include<regx51.h>
#include<intrins.h>
void delay(unsigned int i);

sbit fm = P2^3;
unsigned char a;
unsigned int i, q = 0;
//void bibibi(int i);
//共阴极数码管字模0 - 9 //增加“-”
//unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};
//共阴极数码管位选控制 0 - 7
//unsigned char code dispbit[4]={0x1f,0x2f,0x4f,0x8f};
void main(void)
{
	EA = 1;
	EX0 = 1;
	EX1 = 1;
	IT0 = 1;
	IT1 = 1;
	PX1 = 1;
	while(1)
	{
		P1 = 0xff;
	}
}
void int0()interrupt 0
{
	EX0 = 0;
	a = 0xfe;
	for(i = 0;i < 8;i++)
	{
		P1 = a;
		a = _crol_(a, 1);
		delay(300);
	}
	EX0 = 1;
}
void int1()interrupt 2
{
	EX1 = 0;
	a = 0x7f;
	for(i = 0;i < 8;i++)
	{
		P1 = a;
		a = _cror_(a, 1);
		delay(300);
	}
	EX1 = 1;
}
void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}