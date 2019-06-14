#include<regx52.h>
sbit LED = P2^0;
unsigned int Hz = 400;
void main(void)
{
	//TMOD = 0x02;
	//TH0 = 244;
	//TL0 = 244;
	//EA = 1;
	//ET0 = 1;
	//TR0 = 1;
	while(1)
	{
		LED = ~LED;
		LED = ~LED;
		LED = ~LED;
		LED = ~LED;
	}
}
//void timer() interrupt 1
//{
//	LED = ~LED;
//}