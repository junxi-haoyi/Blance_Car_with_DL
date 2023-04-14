#include "I2C0.h"


#ifdef HARD_I2C0

void i2c_Delay(void)
{
    uint8_t i;

    for(i = 0;i < 10;i++);
}

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
    i2c_clock_config(I2C0,100000,I2C_DTCY_2);
    i2c_mode_addr_config(I2C0,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,0x68<<1);
    i2c_enable(I2C0);
    i2c_ack_config(I2C0,I2C_ACK_ENABLE);


}


uint8_t  mpu6050_CheckDevice(void)
{

    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));

    i2c_start_on_bus(I2C0);
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    printf("start OK\n");

    i2c_Delay();

    i2c_master_addressing(I2C0, 0x68<<1, I2C_TRANSMITTER);
    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
    printf("add OK\n");

    return 0;
    // uint8_t i;
    // i2c_ack_config(I2C0,I2C_ACK_ENABLE);
    // i2c_start_on_bus(I2C0);
    // while(!i2c_flag_get(I2C0,I2C_FLAG_SBSEND));
    // i2c_master_addressing(I2C0,0x68,I2C_TRANSMITTER);
    // i2c_Delay();
    // i = i2c_flag_get(I2C0,I2C_FLAG_ADDSEND);
    // i2c_stop_on_bus(I2C0);
    // return !i;
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
    uint8_t i;

    for(i = 0;i < 10;i++);
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

    i2c_SendByte(_Address<<1);
    i = i2c_WaitACK();
    i2c_Stop();

    return i;



}



#endif

