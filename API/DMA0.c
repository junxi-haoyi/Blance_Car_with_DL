#include "DMA0.h"



void dma_Init(void)
{
    dma_deinit(DMA0, DMA_CH1);

    dma_single_data_parameter_struct dma_init_struct;

    dma_init_struct.periph_addr = (uint32_t )&USART_DATA(USART2);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;

    dma_init_struct.memory0_addr = (uint32_t )&usart_rx_list;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;

    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;

    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.number = 100;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;

    dma_single_data_mode_init(DMA0, DMA_CH1, &dma_init_struct);
    dma_interrupt_enable(DMA0, DMA_CH1, DMA_CHXCTL_FTFIE);
    dma_channel_subperipheral_select(DMA0, DMA_CH1, DMA_SUBPERI4);
    dma_channel_enable(DMA0, DMA_CH1);
    nvic_irq_enable(DMA0_Channel1_IRQn, 10, 0);
}





void DMA0_Channel1_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0, DMA_CH1, DMA_INT_FLAG_FTF) == SET)
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH1, DMA_INT_FLAG_FTF);
    }
}


