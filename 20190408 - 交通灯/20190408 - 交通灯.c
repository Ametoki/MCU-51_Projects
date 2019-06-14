#include<regx52.h>
#define ON 0
#define OFF 1
sbit HR = P1^0;
sbit HY = P1^1;
sbit HG = P1^2;
sbit LR = P1^3;
sbit LY = P1^4;
sbit LG = P1^5;
//共阴极数码管字模0 - 9 //增加“-”符号
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};
void delay(unsigned int xms);
void main(void)
{
	unsigned int i;
	P1 = 0xf3;//P1口初值，行绿灯亮，列红灯亮
	P0 = 0x00;//数码管2初始化
	P2 = 0x00;//数码管1初始化
	while(1)
	{
		if(~HY)//判断行黄灯是否亮，若亮则熄灭，垂直方向亮绿灯
		{
			HY = ~HY;
			LG = ~LG;
		}
		else if(~LY)//判断列黄灯是否亮，若亮则熄灭，垂直方向亮绿灯
		{
			LY = ~LY;
			HG = ~HG;
		}
		for(i = 4;i > 0;i--)
		{
			if(~HG)//行绿灯亮时数码管1显示倒计时
				P2 = Disp_Tab[i];
			else if(~LG)//列绿灯亮时数码管2显示倒计时
				P0 = Disp_Tab[i];
			delay(1000);
		}
		if(~HG)
			P2 = Disp_Tab[0];//绿灯闪烁时对应数码管显示0
		else 
			P0 = Disp_Tab[0];
		for(i = 0;i < 9;i++)
		{
			if(~LR)//若列红灯在亮，闪行绿灯
				HG = ~HG;
			else if(~HR)//若行红灯在亮，闪列绿灯
				LG = ~LG;
			delay(100);
		}
		P2 = 0x00;//数码管熄灭
		P0 = 0x00;//数码管熄灭
		if(~LR)//若列红灯在亮，点亮行黄灯
			HY = ~HY;
		else //若行红灯在亮，点亮列黄灯
			LY = ~LY;
		delay(2000);
		HR = ~HR;//红灯状态取反
		LR = ~LR;//红灯状态取反，进行下次循环
/*		HG = ON;
		LR = ON;
		for(i = 4;i > 0;i--)
		{
			P2 = Disp_Tab[i];
			delay(1000);
		}
		P2 = Disp_Tab[0];
		for(i = 0;i < 9;i++)
		{
			HG = ~HG;
			delay(100);
		}
		P2 = 0x00;
		HY = ON;
		delay(2000);
		HY = OFF;
		HR = ON;//横向第一遍结束
		LR = OFF;
		LG = ON;
		for(i = 4;i > 0;i--)
		{
			P0 = Disp_Tab[i];
			delay(1000);
		}
		P0 = Disp_Tab[0];
		for(i = 0;i < 9;i++)
		{
			LG = ~LG;
			delay(100);
		}
		P0 = 0x00;
		LY = ON;
		delay(2000);
		LY = OFF;
		HR = OFF;
*/
	}
}
void delay(unsigned int xms)
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}