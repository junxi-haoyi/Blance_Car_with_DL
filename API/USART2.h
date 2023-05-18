#ifndef __USART0__
#define __USART0__

#include "gd32f4xx.h"
#include <stdio.h>
#include "car_Task.h"
#include "string.h"
#include "DMA0.h"


extern uint8_t usart_rx_list[500];
extern uint8_t word_length;


void USART2_Init(void);
void USART_Send_String(uint8_t  *str);


#endif 