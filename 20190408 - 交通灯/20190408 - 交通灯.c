#include<regx52.h>
#define ON 0
#define OFF 1
sbit HR = P1^0;
sbit HY = P1^1;
sbit HG = P1^2;
sbit LR = P1^3;
sbit LY = P1^4;
sbit LG = P1^5;
//�������������ģ0 - 9 //���ӡ�-������
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};
void delay(unsigned int xms);
void main(void)
{
	unsigned int i;
	P1 = 0xf3;//P1�ڳ�ֵ�����̵������к����
	P0 = 0x00;//�����2��ʼ��
	P2 = 0x00;//�����1��ʼ��
	while(1)
	{
		if(~HY)//�ж��лƵ��Ƿ�����������Ϩ�𣬴�ֱ�������̵�
		{
			HY = ~HY;
			LG = ~LG;
		}
		else if(~LY)//�ж��лƵ��Ƿ�����������Ϩ�𣬴�ֱ�������̵�
		{
			LY = ~LY;
			HG = ~HG;
		}
		for(i = 4;i > 0;i--)
		{
			if(~HG)//���̵���ʱ�����1��ʾ����ʱ
				P2 = Disp_Tab[i];
			else if(~LG)//���̵���ʱ�����2��ʾ����ʱ
				P0 = Disp_Tab[i];
			delay(1000);
		}
		if(~HG)
			P2 = Disp_Tab[0];//�̵���˸ʱ��Ӧ�������ʾ0
		else 
			P0 = Disp_Tab[0];
		for(i = 0;i < 9;i++)
		{
			if(~LR)//���к�������������̵�
				HG = ~HG;
			else if(~HR)//���к�������������̵�
				LG = ~LG;
			delay(100);
		}
		P2 = 0x00;//�����Ϩ��
		P0 = 0x00;//�����Ϩ��
		if(~LR)//���к�������������лƵ�
			HY = ~HY;
		else //���к�������������лƵ�
			LY = ~LY;
		delay(2000);
		HR = ~HR;//���״̬ȡ��
		LR = ~LR;//���״̬ȡ���������´�ѭ��
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
		HR = ON;//�����һ�����
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