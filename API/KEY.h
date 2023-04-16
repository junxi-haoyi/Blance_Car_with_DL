#ifndef __KEY__
#define __KEY__

#include "gd32f4xx.h"

#define KEY_READ() gpio_input_bit_get(GPIOA, GPIO_PIN_0)

void key_Init(void);

#endif /*KEY*/
