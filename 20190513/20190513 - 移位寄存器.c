#include<regx52.h>
#include<intrins.h>
sbit CLK = P2^0;
sbit D = P2^1;
void delay(unsigned int xms);
void main(void)
{
	unsigned char i;
	D = 1;
	for(i = 0;i < 8;i++)
	{
		CLK = 0;
		CLK = 1;
	}
	while(1)
	{
		CLK = 0;
		D = 0;
		CLK = 1;
		delay(300);
		D = 1;
		for(i = 0;i < 7;i++)
		{
			CLK = 0;
			CLK = 1;
			delay(300);
		}
	}
}
void delay(unsigned int xms)
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}