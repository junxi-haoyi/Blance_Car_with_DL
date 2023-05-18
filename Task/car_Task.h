#ifndef __CAR_TASK__
#define __CAR_TASK__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "systick.h"
#include "string.h"


#include "I2C0.h"
#include "LED.h"
#include "motor.h"
#include "PWM.h"
#include "USART2.h"
#include "LED.h"
#include "KEY.h"

extern QueueHandle_t queue_usart;


void freeRTOS_Task_Start(void);
void task_Create(void *pvParameters);


#endif /*CAR_TASK*/

