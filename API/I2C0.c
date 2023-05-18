#include "I2C0.h"


#ifdef HARD_I2C0


void I2C_Init(void)
{
    //Enable I2C0 and GPIOB clock
    rcu_periph_clock_enable(RCU_GPIOB);


     //GPIO af set
    gpio_af_set(GPIOB,GPIO_AF_4,GPIO_PIN_6);
    gpio_af_set(GPIOB,GPIO_AF_4,GPIO_PIN_7);

    //GPIO_PIN_7 :I2C0_SDA  GPIOB_PIN_6 :I2C_SCL
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_6);
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_7);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ,GPIO_PIN_7);

    //Init I2C0
    i2c_deinit(I2C0);
    rcu_periph_clock_enable(RCU_I2C0);
    i2c_clock_config(I2C0, 400000, I2C_DTCY_2);
    i2c_mode_addr_config(I2C0,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,0xA0);
    i2c_enable(I2C0);
    i2c_ack_config(I2C0,I2C_ACK_ENABLE);


}


uint8_t  mpu6050_CheckDevice(void)
{
    uint8_t i = 0;
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));

    i2c_start_on_bus(I2C0);
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    i2c_master_addressing(I2C0, 0x68<<1, I2C_TRANSMITTER);    
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    i = i2c_flag_get(I2C0, I2C_FLAG_ADDSEND);
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    return i;
}



uint8_t mpu6050_byte_write_timeout(uint8_t *p_buffer, uint8_t write_address)
{
    i2c_process_enum state = I2C_START;
    uint16_t timeout = 0;
    uint8_t i2c_timeout_flag = 0;

    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
    while (!i2c_timeout_flag)
    {   
        switch (state)
        {
        case I2C_START:
            /* i2c master sends start signal only when the bus is idle */
            while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT))
            {
                timeout ++;
            }
            if(timeout < I2C_TIME_OUT)
            {
                i2c_start_on_bus(I2C0);
                timeout = 0;
                state = I2C_SEND_ADDRESS;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf("i2c bus is busy in write byte\n");
            }
            break;

        case I2C_SEND_ADDRESS:
            /* i2c master sends START signal successfully */
            while((!i2c_flag_get(I2C0, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT))
            {
                timeout ++;
            }
            if(timeout < I2C_TIME_OUT)// i2c send slave address if the timeout value less than I2C_TIME_OUT
            {
                i2c_master_addressing(I2C0, MPU6050_add, I2C_TRANSMITTER); // send slave address
                state = I2C_CLEAR_ADDRESS_FLAG;
                timeout = 0;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends starts signal timeout in WRITE BYTE");
            }
            break;

        case I2C_CLEAR_ADDRESS_FLAG:
            /* address flag set means i2c slave sends ACK */
            while((!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT))
            {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT)
            {
                i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
                timeout = 0;
                state = I2C_TRANSMITTER_DATA;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf(" i2c master clears address flag timeout in WRITE BYTE ");
            }
        
        case I2C_TRANSMITTER_DATA:
            /* wait until the tansmit data buffer is empty */
            while((!i2c_flag_get(I2C0, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT))
            {
                timeout ++;
            }

            if(timeout < I2C_TIME_OUT)
            {
                /* send the MPU6050's internal address to write : only one byte address */
                i2c_data_transmit(I2C0, write_address); 
                timeout = 0;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf(" i2c master sends data timeout in WRITE BYTE ");
            }

            /* wait until BTC bit is set */
            while((!i2c_flag_get(I2C0, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT))
            {
                timeout ++;
            }

            if(timeout < I2C_TIME_OUT)
            {
                /* Send Data to write MPU6050 Register */
                i2c_data_transmit(I2C0, *p_buffer);
                timeout = 0;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf(" i2c master sends data timeout in WRITE BYTE ");
            }

            /* wait until BTC is set */
            while((!i2c_flag_get(I2C0, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT))
            {
                timeout++;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf(" i2c master sebds data timeout in WRITE BYTE ");
            }
            break;
        case I2C_STOP:
            /* send a stop signal to I2C bus */
            i2c_stop_on_bus(I2C0);
            /* i2c master sends STOP signal successfully */
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT))
            {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT)
            {
                timeout = 0;
                state = 
            }
        
        default:
            break;
        }
    }
    
}


#endif /*HARD_I2C0*/


#ifdef SOFT_I2C0

static void gpio_config(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);


    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
}

void I2C_Init(void)
{
    gpio_config();

}


void i2c_Delay(void)
{
    // uint8_t i;

    // for(i = 0;i < 1;i++);
    __NOP();
    __NOP();
}


void i2c_Start(void)
{
    i2c_SDA_1();
    i2c_SCL_1();
    i2c_Delay();
    i2c_SDA_0();
    i2c_Delay();
    i2c_SCL_0();
    i2c_Delay();

}

void i2c_Stop()
{
    i2c_SDA_0();
    i2c_SCL_1();
    i2c_Delay();
    i2c_SDA_1();
}


void i2c_SendByte(uint8_t _ucByte)
{
    uint8_t i;

    for(i = 0; i < 8 ; i++)
    {
        if(_ucByte & 0x80)
        {
            i2c_SDA_1();
        }
        else
        {
            i2c_SDA_0();
        }
        i2c_Delay();
        i2c_SCL_1();
        i2c_Delay();
        i2c_SCL_0();
        if(i == 7)
        {
            i2c_SDA_1();
        }
        _ucByte <<= 1;
        i2c_Delay();
    }
}

uint8_t i2c_ReadByte()
{
    uint8_t i;
    uint8_t value;

    value = 0;
    for(i =0; i < 8; i++)
    {
        value <<= 1;
        i2c_SCL_1();
        i2c_Delay();
        if(i2c_SDA_READ())
        {
            value++;
        }
        i2c_SCL_0();
        i2c_Delay();


    }

    return value;
}


uint8_t i2c_WaitACK(void)
{
    uint8_t re;

    i2c_SDA_1();
    i2c_Delay();

    i2c_SCL_1();
    i2c_Delay();

    if(i2c_SDA_READ())
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    i2c_SCL_0();
    i2c_Delay();
    return  re;
}


void i2c_Ack()
{
    i2c_SDA_0();
    i2c_Delay();

    i2c_SCL_1();
    i2c_Delay();
    i2c_SCL_0();
    i2c_Delay();

    i2c_SDA_1();
}


void i2c_NACK()
{
    i2c_SDA_1();
    i2c_Delay();

    i2c_SCL_1();
    i2c_Delay();
    i2c_SCL_0();
    i2c_Delay();
}


uint8_t mpu6050_CheckDevice(uint8_t _Address)
{

    uint8_t i;
    i2c_Start();

    i2c_SendByte(_Address);
    i = i2c_WaitACK();
    i2c_Stop();

    return i;

}

#endif

