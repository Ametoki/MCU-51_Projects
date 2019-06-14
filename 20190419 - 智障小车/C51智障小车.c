#include<regx52.h>
#define ON 1
#define OFF 0
#define YES 0 //红外传回判断
#define NO 1 //红外传回判断
//20Hz, 0.05s = 50ms = 50000us = 一个周期, 分成100份, 每次500us
//P0_0 - P0_7 电机控制端
//P1_0 & P1_1 避障红外传回
//P1_2 - P1_5 循迹红外传回//莫得电源接口
unsigned char Time, Fast_Ratio, Slow_Ratio, COM = 10;
bit  Fast_Speed, Slow_Speed;
sbit Left_1_F = P2^0;//
sbit Left_1_R = P2^1;//***
sbit Left_2_F = P2^2;//
sbit Left_2_R = P1^3;
sbit Right_1_F = P2^4;//
sbit Right_1_R = P2^5;//***
sbit Right_2_F = P2^6;//
sbit Right_2_R = P1^7;
sbit BZ_Left = P1^0;//左避障红外传感器
sbit BZ_Right = P1^1;//右避障红外传感器
sbit XJ_Left = P1^2;//最左边的循迹
sbit XJ_MidL = P1^3;//中间靠左的循迹
sbit XJ_MidR = P1^4;//中间靠右的循迹
sbit XJ_Right = P1^5;//最右边的循迹

void Delay(unsigned int xms);
void F_Init(void);
void R_Init(void);
void main(void)
{
	TMOD = 0x01;//定时器0工作方式1
	TH0 = 0xFE;//65536-500
	TL0 = 0x0C;
	EA = 1;//开总中断
	ET0 = 1;//开定时器0中断
	TR0 = 1;//启动定时器0
	F_Init();
	R_Init();
	Delay(2000);
	while(1)
	{
		if(BZ_Left == NO && BZ_Right == NO)//无障碍物，全速
		{
			Left_1_F = ON;
			Left_2_F = ON;
			Right_1_F = ON;
			Right_2_F = ON;
			Delay(COM);
		}
		else if(BZ_Left == YES && BZ_Right == NO)
		{
			Fast_Ratio = 100;
			Slow_Ratio = 30;
			Left_1_F = Fast_Speed;
			Left_2_F = Fast_Speed;
			Right_1_F = Slow_Speed;
			Right_2_F = Slow_Speed;
			Delay(COM);
		}
		else if(BZ_Left == NO && BZ_Right == YES)
		{
			Fast_Ratio = 100;
			Slow_Ratio = 30;
			Left_1_F = Slow_Speed;
			Left_2_F = Slow_Speed;
			Right_1_F = Fast_Speed;
			Right_2_F = Fast_Speed;
			Delay(COM);
		}
		else if(BZ_Left == YES && BZ_Right == YES)
		{
			F_Init();
			Fast_Ratio = 40;
			Slow_Ratio = 80;
			Left_1_R = Slow_Speed;
			Left_2_R = Slow_Speed;
			Right_1_R = Fast_Speed;
			Right_2_R = Fast_Speed;
			Delay(750);
			R_Init();
		}
	}
}
void F_Init(void)
{
	Left_1_F = OFF;
	Left_2_F = OFF;
	Right_1_F = OFF;
	Right_2_F = OFF;
}
void R_Init(void)
{
	Left_1_R = OFF;
	Left_2_R = OFF;
	Right_1_R = OFF;
	Right_2_R = OFF;
}
void Time0(void) interrupt 1
{
	TR0 = 0;//赋初值时，关闭定时器
	TH0 = 0xFE;
	TL0 = 0x0C;
	TR0 = 1;//打开定时器
	Time++;
	if(Time >= 100)
	  Time = 0;   
	if(Time <= Fast_Ratio)//占空比由Fast_Ratio与Slow_Ratio决定
		Fast_Speed = 1;
	else if(Time > Fast_Ratio)
		Fast_Speed = 0;
	if(Time <= Slow_Ratio)
		Slow_Speed = 1;
	else if(Time > Slow_Ratio)
		Slow_Speed = 0;
}
void Delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}