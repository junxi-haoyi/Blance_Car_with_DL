#ifndef __I2C0__
#define __I2C0__

#include "gd32f4xx.h"
#include <stdio.h>

//#define HARD_I2C0
#define SOFT_I2C0


/*******************************************************************************************************
 *                          HARD I2C0
 ******************************************************************************************************/
#ifdef HARD_I2C0

void I2C_Init(void);
uint8_t  mpu6050_CheckDevice(void);
#endif /*HARD_I2C0*/
/*******************************************************************************************************
 *                          HARD I2C0
 ******************************************************************************************************/




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