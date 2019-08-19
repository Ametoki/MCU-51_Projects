//频率测量 + 1602A显示屏
#include<regx52.h>
#include<intrins.h>
#define RS_CLR RS=0
#define RS_SET RS=1
#define RW_CLR RW=0
#define RW_SET RW=1
#define EN_CLR EN=0
#define EN_SET EN=1
#define DataPort P0
#define delayNOP(); {_nop_();};
sbit RS = P2^6;//P3^5;
sbit RW = P2^5;//P2^5;
sbit EN = P2^7;//P3^4;
sbit key1 = P3^1;
sbit key2 = P3^0;
sbit SCK = P1^0;//p1.0口模拟时钟输出
sbit MOSI = P1^1;//p1.1口模拟主机输出
sbit MISO = P1^2;//p1.2口模拟主机输入
sbit SS1 = P1^3;//p1.3口模拟片选
unsigned int COM = 2;
unsigned char ch[5] = {0x23, 0x31, 0x32, 0x33, 0x34};
unsigned char Data[5];
bit LCD_Check_Busy(void);
void Init(void);
void LCD_Clear(void);
void LCD_Write_Com(unsigned char com);
void LCD_Write_Data(unsigned char Data);
void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s);
void LCD_Write_Char(unsigned char x, unsigned char y, unsigned char Data);
void LCD_Tag(void);
//void SPI_Send_Byte(unsigned char Data);
//unsigned char SPI_Recv_Byte(void);
unsigned char SPI_Send_Recv_Byte(unsigned char Data);
void Delay(unsigned int xms);
void main(void)
{
	unsigned char i, j;
	double fs_cnt, fx_cnt, data_clk, data_fx;
	unsigned long int temp;
	double Data[5], fs_data[5], fx_data[5], clk_data[5], weight[3] = {256, 65536, 16777216};
	while(LCD_Check_Busy());
	Init();
	while(1)
	{
		for(j = 0;j < 3;j++)
		{
					for(i = 0;i < 5;i++)
			Data[i] = SPI_Send_Recv_Byte(0);
		if(Data[0] == 0xAA)
		{
			fs_data[1] = Data[1];
			fs_data[2] = Data[2];
			fs_data[3] = Data[3];
			fs_data[4] = Data[4];
		}
		else if(Data[1] == 0xAA)
		{
			fs_data[0] = 0x20;
			fs_data[1] = Data[2];
			fs_data[2] = Data[3];
			fs_data[3] = Data[4];
			fs_data[4] = Data[0];
		}
		else if(Data[2] == 0xAA)
		{
			fs_data[0] = 0x20;
			fs_data[1] = Data[3];
			fs_data[2] = Data[4];
			fs_data[3] = Data[0];
			fs_data[4] = Data[1];
		}
		else if(Data[3] == 0xAA)
		{
			fs_data[0] = 0x20;
			fs_data[1] = Data[4];
			fs_data[2] = Data[0];
			fs_data[3] = Data[1];
			fs_data[4] = Data[2];
		}
		else if(Data[4] == 0xAA)
		{
			fs_data[0] = 0x20;
			fs_data[1] = Data[0];
			fs_data[2] = Data[1];
			fs_data[3] = Data[2];
			fs_data[4] = Data[3];
		}
		else if(Data[0] == 0xBB)
		{
			fx_data[1] = Data[1];
			fx_data[2] = Data[2];
			fx_data[3] = Data[3];
			fx_data[4] = Data[4];
		}
		else if(Data[1] == 0xBB)
		{
			fx_data[0] = 0x20;
			fx_data[1] = Data[2];
			fx_data[2] = Data[3];
			fx_data[3] = Data[4];
			fx_data[4] = Data[0];
		}
		else if(Data[2] == 0xBB)
		{
			fx_data[0] = 0x20;
			fx_data[1] = Data[3];
			fx_data[2] = Data[4];
			fx_data[3] = Data[0];
			fx_data[4] = Data[1];
		}
		else if(Data[3] == 0xBB)
		{
			fx_data[0] = 0x20;
			fx_data[1] = Data[4];
			fx_data[2] = Data[0];
			fx_data[3] = Data[1];
			fx_data[4] = Data[2];
		}
		else if(Data[4] == 0xBB)
		{
			fx_data[0] = 0x20;
			fx_data[1] = Data[0];
			fx_data[2] = Data[1];
			fx_data[3] = Data[2];
			fx_data[4] = Data[3];
		}
		else if(Data[0] == 0xCC)
		{
			clk_data[1] = Data[1];
			clk_data[2] = Data[2];
			clk_data[3] = Data[3];
			clk_data[4] = Data[4];
		}
		else if(Data[1] == 0xCC)
		{
			clk_data[0] = 0x20;
			clk_data[1] = Data[2];
			clk_data[2] = Data[3];
			clk_data[3] = Data[4];
			clk_data[4] = Data[0];
		}
		else if(Data[2] == 0xCC)
		{
			clk_data[0] = 0x20;
			clk_data[1] = Data[3];
			clk_data[2] = Data[4];
			clk_data[3] = Data[0];
			clk_data[4] = Data[1];
		}
		else if(Data[3] == 0xCC)
		{
			clk_data[0] = 0x20;
			clk_data[1] = Data[4];
			clk_data[2] = Data[0];
			clk_data[3] = Data[1];
			clk_data[4] = Data[2];
		}
		else if(Data[4] == 0xCC)
		{
			clk_data[0] = 0x20;
			clk_data[1] = Data[0];
			clk_data[2] = Data[1];
			clk_data[3] = Data[2];
			clk_data[4] = Data[3];
		}
		}
		//temp = SPI_Recv_Byte();
		fs_cnt = fs_data[4] + fs_data[3] * weight[0] + fs_data[2] * weight[1] + fs_data[1] * weight[2];
    //Delay(100);
		fx_cnt = fx_data[4] + fx_data[3] * weight[0] + fx_data[2] * weight[1] + fx_data[1] * weight[2];
    //Delay(100);
		data_clk = clk_data[4] + clk_data[3] * weight[0] + clk_data[2] * weight[1] + clk_data[1] * weight[2];
    //Delay(100);
    data_fx = (data_clk / fs_cnt) * fx_cnt;
		temp = (unsigned long int)data_fx;
    //temp = (unsigned long int)fs_cnt;
    //temp = (unsigned long int)fx_cnt;
    //temp = (unsigned long int)data_clk;
		Delay(100);
		for(i = 15;i > 0;i--)
		{
			LCD_Write_Char(i, 1, temp % 10 + 0x30);
			temp /= 10;
		}
		/*
		LCD_Write_String(0,0,"      GET!      ");
		data_fx = Disp_Data[1] + Disp_Data[2] * 256;// + Disp_Data[3] + Disp_Data[4];
		//data_fx = Disp_Data[1] + Disp_Data[2] * 256 + Disp_Data[3] * 65536 + Disp_Data[4] * 16777216;
		temp = data_fx;
		for(i = 15;i > 0;i--)
		{
			LCD_Write_Char(i, 1, temp % 10 + 0x30);
			temp /= 10;
		}
		*/
		Delay(500);
	}
}
void Init(void)
{
	LCD_Write_Com(0x38);
	Delay(COM);
	LCD_Write_Com(0x08);
	Delay(COM);
	LCD_Write_Com(0x06);
	Delay(COM);
	LCD_Clear();
	LCD_Write_Com(0x0C);
	Delay(COM);
	SS1 = 1;
	SCK = 0;
}
bit LCD_Check_Busy(void)
{
	RS_CLR;
	RW_SET;
	EN_CLR;
	_nop_();
	EN_SET;
	return (bit)(DataPort & 0x80);
}
void LCD_Clear(void)
{
	LCD_Write_Com(0x01);
	Delay(COM);
}
void LCD_Write_Com(unsigned char com)
{
	while(LCD_Check_Busy());
	RS_CLR;
	RW_CLR;
	DataPort = com;
	EN_SET;
	_nop_();
	EN_CLR;
}
void LCD_Write_Data(unsigned char Data)
{
	while(LCD_Check_Busy());
	RS_SET;
	RW_CLR;
	DataPort = Data;
	EN_SET;
	_nop_();
	EN_CLR;	
}

void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s)
{
	if(y == 0)
	{
		LCD_Write_Com(0x80 + x);
	}
	else
	{
		LCD_Write_Com(0xC0 + x);
	}
	while(*s)
	{
		LCD_Write_Data(*s);
		s++;
	}
}

void LCD_Write_Char(unsigned char x, unsigned char y, unsigned char Data)
{
	if(y == 0)
	{
		LCD_Write_Com(0x80 + x);
	}
	else
	{
		LCD_Write_Com(0xC0 + x);
	}
	LCD_Write_Data(Data);
}
/*
void LCD_Tag(void)
{
	;
}
*/
unsigned char SPI_Send_Recv_Byte(unsigned char Data)
{
	unsigned char n;
	unsigned char data_recv = 0;
	SS1 = 0;
	for(n = 0;n < 9;n++)
	{
		SCK = 0;//时钟置为低电平
		{
		if((Data & 0x80) == 0x80)//若发送数据的最高位为1，则发送1
			MOSI = 1;
		else
			MOSI = 0;//最高位数据为0，发送0
			Data = _crol_(Data, 1);//发送数据左移一位
		}
		delayNOP();
		SCK = 1;
		if(n > 0)
		{
			data_recv = _crol_(data_recv, 1);
			if(MISO)
				data_recv = data_recv | 0x01;
			else
				data_recv = data_recv & 0xfe;
		}
		delayNOP();
	}
	SS1 = 1;
	delayNOP();
	SS1 = 0;
	return(data_recv);
}
void Delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i = xms;i > 0;i--)
		for(j = 112;j > 0;j--);
}