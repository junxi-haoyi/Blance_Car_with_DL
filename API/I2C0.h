#ifndef __I2C0__
#define __I2C0__

#include "gd32f4xx.h"
#include <stdio.h>
#include "USART2.h"

//#define SOFT_I2C0
#define HARD_I2C0

/*************************************** MPU Data ********************************************************/
extern uint8_t mpu_data[50];

/*********************************************************************************************************/



/*************************************** HARD I2C0 ********************************************************/
#ifdef HARD_I2C0

typedef enum{
    I2C_START = 0,
    I2C_SEND_ADDRESS,
    I2C_CLEAR_ADDRESS_FLAG,
    I2C_TRANSMITTER_DATA,
    I2C_RECEIVER_DATA,
    I2C_STOP,

    I2C_REV_START,
    I2C_REV_SEND_ADDRESS,
    I2C_REV_DATA,
    I2C_REV_CLEAR_ADDRESS_FLAG
}i2c_process_enum;

#define I2C_END     1
#define I2C_FAIL    0
#define I2C_OK      1

#define I2CX            I2C0
#define I2C_TIME_OUT    5000
#define MPU6050_add     0x68<<1 // mpu6050???¡¤

void I2C_Init(void);
uint8_t  mpu6050_CheckDevice(void);
uint8_t mpu6050_byte_write_timeout(uint8_t slave_address,uint8_t *p_buffer, uint8_t write_address, uint8_t len);
uint32_t mpu6050_byte_read_timeout(uint8_t slave_address, uint8_t read_address, uint8_t data_len, uint8_t *data_save);


uint8_t dmp_mpu6050_write(uint8_t addr,uint8_t reg, uint8_t len, uint8_t *dat);
uint8_t dmp_mpu6050_read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *dat);


#endif /*HARD_I2C0*/
/*******************************************************************************************************/
                           





/*******************************************************************************************************
 *                          SOFT I2C0
 ******************************************************************************************************/
#ifdef SOFT_I2C0

#define MPU6050_add 0x68

#define i2c_SCL_0()     gpio_bit_reset(GPIOB,GPIO_PIN_6)
#define i2c_SCL_1()     gpio_bit_set(GPIOB,GPIO_PIN_6)
#define i2c_SDA_0()     gpio_bit_reset(GPIOB,GPIO_PIN_7)
#define i2c_SDA_1()     gpio_bit_set(GPIOB,GPIO_PIN_7)

#define i2c_SCL_READ()  gpio_input_bit_get(GPIOB,GPIO_PIN_6)
#define i2c_SDA_READ()  gpio_input_bit_get(GPIOB,GPIO_PIN_7)

void I2C_Init(void);
void i2c_Delay(void);
void i2c_Start(void);
void i2c_Stop();
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte();
uint8_t i2c_WaitACK(void);
void i2c_Ack();
void i2c_NACK();
uint8_t mpu6050_CheckDevice(uint8_t _Address);
/*******************************************************************************************************
 *                          SOFT I2C0 END
 ******************************************************************************************************/
#endif /*SOFT_I2C0*/



#endif 