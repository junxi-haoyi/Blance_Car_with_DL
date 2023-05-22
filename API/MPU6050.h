#ifndef __MPU6050__
#define __MPU6050__

#include "gd32f4xx.h"
#include "I2C0.h"
#include "USART2.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


#define Battery_manage  0x6B    // 电池管理寄存器
#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_SMPLRT_DIV       0x19
#define MPU6050_RA_CONFIG           0x1A
#define MPU6050_RA_ACCEL_CONFIG     0x1C
#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_WHO_AM_I         0x75
#define MPU_ACCEL_CFG_REG           0X1C    // 加速度计配置寄存器
#define MPU_PWR_MGMT2_REG           0X6C    // 电源管理寄存器2 
#define DEFAULT_MPU_HZ  (200)
static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};


void MPU_6050_Init(void);
void mpu6050_ReadData(uint8_t reg_add, uint8_t *data_buffer, uint8_t num);
uint8_t mpu6050_ReadID(void);
void DMP_Init(void);

#endif /*MPU6050*/
