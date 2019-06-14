#include<regx52.h>
void delay(unsigned int xws);
unsigned char temp;
unsigned int hz, t, i, p, q, a = 0;//10000 / hz = t;
unsigned int LedOut[4];
//共阴极数码管字模0 - 9 //增加“-”
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};
//共阴极数码管位选控制 0 - 7
unsigned char code dispbit[4]={0x1f,0x2f,0x4f,0x8f};
void main(void)
{
	hz = 50;
	while(1)
	{
		p = 100 - (a * 100) / t;
		if(p > 99)
			p = 99;
		LedOut[0] = Disp_Tab[hz / 10];
		LedOut[1] = Disp_Tab[hz % 10]|0x80;
		LedOut[2] = Disp_Tab[p / 10];
		LedOut[3] = Disp_Tab[p % 10];
		for(i = 0;i <= 3;i++)
		{
			P0 = LedOut[i];
			P2 = dispbit[i];
			delay(t - a);
			P0 = 0x00;
			delay(a);
		}
		P3 = 0xff;
		t = 10000 / (4 * hz);
		if(P3 != 0xff)
		{
			delay(700);
			if(P3 != 0xff)
			{
				temp = P3;
				switch (temp)
				{
					case 0xef:
						if(a < t)
							a++;
						break;
					case 0xdf:
						if(a > 0)
							a--;
						break;
					case 0xbf:
						if(hz > 1)
							hz--;
						break;
					case 0x7f:
						if(hz < 99)
							hz++;
						break;
					default:
						break;
				}
			}
		}
	}
}
void delay(unsigned int xws)		
{
	unsigned int i,j;
	for(i=xws;i>0;i--)
		for(j=11;j>0;j--);
}