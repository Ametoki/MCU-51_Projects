//实验二 甲乙两机程序相同
#include<regx52.h>
unsigned char temp;
void main(void)
{
	TMOD = 0x20;
	SCON = 0x50;
	PCON = 0x00;
	TH1 = 0xfd;
	TL1 = 0xfd;
	ES = 1;
	EA = 1;
	TR1 = 1;
	P2 = 0xff;//P2口置为输入状态
	//引发串行接收中断服务程序前，甲乙需要互相发送第一次的数据
	temp = P2;//P2口状态保存至temp
	SBUF = temp;//temp赋给SBUF发送出去
	while(!TI);//当TI=1的时候发送完毕
	TI = 0;//清除发送中断标志位
	while(1);
}
void uart(void) interrupt 4//接收到数据后进入这个串行接收中断
{
	temp = SBUF;//将SBUF接收到的数据保存至temp
	RI = 0;//清除接收中断标志位
	P0 = temp;//改变灯的状态
	temp = P2;//P2口开关状态保存到temp
	SBUF = temp;//发送
	while(!TI);
	TI = 0;
}