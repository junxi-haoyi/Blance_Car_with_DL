#include "TIM.h"




/**
 * @brief Init of TIMER7_CH0---PC6
 * @details APB2---120M
 */
void PWMA_Init(void)
{
    // open GPIOC and Timer7 Clock 
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_TIMER7);

    // Set Timer7 Division 
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);

    // GPIO Mode Set
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_af_set(GPIOC, GPIO_AF_3, GPIO_PIN_6);

    // Timer7 Setting
    timer_parameter_struct timerA_struct_init;
    timer_oc_parameter_struct timerA_oc_struct_init;

    timerA_struct_init.prescaler = 6000U - 1;
    timerA_struct_init.alignedmode = TIMER_COUNTER_EDGE;
    timerA_struct_init.counterdirection = TIMER_COUNTER_UP;
    timerA_struct_init.clockdivision = TIMER_CKDIV_DIV4;
    timerA_struct_init.period = 1000 - 1;
    timerA_struct_init.repetitioncounter = 0;

    timer_init(TIMER7, &timerA_struct_init);
 
    timerA_oc_struct_init.outputstate = TIMER_CCX_ENABLE;
    // timerA_oc_struct_init.outputnstate;
    timerA_oc_struct_init.ocpolarity = TIMER_OC_POLARITY_HIGH;
    // timerA_oc_struct_init.ocnpolarity;
    // timerA_oc_struct_init.ocidlestate;
    // timerA_oc_struct_init.ocnidlestate;
    timer_channel_output_config(TIMER7, TIMER_CH_0, &timerA_oc_struct_init);
    timer_channel_output_mode_config(TIMER7, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER7, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);
    timer_auto_reload_shadow_enable(TIMER7);
    timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_0, 0);

    timer_primary_output_config(TIMER7, ENABLE);// MOST Significant Part ,if it isn't opened ,the PWM wiil not output from Channel
 
    timer_channel_output_state_config(TIMER7, TIMER_CH_0, TIMER_CCX_ENABLE);

    timer_enable(TIMER7);


}





/**
 * @brief Init of TIMER0_CH1---PA9
 * 
 */
void PWMB_Init(void)
{
        // open GPIOC and Timer7 Clock 
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_TIMER0);

    // Set Timer7 Division 
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);

    // GPIO Mode Set
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);

    // Timer7 Setting
    timer_parameter_struct timerA_struct_init;
    timer_oc_parameter_struct timerA_oc_struct_init;

    timerA_struct_init.prescaler = 6000U - 1;
    timerA_struct_init.alignedmode = TIMER_COUNTER_EDGE;
    timerA_struct_init.counterdirection = TIMER_COUNTER_UP;
    timerA_struct_init.clockdivision = TIMER_CKDIV_DIV4;
    timerA_struct_init.period = 1000 - 1;
    timerA_struct_init.repetitioncounter = 0;

    timer_init(TIMER0, &timerA_struct_init);
 
    timerA_oc_struct_init.outputstate = TIMER_CCX_ENABLE;
    // timerA_oc_struct_init.outputnstate;
    timerA_oc_struct_init.ocpolarity = TIMER_OC_POLARITY_HIGH;
    // timerA_oc_struct_init.ocnpolarity;
    // timerA_oc_struct_init.ocidlestate;
    // timerA_oc_struct_init.ocnidlestate;
    timer_channel_output_config(TIMER0, TIMER_CH_1, &timerA_oc_struct_init);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_1, TIMER_OC_SHADOW_ENABLE);
    timer_auto_reload_shadow_enable(TIMER0);
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, 0);

    timer_primary_output_config(TIMER0, ENABLE);// MOST Significant Part ,if it isn't opened ,the PWM wiil not output from Channel

    timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_ENABLE);

    timer_enable(TIMER0);

}



