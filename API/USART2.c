#include "USART2.h"



/*********** Params ****************/
uint8_t usart_rx_list[500];
uint8_t word_length = 0;
/**********************************/


/**
 * @brief Init of USART2 
 * 
 */
void USART2_Init(void)
{


    // clock open
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_USART2);


/*********** GPIOC Mode Setting *************/
    // PC10 USART2_TX, PC11 USART2_RX 
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11);

    // GPIO AF Mode set
    gpio_af_set(GPIOC, GPIO_AF_7, GPIO_PIN_10);
    gpio_af_set(GPIOC, GPIO_AF_7, GPIO_PIN_11);

    // GPIOC output options set
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
/*******************************************/

/************ USART2 Setting ***************/
    // usart baudrate set
    usart_baudrate_set(USART2, 115200);

    // usart baudrate set
    usart_parity_config(USART2, USART_PM_NONE);

    // usart word length set
    usart_word_length_set(USART2, USART_WL_8BIT);

    // usart stop bit set
    usart_stop_bit_set(USART2, USART_STB_1BIT);

    // usart transmit config
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);

    // usart receive config
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);

    // enable USART2 RX Interrupt
    // usart_interrupt_enable(USART2, USART_INT_RBNE);

    // enable USART2 RX IDLE interrupt 
    usart_interrupt_enable(USART2, USART_INT_IDLE);

    // enable DMA Receive config
    usart_dma_receive_config(USART2, USART_RECEIVE_DMA_ENABLE);

    // usart enable
    usart_enable(USART2);
    nvic_irq_enable(USART2_IRQn, 11, 0);
}
/****************************************/



/**
 * @brief redirect printf
 * 
 * @param ch 
 * @param f 
 * @return int 
 */
int fputc(int ch,FILE *f)
{
    USART_DATA(USART2)=(USART_DATA_DATA&ch);
    while((USART_STAT0(USART2)&USART_STAT0_TBE)== 0);
    return ch;
}

/**
 * @brief USART2 Send String
 * 
 * @param str 
 */
void USART_Send_String(uint8_t  *str)
{
    while (str && *str)
    {
        usart_data_transmit(USART2,*(str));
        while(usart_flag_get(USART2,USART_FLAG_TBE) == RESET);// ATTENTION : the cache area must be empty instead of transmition area competion 
        str+=1;                                               //             or the first letter of String will missing
    }
    
}


/**
 * @brief USART2 Intrrupt function
 * 
 */
void USART2_IRQHandler(void)
{

    // if(usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE) == SET)
    // {
    //     usart_rx_list[word_length++] = usart_data_receive(USART2);

    // }


    if(usart_interrupt_flag_get(USART2, USART_INT_FLAG_IDLE) == SET)
    {
        // clear IDLE Bit
        USART_STAT0(USART2);
        USART_DATA(USART2);

        word_length = 500 - dma_transfer_number_get(DMA0, DMA_CH1);
        usart_rx_list[word_length] = '\0';
        printf("word_length : %d \n", word_length);

        dma_channel_disable(DMA0, DMA_CH1);
        dma_Init();
        printf("%s\n", usart_rx_list);
        memset(usart_rx_list, 0, 500);


    }
}







