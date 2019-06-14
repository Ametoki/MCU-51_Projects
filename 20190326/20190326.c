#include<regx51.h>
	/*
	unsigned char data i,*dcpt;
	unsigned char xdata *xcpt;
	dcpt = 0x30;
	xcpt = 0x1000;
	for(i = 0;i < 16;i++)
	{
		*(dcpt + i) = *(xcpt + i);
	}
	*/
void delay(unsigned int time)
{	
	unsigned int j=0;
		for(;time>0;time--)
			for(j=0;j<125;j++);
}

void main(void)
{
	char led_mod[]={0x003f,0x0006,0x005b,0x004f,0x0066,0x006d,0x007d,0x0007,0x007f,0x006f};
	int i;
	while(1)
	{
		for(i = 0;i <= 9;i++)
		{
			P0 = led_mod[i];
			delay(500);
		}
	}
}