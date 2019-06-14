#include<regx51.h>
void delay(unsigned int xms);
unsigned char code Num[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};//π≤“ı

void main(void)
{
	int i=0;
	while(1)
	{
		for(i=0;i<=60;i++)
		{
			if(i==60)
			{
				i=0;
			}
			P2=Num[i%10];
			P0=Num[i/10];
			delay(1000);
		}
	}
}

void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}