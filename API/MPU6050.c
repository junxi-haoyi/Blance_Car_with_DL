#include "MPU6050.h"


/**
 * @brief ��Ĵ���д���ض�ֵ Write Special Data on Register
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
    // �ϵ���ʱ����ֹ���ݳ��ִ���
    int i = 0,j = 0;
    for(i = 0; i<1000; i++)
    {
        for(j = 0; j<1000; j++)
        {
            ;
        }
    }

    MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x01);      // �������״̬, ʹ��x����������Ϊ�ο�
    MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV, 0x07);      // �����ǲ�����
    MPU6050_WriteReg(MPU6050_RA_CONFIG, 0x06);
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG, 0x00);    // ���ü��ٶȴ�������4gģʽ
    MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     // �������Լ켰������Χ������ֵ: 0x18(���Լ죬 ��2000deg/s)
    MPU6050_WriteReg(MPU_PWR_MGMT2_REG, 0x00);          // ���ٶȺ������Ƕ���ʼ����
}




/**
 * @brief Read MPU6050 ID ��ȡ MPU6050 ID
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








