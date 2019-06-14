/***
适用于11.0592MHz的实验板的精准秒表
功能介绍：
	外部中断0按键：开始&暂停
	外部中断1按键：清零并停止
	矩阵按键功能：
	SEC+秒加   SEC-秒减
	MIN+分加   MIN-分减
	HOUR+时加  HOUR-时减
	QUICK上一次调整的数变成进制的一半，例如上一次更改了分钟，就会把分钟改为30
	调整时间后可以立即刷新显示，暂停、停止时也可以调整时间
	可以使用矩阵按键中的QUICK按键快速调时
***/
#include<regx52.h>
sbit Disp_EN = P1^0;
sbit FM = P0^0;
unsigned char code Disp_Tab_2[] = {0x77,0x14,0xb3,0xb6,0xd4,0xe6,0xe7,0x34,0xf7,0xf6,0x00};
unsigned char i, j, sec = 0, min = 0, hour = 0, flag[4] = {0};//flag数组
unsigned int t = 0;
unsigned long int temp;
void Init(void);
void Disp(void);
void Button_Switch(void);
void Beep(void);
void delay(unsigned int i);
void main(void)
{
	Init();
	while(1)
	{
		for(hour = 0;hour <= 24;hour++)
		{
			for(min = 0;min <= 59;min++)
			{
				for(sec = 0;sec <= 59;sec++)
				{
					/*flag[0]为时间判断，在定时中断里每过1秒flag[0]会置1，
					在main函数中用while(!flag[0])操作反复查询并等待flag[0]变为1，
					flag[0]为1时跳出键盘扫描循环，执行sec变量的+1操作。
					*/
					while(!flag[0])//flag为1时跳出循环
						Button_Switch();
					flag[0] = 0;
				}
			}
		}
	}
}
void Init(void)//初始化函数
{
	TMOD = 0x02;//定时计数器0工作方式2
	SCON = 0x00;
	TH0 = 0x80;//初值为十进制的128，每过256 - 128 = 128个机器周期就溢出并向CPU申请中断
	TL0 = 0x80;
	ET0 = 1;
	EX0 = 1;
	EX1 = 1;
	IT0 = 1;
	EA = 1;
	P2 = 0xff;
	Disp();
}
void Disp(void)//显示函数
{
	temp = sec + min * 1E2 + hour * 1E4;//将秒、分、时连起来保存至一个长整型变量中，以缩减后面的代码量
	for(i = 0;i < 8;i++)
	{
		Disp_EN = 1;
		if(i == 2||i == 5)
		{
			SBUF = 0x80;
		}
		else
		{
			SBUF = Disp_Tab_2[temp % 10];//将temp中的数字逐位发送至74HC164
			temp /= 10;
		}
		while(!TI);
		TI = 0;
		Disp_EN = 0;
	}
}
void Button_Switch(void)
{
	switch(P2)
	{
		case 0xfe:
			while(P2 ^ 0xff);//等待按键抬起
			if(sec <= 58)
				sec++;
			else
				sec = 0;
			flag[1] = 1;//标记当前操作的数是秒、分还是时，对第7个按键的功能有用
			Disp();//每次按键操作后刷新一次数码管显示
			Beep();
			break;
		case 0xfd:
			while(P2 ^ 0xff);
			if(min <= 58)
				min++;
			else
				min = 0;
			flag[2] = 1;
			Disp();
			Beep();
			break;
		case 0xfb:
			while(P2 ^ 0xff);
			if(hour <= 22)
				hour++;
			else
				hour = 0;
			flag[3] = 1;
			Disp();
			Beep();
			break;
		case 0xef:
			while(P2 ^ 0xff);
			if(sec >= 2)
				sec--;
			else
				sec = 59;
			flag[1] = 1;
			Disp();
			Beep();
			break;
		case 0xdf:
			while(P2 ^ 0xff);
			if(min >= 1)
				min--;
			else
				min = 59;
			flag[2] = 1;
			Disp();
			Beep();
			break;
		case 0xbf:
			while(P2 ^ 0xff);
			if(hour >= 1)
				hour--;
			else
				hour = 23;
			flag[3] = 1;
			Disp();
			Beep();
			break;
			/**
		case 6:
			while(P2 ^ 0xff);
			for(j = 1;j < 4;j++)//用循环查询上次用按键改变的数位是秒、分还是时
			{
				if(flag[j] == 1)
				{
					if(j == 1)
						sec = 30;
					else if(j == 2)
						min = 30;
					else if(j == 3)
						hour = 12;
					flag[j] = 0;//将相应的flag标志位置零
				}
			}
			Disp();
			break;
			**/
		case 0xff: break;
	}
}
void Beep(void)
{
	FM = 1;
	delay(70);
}
void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 112; j > 0; j--);
}

void Start_Pause(void) interrupt 0//开始&暂停函数
{
	EX0 = 0;
	TR0 = ~TR0;
	EX0 = 1;
}
void Timer0(void) interrupt 1
{
	t++;
	/***128 * 7200 = 921600，在11.0592Mhz晶振下的921600个机器周期，
	除以(11.0592 / 12)的比例关系，正好是1000000微秒，即1秒***/
	if(t == 7200)
	{
		t = 0;
		flag[0] = 1;//每过1秒flag[0]置1，以在main函数里进行sec+1
		Disp();
	}
}
void Reset(void) interrupt 2
{
	TR0 = 0;
	sec = 0;
	min = 0;
	hour = 0;
	Disp();
}