#include "MPU6050.h"


/**
 * @brief 向寄存器写入特定值 Write Special Data on Register
 * 
 * @param reg_add 
 * @param reg_dat 
 */
void MPU6050_WriteReg(uint8_t reg_add, uint8_t reg_dat)
{
    mpu6050_byte_write_timeout(MPU6050_add, &reg_dat, reg_add, 1);

}


/**
 * @brief Read MPU6050 Register Data
 * 
 * @param reg_add 
 * @param data_buffer 
 * @param num 
 */
void mpu6050_ReadData(uint8_t reg_add, uint8_t *data_buffer, uint8_t num)
{
    mpu6050_byte_read_timeout(MPU6050_add, reg_add, num, data_buffer); 
}



/**
 * @brief 
 * 
 */
void MPU_6050_Init(void)
{   
    // 上电延时，防止数据出现错误
    int i = 0,j = 0;
    for(i = 0; i<1000; i++)
    {
        for(j = 0; j<1000; j++)
        {
            ;
        }
    }

    MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x01);      // 解除休眠状态, 使用x轴陀螺仪作为参考
    MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV, 0x07);      // 陀螺仪采样率
    MPU6050_WriteReg(MPU6050_RA_CONFIG, 0x06);
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG, 0x00);    // 配置加速度传感器在4g模式
    MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     // 陀螺仪自检及测量范围，典型值: 0x18(不自检， ±2000deg/s)
    MPU6050_WriteReg(MPU_PWR_MGMT2_REG, 0x00);          // 加速度和陀螺仪都开始工作
}




/**
 * @brief Read MPU6050 ID 读取 MPU6050 ID
 * 
 * @return uint8_t 
 */
uint8_t mpu6050_ReadID(void)
{
    uint8_t Re = 0;
    mpu6050_ReadData(MPU6050_RA_WHO_AM_I, &Re, 1);
    if(Re != 0x68)
    {
        printf("MPU6050 detected error!\n");
        return 0;
    }
    else
    {
        printf("MPU6050 ID = 0x%X\n", Re);
        return 1;
    }
}








