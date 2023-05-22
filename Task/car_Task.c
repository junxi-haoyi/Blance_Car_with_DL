#include "car_Task.h"



/*******    Stacks Setting   **********/
#define FREERTOS_TASK_START_SIZE   128
#define BLINK_LEDE_STACK_SIZE      50
#define MOTOER_TEST_STACK_SIZE     50
#define USART_RX_PRINT_STACK_SIZE  50
/************************************/


/*******    Queue Handlers   **********/
QueueHandle_t   key_queue;
QueueHandle_t   big_date_queue;
uint8_t buffers[100] = {"it must be a great car that is"};
/************************************/


/*******    Signal volume   **********/
SemaphoreHandle_t binary_Task_binary;
/************************************/


/*******   Task Handlers   **********/
TaskHandle_t    task_Create_Handler;
TaskHandle_t    blink_LEDE_Handler;
TaskHandle_t    motor_test_Handler;
/************************************/



/*******        Tasks       **********/
void freeRTOS_Task_Start(void);
void task_Create(void *pvParameters);
void blink_LEDE(void *pvParameters);
void motor_test(void *pvParameters);
/************************************/


/********* List Defination **********/
QueueHandle_t queue_usart;
/************************************/


static uint8_t i = 5;




/**
 * @brief FreeRTOS task Initialization
 * 
 * @param void
 * 
 * @return void
 * 
 */
void freeRTOS_Task_Start(void)
{
    taskENTER_CRITICAL();

    xTaskCreate((TaskFunction_t                 )   task_Create,
                (const  char*                   )   "task_Create",
                (configSTACK_DEPTH_TYPE         )   FREERTOS_TASK_START_SIZE,
                (void *                         )   NULL,
                (UBaseType_t                    )   3,
                (TaskHandle_t*                  )   &task_Create_Handler);
    
    taskEXIT_CRITICAL();

    vTaskStartScheduler();
}





/**
 * @brief Task Create 
 * 
 * @param pvParameters 
 */
void task_Create(void *pvParameters)
{

    xTaskCreate((TaskFunction_t                 )   blink_LEDE,
                (const char*                    )   "blink_LEDE",
                (configSTACK_DEPTH_TYPE         )   BLINK_LEDE_STACK_SIZE,
                (void *                         )   NULL,
                (UBaseType_t                    )   2,
                (TaskHandle_t*                  )   &blink_LEDE_Handler
    );

    xTaskCreate((TaskFunction_t                 )   motor_test,
                (const char*                    )   "motor_test",
                (configSTACK_DEPTH_TYPE         )   MOTOER_TEST_STACK_SIZE,
                (void *                         )   NULL,
                (UBaseType_t                    )   5,
                (TaskHandle_t*                  )   &motor_test_Handler
    );



    vTaskDelete(NULL);
}



/**
 * @brief 
 * 
 * @param pvParameters 
 * @return void
 */
void blink_LEDE(void *pvParameters)
{
    while(1)
    {
        // gpio_bit_set(GPIOE, GPIO_PIN_3);
        gpio_bit_toggle(GPIOE, GPIO_PIN_3);
        //delay_1ms(500);
        vTaskDelay(500);
    }
}


/**
 * @brief motor test ¶æ»ú²âÊÔ
 * 
 * @param pvParameters 
 */
void motor_test(void *pvParameters)
{
    while (1)
    {
        if(i==27)delay_1ms(1000);
        gpio_bit_set(GPIOC, GPIO_PIN_9);
        delay_1ms(i);
        gpio_bit_reset(GPIOC, GPIO_PIN_9);
        if(key_Scan())
        {
            i++;
        }
        if(i >= 27)i=5;
        vTaskDelay(500-i);
    } 
}









/**
 * @brief Get the Remain Stacks object
 * 
 * @param pvParameters
 * 
 * @return void 
 */
// void get_Remain_Stacks(void *pvParameters)
// {
//     uxSemaphoreGetCount    
// }
