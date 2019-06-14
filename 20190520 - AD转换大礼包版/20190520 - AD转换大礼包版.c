#include<regx52.h>
sbit SEG_EN = P2^7;
sbit OE = P3^5;
sbit EOC = P2^0;
sbit ALE_START = P3^7;
sbit Int0_Trigger = P3^4;
unsigned char code Disp_Tab[] = {0x77,0x14,0xb3,0xb6,0xd4,0xe6,0xe7,0x34,0xf7,0xf6,0x00};
unsigned int x;
void Init(void);
void Display(void);
void Delay(unsigned int i);
void main(void)
{
	P2 = 0x01;//ADC0808地址端输入为0;
	Init();
	while(1)
	{
		if(EOC == 1)
			Int0_Trigger = 0;
	}
}
void Delay(unsigned int i)
{
    unsigned char j;
    for(; i > 0; i--)
        for(j = 112; j > 0; j--);
}
void Display(void)
{
	char i;
	unsigned int temp;
	temp = x;
	SEG_EN = 1;
	for(i = 0;i < 8;i++)
	{
		if(i == 3)
			SBUF = Disp_Tab[temp % 10] | 0x08;
		else
			SBUF = Disp_Tab[temp % 10];
		temp /= 10;
		while(!TI)
			TI = 0;
	}
	SEG_EN = 0;
}
void Init(void)
{
	Int0_Trigger = 1;
	SCON = 0x00;
	IT0 = 1;
	EX0 = 1;
	EA = 1;
	ALE_START = 1;
	Delay(1);
	ALE_START = 0;
}
void Read_data(void) interrupt 0
{
	EX0 = 0;
	Int0_Trigger = 1;
	OE = 1;
	x = (P1 * 1E3) / 51;
	OE = 0;
	Display();
	Delay(1000);
	ALE_START = 1;
	Delay(1);
	ALE_START = 0;
	EX0 = 1;
}