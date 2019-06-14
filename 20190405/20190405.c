#include<regx52.h>
#include<intrins.h>
unsigned char temp;
unsigned char key;
unsigned char code LEDCode[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//�������������ģ
void delay(unsigned int xms);//����11.0592Mhz
void keyscan(void);//��ɨ�躯��
void main(void)
{
	while(1)
	{
		keyscan();
		P2 = LEDCode[key];
	}
}
void keyscan(void)
{
	int i = 0;
	unsigned char h = 0xEF, l = 0x0F;//h = 0xE0;
	/* P1^0~P1^3�иߵ�ƽ��P1^4~P1^7��ɨ�� */
	for(i = 0;i < 4;i++)
	{
		P1 = (h & 0xF0) | l;
		if((P1 & 0x0F) != 0x0F)
		{
			delay(10);
			if((P1 & 0x0F) != 0x0F)
			{
				switch(P1&0x0F)
				{
					case 0x0E:
						key = i;
						break;
					case 0x0D:
						key = 4 + i;
						break;
					case 0x0B:
						key = 8 + i;
						break;
					case 0x07:
						key = 12 + i;
						break;
					default: break;
				}
			}
		}
			h = _crol_(h, 1);
			//h = (~(((~h) & 0xF0) << 1)) & 0xF0;
	}
}
void delay(unsigned int xms)		
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}