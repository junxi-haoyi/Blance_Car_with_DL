#include "car_Task.h"



/*******    Stacks Setting   **********/
#define FREERTOS_TASK_START_SIZE    128

/*******                    **********/


/*******     Task Handlers  **********/
TaskHandle_t    rtos_Start_Handler;
TaskHandle_t    e_Blink_Handler;

/*******                    **********/



/*******        Tasks       **********/
void freeRTOS_Task_Start(void);
void task_Create(void *pvParameters);
void blink_LEDE(void *pvParameters);

/*******                    **********/




void freeRTOS_Task_Start(void)
{
    taskENTER_CRITICAL();

    xTaskCreate((TaskFunction_t                 )   task_Create,
                (const  char*                   )   "task_Create",
                (const  configSTACK_DEPTH_TYPE  )   FREERTOS_TASK_START_SIZE,
                (void * const                   )   NULL,
                (UBaseType_t                    )   3,
                (TaskHandle_t                   )   &rtos_Start_Handler);


    vTaskDelete(task_Create);
    
    taskEXIT_CRITICAL();
}

void task_Create(void *pvParameters)
{

    xTaskCreate((TaskFunction_t                 )   blink_LEDE,
                (const char*                    )   "blink_LEDE",
                (const  configSTACK_DEPTH_TYPE  )   
    );
}




void blink_LEDE(void *pvParameters)
{
    while(1)
    {
        gpio_bit_set(GPIOE, GPIO_PIN_3);
        delay_1ms(500);
        gpio_bit_reset(GPIOE, GPIO_PIN_3);

        vTaskDelay(500);
    }
}



void get_Stack_Remaining(void)
{
    while(1)
    {   
        static uint8_t stack_num;
        stack_num = uxTaskGetStackHighWaterMark(rtos_Start_Handler);
        printf("rtos_Start_Task stack remain : %d",stack_num);

        vTaskDelay(1000);
    }
}


