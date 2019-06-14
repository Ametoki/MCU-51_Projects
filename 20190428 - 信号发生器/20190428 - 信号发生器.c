#include<regx52.h.>
void init_com(void) 
{ 
    SCON=0x50; //串口工作方式1，8位UART，波特率可变  
    TH2=0xFF;           
    TL2=0xFD;   //波特率:115200 晶振=11.0592MHz 
    RCAP2H=0xFF;   
    RCAP2L=0xFD; //16位自动再装入值 

/*****************/
    TCLK=1;   
    RCLK=1;   
    C_T2=0;   
    EXEN2=0; //波特率发生器工作方式

/*****************/
    TR2=1 ; //定时器2开始
}