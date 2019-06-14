//比较精准的频率测量程序
#include<regx52.h>
sbit Pulse = P3^5;
unsigned char code Disp_Tab_2[] = {0x77,0x14,0xb3,0xb6,0xd4,0xe6,0xe7,0x34,0xf7,0xf6,0x00};
unsigned long int Fre;
unsigned int t = 0;
unsigned char Offset = 0x3D;//当频率大于65536时更新计数器的时间补偿
void Display(unsigned long int temp);
void main(void)
{
	TMOD = 0x52;//0101 0010
	SCON = 0x00;//串口方式0输出
	TH0 = 6;//定时器0 初值 256-250
	TL0 = 6;
	TH1 = 0;
	TL1 = 0;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
	TR1 = 1;
	while(1)
	{
		if(TH1 >= 0xE0)//计数器即将溢出时更新计数器
		{
			Fre += TH1 * 256 + TL1;
			if(TL0 >= Offset)
			TL0 -= Offset;
			TH1 = 0x00;
			TL1 = 0x00;
		}
		if(t == 0)
			Display(Fre);//每秒只刷新一次频率显示
	}
}
void timer0(void) interrupt 1
{
	t++;
	if(t == 4000)
	{
		t = 0;
		Fre += TH1 * 256 + TL1;
		TH1 = 0;
		TL1 = 0;
	}
}
void Display(unsigned long int temp)
{
	unsigned char i;
	temp = Fre;
	for(i = 0;i < 8;i++)
	{
		SBUF = Disp_Tab_2[temp % 10];
		temp /= 10;
	}
	Fre = 0;
}