//适用于11.0592MHz的实验板的精准秒表
#include<regx52.h>
unsigned char code Disp_Tab_2[] = {0x77,0x14,0xb3,0xb6,0xd4,0xe6,0xe7,0x34,0xf7,0xf6,0x00};
unsigned int t = 0, i, sec = 0, temp;
void init(void);
void main(void)
{
	init();
	while(1);
}
void init(void)
{
	TMOD = 0x02;
	SCON = 0x00;
	TH0 = 0x80;
	TL0 = 0x80;
	ET0 = 1;
	EX0 = 1;
	EX1 = 1;
	IT0 = 1;
	EA = 1;
}
void start(void) interrupt 0
{
	EX0 = 0;
	TR0 = 1;
	EX0 = 1;
}
void timer0(void) interrupt 1
{
	t++;
	if(t == 7200)
	{
		t = 0;
		if(sec < 99)
			sec++;
		else if(sec == 99)
			sec = 0;
		temp = sec;
		for(i = 0;i < 2;i++)
		{
			SBUF = Disp_Tab_2[temp % 10];
			temp /= 10;
		}
	}
}
void stop(void) interrupt 2
{
	EX1 = 0;
	TR0 = 0;
	EX1 = 1;
}