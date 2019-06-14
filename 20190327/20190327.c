#include<regx51.h>
void delay(unsigned int time)
{	
	unsigned int j=0;
		for(;time>0;time--)
			for(j=0;j<125;j++);
}
void main()
{ 
	bit dir=0,run=0; //run起停标志，dir方向标志 初始化
	char i;
	
	char led[]={0xfe,0x0fd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//P2口亮灯编码
	while(1)
	{ 
		switch(P3&0xf0)//无按键
		{
     	case 0xe0:
				delay(50);
				if ((P3&0xf0)==0xe0) {run=1;break;}//K1启动键
			case 0xd0:
				delay(50);
				if ((P3&0xf0)==0xd0) {run=0,dir=0;break;}//K2停止键
			case 0xb0:
				delay(50);
				if ((P3&0xf0)==0xb0) {dir=1;break;}//K3方向自上而下
			case 0x70:
				delay(50);
				if ((P3&0xf0)==0x70) {dir=0;break;}//K4方向自下而上
		}
			if(run)
			{
				if(dir)
					for(i=0;i<=7;i++)//若dir=1,run=1,自上而下
				{
				P1=led[i];
				delay(200);
				}
				else
					for(i=7;i>=0;i--)//若dir=0,run=1,自下而上
				{
					P1=led[i];
					delay(200);
				}
			}
			else P1=0xff;//若run=0,灯全灭
	}
}