#include "car_Task.h"



/*******    Stacks Setting   **********/
#define FREERTOS_TASK_START_SIZE    128
#define BLINK_LEDE_STACK_SIZE    50
#define KEY_GET_STACK_SIZE    50
/************************************/


/*******    Queue Handlers   **********/
QueueHandle_t   key_queue;
QueueHandle_t   big_date_queue;
uint8_t buffers[100] = {"it must be a great car that is"};
/************************************/


/*******   Task Handlers   **********/
TaskHandle_t    task_Create_Handler;
TaskHandle_t    blink_LEDE_Handler;
TaskHandle_t    get_Key_Handler;
/************************************/



/*******        Tasks       **********/
void freeRTOS_Task_Start(void);
void task_Create(void *pvParameters);
void blink_LEDE(void *pvParameters);
void key_Get(void *pvParameters);
/************************************/






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

    // Create Key queue
    key_queue = xQueueCreate( 3, sizeof(uint8_t));
    if(key_queue == NULL)
    {
        printf("Key Queue Create fail\n");
        while(1);
    }
    else
    {
        printf("Key Queue Create successfully\n");
    }

    // Create big data queue
    big_date_queue = xQueueCreate(1, sizeof(char*));
    if(big_date_queue == NULL)
    {
        printf("big_date_queue Queue Create fail\n");
        while(1);
    }
    else
    {
        printf("big_date_queue Queue Create successfully\n");
    }


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

    xTaskCreate((TaskFunction_t                 )   key_Get,
                (const char*                    )   "key_Get",
                (configSTACK_DEPTH_TYPE         )   BLINK_LEDE_STACK_SIZE,
                (void *                         )   NULL,
                (UBaseType_t                    )   2,
                (TaskHandle_t*                  )   &blink_LEDE_Handler
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
 * @brief get key value
 * 
 * @param pvParameters 
 * @return void
 */
void key_Get(void *pvParameters)
{
    uint8_t key = 0;
    BaseType_t state = 0;
    while(1)
    {
        key = KEY_READ();
        if(key)
        {
            state = xQueueSend(key_queue, key, portMAX_DELAY);
            if(state == errQUEUE_FULL)
            {
                printf("write key queue fail\n");
            }
        }
        
        vTaskDelay(10);
    }
}

/**
 * @brief Get the Remain Stacks object
 * 
 * @param pvParameters
 * 
 * @return void 
 */
void get_Remain_Stacks(void *pvParameters)
{

}

