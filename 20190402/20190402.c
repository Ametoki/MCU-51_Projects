#include<regx52.h>
#include<intrins.h>
void delay(unsigned int i);
void Beep(void);

sbit fm = P2^3;
unsigned int i, q = 0;
//void bibibi(int i);
//共阴极数码管字模0 - 9 //增加“-”
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};
//共阴极数码管位选控制 0 - 7
//unsigned char code dispbit[4]={0x1f,0x2f,0x4f,0x8f};
void main(void)
{
	char r = 0, s = 0;
  unsigned int LedOut[4];
	EA = 1;
	EX0 = 1;
	IT0 = 1;
	while(1)
	{
			P2 = Disp_fTab[q];
	}
}

void Plus()interrupt 0
{
	q++;
	Beep();
}
void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}
void Beep(void)
{
	fm = 0;
	delay(50);
	fm = 1;
}