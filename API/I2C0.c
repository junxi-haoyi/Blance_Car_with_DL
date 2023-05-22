#include "I2C0.h"


uint8_t mpu_data[50];
#ifdef HARD_I2C0

/**
 * @brief I2C初始化
 * 
 */
void I2C_Init(void)
{
    //Enable I2CX and GPIOB clock
    rcu_periph_clock_enable(RCU_GPIOB);


     //GPIO af set
    gpio_af_set(GPIOB,GPIO_AF_4,GPIO_PIN_6);
    gpio_af_set(GPIOB,GPIO_AF_4,GPIO_PIN_7);

    //GPIO_PIN_7 :I2C0_SDA  GPIOB_PIN_6 :I2C_SCL
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_6);
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_7);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ,GPIO_PIN_7);

    //Init I2CX
    i2c_deinit(I2CX);
    rcu_periph_clock_enable(RCU_I2C0);
    i2c_clock_config(I2CX, 100000, I2C_DTCY_2);
    i2c_mode_addr_config(I2CX,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,0xA0);
    i2c_enable(I2CX);
    i2c_ack_config(I2CX,I2C_ACK_ENABLE);


}


/**
 * @brief 检查MPU6050设备
 * 
 * @return uint8_t 
 */
uint8_t  mpu6050_CheckDevice(void)
{
    uint8_t i = 0;
    while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY));

    i2c_start_on_bus(I2CX);
    while(!i2c_flag_get(I2CX, I2C_FLAG_SBSEND));

    i2c_master_addressing(I2CX, 0x68<<1, I2C_TRANSMITTER);    
    while(!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND));
    i = i2c_flag_get(I2CX, I2C_FLAG_ADDSEND);
    i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
    return i;
}






/**
 * @brief 向寄存器写入数据
 * 
 * @param slave_address 
 * @param p_buffer 
 * @param write_address 
 * @param len 
 * @return uint8_t 
 */
uint8_t mpu6050_byte_write_timeout(uint8_t slave_address,uint8_t *p_buffer, uint8_t write_address, uint8_t len)
{

#if 0

    uint32_t result = 0;
    uint32_t i = 0;

    // wait for busy flag to be cleared
    while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY));

    // wait for the start bit to be set
    i2c_start_on_bus(I2CX);

    // wait for the start bit to be set
    while(!i2c_flag_get(I2CX, I2C_FLAG_SBSEND));

    // Transmit the slave address and enable writing operation
    i2c_master_addressing(I2CX, slave_address, I2C_TRANSMITTER);

    // wait for address bit to be set
    while(!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND));

    // clear the ADDR bit
    i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);

    // Transmit the first address for write 
    i2c_data_transmit(I2CX, write_address);

    for(i = 0; i<len ; i++)
    {
        while(!i2c_flag_get(I2CX, I2C_FLAG_TBE));

        i2c_data_transmit(I2CX, p_buffer[i]);
    }

    while(!i2c_flag_get(I2CX, I2C_FLAG_BTC));

    i2c_stop_on_bus(I2CX);

    return result;

#endif

#if 1
    i2c_process_enum state = I2C_START;
    uint16_t timeout = 0;
    uint8_t i2c_timeout_flag = 0;

    i2c_ack_config(I2CX, I2C_ACK_ENABLE);
    while (!i2c_timeout_flag)
    {   
        switch (state)
        {
        case I2C_START:
            /* i2c master sends start signal only when the bus is idle */
            while (i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT))
            {
                timeout ++;
            }
            if(timeout < I2C_TIME_OUT)
            {
                i2c_start_on_bus(I2CX);
                timeout = 0;
                state = I2C_SEND_ADDRESS;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf("i2c bus is busy in write byte t1 \n");
            }
            break;

        case I2C_SEND_ADDRESS:
            /* i2c master sends START signal successfully */
            while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT))
            {
                timeout ++;
            }
            if(timeout < I2C_TIME_OUT)// i2c send slave address if the timeout value less than I2C_TIME_OUT
            {
                i2c_master_addressing(I2CX, slave_address, I2C_TRANSMITTER); // send slave address
                state = I2C_CLEAR_ADDRESS_FLAG;
                timeout = 0;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends starts signal timeout in WRITE BYTE t2 \n");
            }
            break;

        case I2C_CLEAR_ADDRESS_FLAG:
            /* address flag set means i2c slave sends ACK */
            while((!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT))
            {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT)
            {
                i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                timeout = 0;
                state = I2C_TRANSMITTER_DATA;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf(" i2c master clears address flag timeout in WRITE BYTE t3 \n");
            }
        
        case I2C_TRANSMITTER_DATA:
            /* wait until the tansmit data buffer is empty */
            while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT))
            {
                timeout ++;
            }

            if(timeout < I2C_TIME_OUT)
            {
                /* send the MPU6050's internal address to write : only one byte address */
                i2c_data_transmit(I2CX, write_address); 
                timeout = 0;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf(" i2c master sends data timeout in WRITE BYTE t4 \n");
            }

            for(uint8_t i = 0; i < len ; i++)
            {
                /* wait until BTC bit is set */
                while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT))
                {
                    timeout ++;
                }

                if(timeout < I2C_TIME_OUT)
                {
                    /* Send Data to write MPU6050 Register */
                    while(!i2c_flag_get(I2CX, I2C_FLAG_TBE));
                    i2c_data_transmit(I2CX, p_buffer[i]);
                    timeout = 0;
                }
                else
                {
                    timeout = 0;
                    state = I2C_START;
                    printf(" i2c master sends data timeout in WRITE BYTE t5 \n");
                }
            }

            // /* wait until BTC bit is set */
            // while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT))
            // {
            //     timeout ++;
            // }

            // if(timeout < I2C_TIME_OUT)
            // {
            //     /* Send Data to write MPU6050 Register */
            //     i2c_data_transmit(I2CX, *p_buffer);
            //     timeout = 0;
            // }
            // else
            // {
            //     timeout = 0;
            //     state = I2C_START;
            //     printf(" i2c master sends data timeout in WRITE BYTE t5 \n");
            // }

            /* wait until BTC is set */
            while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT))
            {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT)
            {
                state = I2C_STOP;
                timeout = 0;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf(" i2c master sebds data timeout in WRITE BYTE t6 \n");
            }
            break;
        case I2C_STOP:
            /* send a stop signal to I2C bus */
            i2c_stop_on_bus(I2CX);
            /* i2c master sends STOP signal successfully */
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT))
            {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT)
            {
                timeout = 0;
                state = I2C_STOP;
                i2c_timeout_flag = I2C_OK;
            }
            else
            {
                timeout = 0;
                state = I2C_START;
                printf(" i2c master sends stop signal timeout in WRITE BYTE t7 \n");
            }
            break;
        default:
            state = I2C_START;
            i2c_timeout_flag = I2C_OK;
            timeout = 0;
            printf(" i2c master sends start signal in WRITE BYTE. t8 \n");
            break;
        }
    }

    return I2C_END;

#endif

}


/**
 * @brief Read Register Data
 * 
 * @param slave_address 
 * @param reg_address 
 * @param data_len 
 * @param data_save 
 * @return uint32_t 
 */
uint32_t mpu6050_byte_read_timeout(uint8_t slave_address, uint8_t read_address, uint8_t data_len, uint8_t *data_save)
{
    // wait until I2C bus is idle
    while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY));
    // send a start signal to I2C bus
    i2c_start_on_bus(I2CX);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2CX, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2CX, slave_address, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND));

    // clear ADDSEND bit 
    i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
    // wait until transmit data buffer is empty
    while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)));
    // transmit the register address to be read
    i2c_data_transmit(I2CX, read_address);
    // wait until transmit data buffer is empty
    while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)));
    
    // send START signal a second time
    i2c_start_on_bus(I2CX);
    // wait until SBSEND bit is set
    while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)));
    i2c_master_addressing(I2CX, slave_address, I2C_RECEIVER);
    // wait until ADDSEND bit is set

    if(data_len == 1)
    {
        // reset ACKEN bit before clearing ADDESEND bit
        i2c_ack_config(I2CX, I2C_ACK_DISABLE);
        // clear ADDSEND bit
        i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
        // send stop signal after clearing ADDSEND bit
        i2c_stop_on_bus(I2CX);
        // wait until the RBNE bit is set
        while(!i2c_flag_get(I2CX, I2C_FLAG_RBNE));
        // read a data from I2C_DATA
        data_save[0] = i2c_data_receive(I2CX);

    }
    else if(data_len == 2)
    {
        // reset ACKEN bit before clearing ADDSEND bit
        i2c_ack_config(I2CX, I2C_ACK_DISABLE);
        // Set POAP bit
        I2C_CTL0(I2CX) |= I2C_CTL0_POAP;
        // clear ADDSEND bit
        i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
        // wait until the BTC bit is set
        while(!i2c_flag_get(I2CX, I2C_FLAG_BTC));
        // send STOP signal
        i2c_stop_on_bus(I2CX);

        // read 2 byte
        data_save[0] = i2c_data_receive(I2CX);
        data_save[1] = i2c_data_receive(I2CX);
    }
    else if( data_len == 3)
    {
        // clear ADDSEND bit
        i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
        // wait until the  Byte Transmit Complish bit is set
        while(!i2c_flag_get(I2CX, I2C_FLAG_BTC));
        // reset ACKEN bit before clearing ADDSEND bit
        i2c_ack_config(I2CX, I2C_ACK_DISABLE);

        data_save[0] = i2c_data_receive(I2CX);
        // send STOP signal
        i2c_stop_on_bus(I2CX);

        data_save[1] = i2c_data_receive(I2CX);
        // wait until the RBNE bit is set
        while(!i2c_flag_get(I2CX, I2C_FLAG_RBNE));

        data_save[2] = i2c_data_receive(I2CX);

    }



    return 1;

#if 0
    uint8_t state = I2C_START;
    uint16_t timeout = 0;
    uint8_t i2c_timeout_flag = 0;

    while(!(i2c_timeout_flag))
    {
        switch(state)
        {
            case I2C_START:
                /* i2c master sends start signal only when the bus is idle */
                while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT))
                {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT)
                {
                    i2c_start_on_bus(I2CX);
                    timeout = 0;
                    state = I2C_SEND_ADDRESS;
                }
                else
                {
                    timeout = 0;
                    state = I2C_START;
                    printf(" i2c bus is busy in WRITE BYTE! r1 \n ");
                }
                break;

            case I2C_SEND_ADDRESS:
                /* i2c master sends START signal successfully */
                while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND))  && (timeout < I2C_TIME_OUT))
                {
                    timeout++;
                }

                if(timeout < I2C_TIME_OUT)
                {
                    timeout = 0;
                    state = I2C_CLEAR_ADDRESS_FLAG;
                    i2c_master_addressing(I2CX, slave_address, I2C_TRANSMITTER);
                }
                else
                {
                    timeout = 0;
                    state = I2C_START;
                    printf(" i2c master sends starts signal timeout in WRITE BYTE r2 \n ");
                }
                break;

            case I2C_CLEAR_ADDRESS_FLAG:
                while((!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT))
                {
                    timeout++;
                }

                if(timeout < I2C_TIME_OUT)
                {
                    i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                    timeout = 0;
                    state = I2C_TRANSMITTER_DATA;
                }
                else
                {
                    timeout = 0;
                    state = I2C_START;
                    printf(" i2c master clear flag timeout in WRITE BYTE! r3 \n ");
                }
                break;

            case I2C_TRANSMITTER_DATA:
                /* wait until the transmit data buffer is empty */
                while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT))
                {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT)
                {
                    timeout = 0;
                    i2c_data_transmit(I2CX, read_address);
                    while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT))
                    {
                        timeout++;
                    }
                    if(timeout < I2C_TIME_OUT)
                    {
                        i2c_stop_on_bus(I2CX);
                        timeout = 0;
                        state = I2C_REV_START;
                    }
                    else
                    {
                        timeout = 0;
                        state = I2C_START;
                        printf(" i2c bus send stop signal timeout r4 \n");
                    }
                }
                else
                {
                    timeout = 0;
                    state = I2C_START;
                    printf(" i2c master sends data timeout in WRITE BYTE! \n");
                }
            break;

            case I2C_REV_START:
                /*i2c master sends start signal only when the bus id idle */
                while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT))
                {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT)
                {
                    i2c_start_on_bus(I2CX);
                    timeout = 0;
                    state = I2C_REV_SEND_ADDRESS;
                }
                else
                {
                    timeout = 0;
                    state = I2C_START;
                    printf(" i2c get busy on start r5 \n ");
                }
                break;

            case I2C_REV_SEND_ADDRESS:
                /* i2c master sends START signal successfully */
                while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT))
                {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT)
                {
                    timeout = 0;
                    i2c_master_addressing(I2CX, slave_address, I2C_RECEIVER);
                    state = I2C_REV_DATA;
                }
                else
                {
                    timeout = 0;
                    state = I2C_START;
                    printf(" i2c sends start signal timeout r6 \n ");
                }
            break;

            case I2C_REV_DATA:
                /* wait until ADDSEND bit is set */
                while((!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT))
                {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT)
                {
                    i2c_ack_config(I2CX, I2C_ACK_DISABLE);
                    i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                    i2c_stop_on_bus(I2CX);
                    while(!i2c_flag_get(I2CX, I2C_FLAG_RBNE));
                    data_save[0] = i2c_data_receive(I2CX);
                    while(I2C_CTL0(I2CX) & I2C_CTL0_STOP);
                    i2c_ack_config(I2CX, I2C_ACK_ENABLE);

                    timeout = 0;
                    state = I2C_START;
                    i2c_timeout_flag = 1;
                }
            default:
                state = I2C_START;
                i2c_timeout_flag = 0;
                timeout = 0;
        }
    }
#endif

}





/**
 * @brief       往ATK-MS6050的指定寄存器连续写入指定数据
 * @param       addr: ATK-MS6050的IIC通讯地址
 *              reg : ATK-MS6050寄存器地址
 *              len : 写入的长度
 *              dat : 写入的数据
 * @retval      ATK_MS6050_EOK : 函数执行成功
 *              ATK_MS6050_EACK: IIC通讯ACK错误，函数执行失败
 */
uint8_t dmp_mpu6050_write(uint8_t addr,uint8_t reg, uint8_t len, uint8_t *dat)
{
    mpu6050_byte_write_timeout(addr,dat, reg, len);
    return 0;
}



/**
 * @brief       连续读取ATK-MS6050指定寄存器的值
 * @param       addr: ATK-MS6050的IIC通讯地址
 *              reg : ATK-MS6050寄存器地址
 *              len: 读取的长度
 *              dat: 存放读取到的数据的地址
 * @retval      ATK_MS6050_EOK : 函数执行成功
 *              ATK_MS6050_EACK: IIC通讯ACK错误，函数执行失败
 */
uint8_t dmp_mpu6050_read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *dat)
{
    mpu6050_byte_read_timeout(addr, reg, len, dat);
    return 0;
}


/******************************  SOFT I2C    *************************************/


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
    __NOP();__NOP();__NOP();__NOP();__NOP();
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


/**
 * @brief 向寄存器写入数据
 * 
 * @param addr 
 * @param reg 
 * @param len 
 * @param data 
 * @return uint8_t 
 */
uint8_t i2c_write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    uint8_t i;

    i2c_Start();
    i2c_SendByte((addr << 1) | 0);
    if(i2c_WaitACK() == 1)
    {
        i2c_Stop();
        printf("IIC Send Slave Address error\n");
        return MS6050_EACK;
    }

    i2c_SendByte(reg);
    if(i2c_WaitACK() == 1)
    {
        i2c_Stop();
        printf("IIC Send Reg Address error\n");
        return MS6050_EACK;
    }

    for(i = 0; i<len ; i++)
    {
        i2c_SendByte(data[i]);
        if(i2c_WaitACK() == 1)
        {
            i2c_Stop();
            printf("IIC Send Data error in i = %d", i);
        }
    }

    i2c_Stop();
    return MS6050_EOK;

}


/**
 * @brief 读取MPU6050数据
 * 
 * @param addr 
 * @param reg 
 * @param len 
 * @param data 
 * @return uint8_t 
 */
uint8_t i2c_read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    i2c_Start();
    i2c_SendByte((addr << 1) | 0);
    if(i2c_WaitACK() == 1)
    {
        i2c_Stop();
        printf("IIC Send Slave Address error\n");
        return MS6050_EACK;
    }

    i2c_SendByte(reg);
    if(i2c_WaitACK() == 1)
    {
        i2c_Stop();
        printf("IIC Send Reg Address error\n");
        return MS6050_EACK;
    }

    i2c_Start();
    i2c_SendByte((addr << 1) | 1);
    if(i2c_WaitACK() == 1)
    {
        i2c_Stop();
        printf("IIC Send Slave Address error\n");
        return MS6050_EACK;
    }

    while ((len > 1) ? 1 : 0)
    {
        *data = i2c_ReadByte();
        len--;
        data++;
    }

    i2c_Stop();
    return MS6050_EOK;
    
    


}



#endif


/****************************  END SOFT I2C   ***********************************/

