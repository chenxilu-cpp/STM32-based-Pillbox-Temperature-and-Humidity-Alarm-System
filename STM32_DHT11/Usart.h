#ifndef __USART_H__
#define __USART_H__

#include <stm32f10x.h>
#include <stdio.h>

void Usart1_Init(int baud);
void Send_Str(char *str);


#endif
