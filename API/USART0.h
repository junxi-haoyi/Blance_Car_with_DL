#ifndef __USART0__
#define __USART0__

#include "gd32f4xx.h"
#include <stdio.h>




void USART_Init(void);
void USART_Send_String(uint8_t  *str);
#endif 