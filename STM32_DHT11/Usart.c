#include "Usart.h"

void Usart1_Init(int baud)
{
	//初始化GPIOA9引脚（TX）
	//结构体变量定义
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
	
	//使能GPIOA的时钟线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
	
	//配置GPIOA0的输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//初始化串口1
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	//使能串口外设
	USART_Cmd(USART1, ENABLE);
}

void Send_Str(char *str)
{
	while(*str != '\0')
	{
		//发送一个字符
		USART_SendData(USART1, *str);
		str++;
		//等待发送完成，等待发送完成的标注位被置1
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
}


void send_char(unsigned char ch)
{
	USART_SendData(USART1, ch);
	//等待发送完成，等待发送完成的标注位被置1
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

//重写fputc
int fputc(int ch, FILE * file)
{
	send_char(ch);
	
	return ch;
}
