#include<regx52.h>
#include<intrins.h>
unsigned char temp;
unsigned char key;
unsigned char code LEDCode[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//共阴极数码管字模
void delay(unsigned int xms);//晶振11.0592Mhz
void keyscan(void);//键扫描函数
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
	/* P1^0~P1^3行高电平，P1^4~P1^7列扫描 */
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