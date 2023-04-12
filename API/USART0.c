#include "USART0.h"


void USART_Init(void)
{
        RCU_AHB1EN|=RCU_AHB1EN_PAEN;
    RCU_APB2EN|=RCU_APB2EN_USART0EN;

    GPIO_CTL(GPIOA)&=~GPIO_CTL_CTL10;
    GPIO_CTL(GPIOA)|=(GPIO_CTL_CTL10&(0x2U<<(10U*2)));
    GPIO_CTL(GPIOA)&=~GPIO_CTL_CTL9;
    GPIO_CTL(GPIOA)|=(GPIO_CTL_CTL9&(0x2U<<(9U*2)));

    GPIO_OMODE(GPIOA)&=~GPIO_OMODE_OM9;
    GPIO_OMODE(GPIOA)&=~GPIO_OMODE_OM10;

    //GPIO_OSPD(GPIOA)|=GPIO_OSPD_OSPD10;
    GPIO_OSPD(GPIOA)|=GPIO_OSPD_OSPD9;

    GPIO_AFSEL1(GPIOA)&=~GPIO_AFSEL1_SEL9;
    GPIO_AFSEL1(GPIOA)|=(GPIO_AFSEL1_SEL9&(GPIO_AF_7<<1*4));
    GPIO_AFSEL1(GPIOA)&=~GPIO_AFSEL1_SEL10;
    GPIO_AFSEL1(GPIOA)|=(GPIO_AFSEL1_SEL10&(GPIO_AF_7<<2*4));

    //NVIC_Config();

    usart_baudrate_set(USART0,115200);

    USART_CTL0(USART0)&=~USART_CTL0_WL;

    USART_CTL0(USART0)&=~USART_CTL0_PCEN;

    USART_CTL1(USART0)&=~USART_CTL1_STB;
 
    USART_CTL0(USART0)|=USART_CTL0_UEN;

    USART_CTL0(USART0)|=USART_CTL0_TEN;
    USART_CTL0(USART0)|=USART_CTL0_REN;

    //USART_CTL0(USART0)|=USART_CTL0_RBNEIE;
    USART_CTL0(USART0)|=USART_CTL0_IDLEIE; 
    //USART_CTL2(USART0)|=USART_CTL2_DENT;//enable DMA tansmit
    USART_CTL2(USART0)|=USART_CTL2_DENR;//enable DMA recieve
    //usart_dma_receive_config(USART0,USART_DENR_ENABLE);
    // rcu_periph_clock_enable(RCU_GPIOA);//enable GPIOA GPIOA_9:TX GPIOA_10:RX

    // rcu_periph_clock_enable(RCU_USART0);//enbale USART0 Clock

    // gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_9);//set GPIOA_PIN_9 as output mode

    // gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_10);//set GPIOA_PIN_10 as input mode

    // gpio_af_set(GPIOA,GPIO_AF_7,GPIO_PIN_9);

    // gpio_af_set(GPIOA,GPIO_AF_7,GPIO_PIN_10);

    // gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);//set GPIO_PIN_9 as PP mode

    // gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);//set GPIO_PIN_10 as ....

    // usart_baudrate_set(USART0,115200);//set baudrate as 115200

    // usart_parity_config(USART0,USART_PM_NONE);//set none parity check

    // usart_word_length_set(USART0,USART_WL_8BIT);//set data length as 8 bit

    // usart_stop_bit_set(USART0,USART_STB_1BIT);//set stop bit as 1bit

    // usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);//enbale TX 

    // usart_receive_config(USART0,USART_RECEIVE_ENABLE);//enbale RX

    // usart_enable(USART0);

    // usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);//enbale TX 

    // usart_receive_config(USART0,USART_RECEIVE_ENABLE);//enbale RX

}


int fputc(int ch,FILE *f)
{
    USART_DATA(USART0)=(USART_DATA_DATA&ch);
    while((USART_STAT0(USART0)&USART_STAT0_TBE)== 0);
    return ch;
}

void USART_Send_String(uint8_t  *str)
{
    while (str && *str)
    {
        usart_data_transmit(USART0,*(str));
        while(usart_flag_get(USART0,USART_FLAG_TBE)==RESET);// ATTENTION : the cache area must be empty instead of transmition area competion 
        str+=1;                                             //             or the first letter of String will missing
    }
    
}





