//实验一 乙机-接收端
#include<regx52.h>
void main(void)
{
	TMOD = 0x20;//计数器1，工作方式2
	SCON = 0x50;//0101 0000, 设置为工作方式1,串行接受允许位REN=1
	PCON = 0x00;//波特率不加倍
	TH1 = 0xfd;
	TL1 = 0xfd;
	ES = 1;//串行接收中断允许位，接收到数据后会引起一次中断
	EA = 1;
	TR1 = 1;
	while(1);
}
void receive(void) interrupt 4 //串行接收中断服务函数
{
	unsigned char temp;
	temp = SBUF;
	P1 = temp;
	RI = 0;
}