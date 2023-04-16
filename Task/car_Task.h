#ifndef __CAR_TASK__
#define __CAR_TASK__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "systick.h"


#include "I2C0.h"
#include "LED.h"
#include "motor.h"
#include "PWM.h"
#include "USART0.h"
#include "LED.h"
#include "KEY.h"


void freeRTOS_Task_Start(void);
void task_Create(void *pvParameters);


#endif /*CAR_TASK*/

