//实验一 甲机-发送端
#include<regx52.h>
void main(void)
{
	unsigned char temp;
	TMOD = 0x20;//计数器1，工作方式2
	SCON = 0x40;//0100 0000, 设置为工作方式1,串行接受允许位REN=0
	PCON = 0x00;//波特率不加倍
	TH1 = 0xfd;
	TL1 = 0xfd;
	EA = 1;
	TR1 = 1;
	P1 = 0xff;
	while(1)
	{
		temp = P1;
		SBUF = temp;
		while(!TI);//等待TI=1即数据发送完成
		TI = 0;//清除发送标志位
	}
}