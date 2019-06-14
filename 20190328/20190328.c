#include<regx51.h>
void delay(unsigned int i);
//void bibibi(int i);
sbit Beep = P1^7;
//共阴极数码管字模0 - 9 //增加“-”
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};
//共阴极数码管位选控制 0 - 7
//unsigned char code dispbit[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
void main(void)
{
	unsigned int i, j, o, p ,q, LedNumVal=1;
  unsigned int LedOut[8];
	while(1)
	{
		for(o = 0;o <= 24;o++)
		{
			for(p = 0;p <= 59;p++)
			{
				for(q = 0;q <= 59;q++)
				{
					switch(P1)
					{
						case 0xfe : if(q <= 58){q++; break;}else{q = 0; break;}
						case 0xfd : if(p <= 58){p++; break;}else{p = 0; break;}
						case 0xfb : if(o <= 23){o++; break;}else{o = 0; break;}
						case 0xef : if(q >= 2){q--; break;}else{q = 59; break;}
						case 0xdf : if(p >= 1){p--; break;}else{p = 59; break;}
						case 0xbf : if(o >= 1){o--; break;}else{o = 23; break;}
						case 0xff : break;
					}
					LedOut[0] = Disp_Tab[o / 10];
					LedOut[1] = Disp_Tab[o % 10];
					LedOut[2] = Disp_Tab[10];
					LedOut[3] = Disp_Tab[p / 10];
					LedOut[4] = Disp_Tab[p % 10];
					LedOut[5] = Disp_Tab[10];//横杠
					LedOut[6] = Disp_Tab[q / 10];//秒数十位
					LedOut[7] = Disp_Tab[q % 10];//秒数个位
					for(j = 0;j <= 56;j++) //调整速度
					{
						for(i = 0;i <= 7;i++)
						{
							P0 = LedOut[i];
							P2 = 0xf8 + i;
							delay(120);//改频闪，会影响速度
						}
					}
				}
			}
		}
	}
}
/*
void bibibi(int i)
{
	 Beep= 1;
	 delay(i);
	 Beep= 0;
	 delay(i);
}
*/
void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}
