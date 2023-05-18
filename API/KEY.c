#include "KEY.h"




void key_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);

    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0);
}



/**
 * @brief the value of key
 * 
 * @param void
 * 
 * @return uint8_t
 */
uint8_t key_Scan(void)
{
    if(gpio_input_bit_get(GPIOA, GPIO_PIN_0))
    {
        delay_1ms(100);
        if(gpio_input_bit_get(GPIOA, GPIO_PIN_0))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}


