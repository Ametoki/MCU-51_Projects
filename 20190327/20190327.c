#include<regx51.h>
void delay(unsigned int time)
{	
	unsigned int j=0;
		for(;time>0;time--)
			for(j=0;j<125;j++);
}
void main()
{ 
	bit dir=0,run=0; //run��ͣ��־��dir�����־ ��ʼ��
	char i;
	
	char led[]={0xfe,0x0fd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//P2�����Ʊ���
	while(1)
	{ 
		switch(P3&0xf0)//�ް���
		{
     	case 0xe0:
				delay(50);
				if ((P3&0xf0)==0xe0) {run=1;break;}//K1������
			case 0xd0:
				delay(50);
				if ((P3&0xf0)==0xd0) {run=0,dir=0;break;}//K2ֹͣ��
			case 0xb0:
				delay(50);
				if ((P3&0xf0)==0xb0) {dir=1;break;}//K3�������϶���
			case 0x70:
				delay(50);
				if ((P3&0xf0)==0x70) {dir=0;break;}//K4�������¶���
		}
			if(run)
			{
				if(dir)
					for(i=0;i<=7;i++)//��dir=1,run=1,���϶���
				{
				P1=led[i];
				delay(200);
				}
				else
					for(i=7;i>=0;i--)//��dir=0,run=1,���¶���
				{
					P1=led[i];
					delay(200);
				}
			}
			else P1=0xff;//��run=0,��ȫ��
	}
}