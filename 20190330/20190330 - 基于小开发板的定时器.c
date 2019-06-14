#include<regx52.h>
#include<intrins.h>
sbit fm = P2^3;
sbit CLR = P2^4;
void delay(unsigned int i);
void Timer0_Init(void);
void Beep(void);
void Alarm(void);
//void bibibi(int i);
//共阴极数码管字模0 - 9 //增加“-”
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};
//共阴极数码管位选控制 0 - 7
unsigned char code dispbit[4]={0x1f,0x2f,0x4f,0x8f};
void main(void)
{
	SCON = 0x00;
	CLR = 0;
	unsigned char r = 0, s = 0;
	bit hour;
	unsigned int i, o, p , q;
	unsigned int LedOut[8];
	Timer0_Init();
	while(1)
	{
		for(o = 0;o <= 24;o++)
		{
			for(p = 0;p <= 59;p++)
			{
				for(q = 0;q <= 59;q++)
				{
					if(o == 14 && p == 1 && q == 0)
						Alarm();
					if(hour == 0)
					{
						LedOut[0] = Disp_Tab[p / 10];
						LedOut[1] = Disp_Tab[p % 10]|0x80;
						LedOut[2] = Disp_Tab[q / 10];
						LedOut[3] = Disp_Tab[q % 10];
					}
					else if(hour == 1 && s <= 9)
					{
						LedOut[0] = Disp_Tab[p / 10];
						LedOut[1] = Disp_Tab[p % 10]|0x80;
						LedOut[2] = Disp_Tab[q / 10];
						LedOut[3] = Disp_Tab[q % 10];
					}
					else if(hour == 1 && s > 9 && s <= 14)
					{
						LedOut[0] = Disp_Tab[o / 10];
						LedOut[1] = Disp_Tab[o % 10];
						LedOut[2] = Disp_Tab[p / 10];
						LedOut[3] = Disp_Tab[p % 10];
					}
					else if(hour == 1 && s > 14 && s <= 18)
					{
						LedOut[0] = Disp_Tab[2];
						LedOut[1] = Disp_Tab[0];
						LedOut[2] = Disp_Tab[1];
						LedOut[3] = Disp_Tab[9];
					}
					while(1)
					{
						for(i = 0;i <= 3;i++)
						{
							SBUF = LedOut[i];
							P2 = i;
							delay(3);
							CLR = 1;
						}
						switch(P3)
							{
								  case 0xef:
									delay(60); 
									if(P3 == 0xef)
									{
										if (o <= 23){o++;Beep();break;}
										else{o = 0;Beep();break;}
									}
									case 0xdf:
									delay(60); 
									if(P3 == 0xdf)
									{
										if (p <= 59){p++;Beep();break;}
										else{p = 0;Beep();break;}
									}
									case 0xbf:
									delay(60); 
									if(P3 == 0xbf)
									{
										if (q <= 59){q++;Beep();break;}
										else{q = 0;Beep();break;}
									}
									case 0x7f:
									delay(60); 
									if(P3 == 0x7f)
									{
										if(hour == 0){hour = 1;Beep();break;}
										else{hour = 0;Beep();break;}
									}
						//case 0xfe : if(q <= 58){q++; break;}else{q = 0; break;}
						//case 0xfd : if(p <= 58){p++; break;}else{p = 0; break;}
						//case 0xfb : if(o <= 23){o++; break;}else{o = 0; break;}
						//case 0xef : if(q >= 2){q--; break;}else{q = 59; break;}
						//case 0xdf : if(p >= 1){p--; break;}else{p = 59; break;}
						//case 0xbf : if(o >= 1){o--; break;}else{o = 23; break;}
						//case 0xff : break;
							}
						if(TF0 == 1)
						{
							TF0 = 0;
							r++;
						}
						if(r >= 7 && ((hour == 0) || (hour == 1 && s <=9)))
						{
							LedOut[1] = Disp_Tab[p % 10];
						}
						if(r == 14)
						{
							r = 0;
							s++;
							if(s == 18) s = 0;
							break;
						}
					}
					/********
					if(TF0 == 1)
					{
						TF0=0;
						r++;
						if(r==14)
						{
							r=0;
							for(i = 0;i <= 3;i++)
							{
								P0 = LedOut[i];
								P2 = dispbit[i];
							}
						}
					}
					********/
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
void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}
void Timer0_Init(void)
{
   TMOD = 0x01;
   TH0 = 0;
   TL0 = 0;
   TR0 = 1;
}
void Beep(void)
{
	fm = 0;
	delay(50);
	fm = 1;
}
void Alarm(void)
{
	unsigned int i, j;
	P1 = 0xfe;	
	for(j = 0;j < 48;j++)
	{
			for(i = 0;i < 6; i++)
		{
			if(i < 4)
			{
				fm = 0;
				P1 =_crol_(P1,1);
				delay(80);
				fm = 1;
				delay(80);
			}
			else
			{
				delay(80);
			}
		}
	}
}