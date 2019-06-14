#include<regx52.h>
sbit OE = P3^5;
sbit EOC = P3^6;
sbit ALE_START = P3^7;
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xbf};
unsigned char code Disp_bit[] = {0x0e, 0x0d, 0x0b, 0x07};
unsigned int x;
void Init(void);
void Display(void);
void Delay(unsigned int i);
void main(void)
{
	P2 = 0x0f;//ADC0808地址端输入为0;
	Init();
	while(1)
		Display();
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
	for(i = 3;i >= 0;i--)
	{
		if(i == 0)
			P0 = Disp_Tab[temp % 10] | 0x80;
		else
			P0 = Disp_Tab[temp % 10];
		temp /= 10;
		P2 = Disp_bit[i];
		Delay(10);
		P2 = 0x0f;
	}
}
void Init(void)
{
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
	OE = 1;
	x = (P1 * 1E3) / 51;
	OE = 0;
	ALE_START = 1;
	Delay(1);
	ALE_START = 0;
	EX0 = 1;
}