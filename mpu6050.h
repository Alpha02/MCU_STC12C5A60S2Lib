#ifndef _MPU6050_H
	#define _MPU6050_H
	#include "comm.h"
	#include "I2C.h"
	#define IDD_ACCEL_X	0
	#define IDD_ACCEL_Y	1
	#define IDD_ACCEL_Z	2
	#define IDD_GYRO_X	3
	#define IDD_GYRO_Y	4
	#define IDD_GYRO_Z	5

extern void normalization_3(float * x,float * y,float * z);
extern void normalization_4(float * x,float * y,float * z,float *w);
extern void MPU6050_Init();
extern void MPU6050_Multiple_Read();
extern int MPU6050_getRaw(uchar dat_idx);

extern int MPU6050_get(uchar dat_idx);
extern float MPU6050_getReal(uchar dat_idx);
extern void IMUUpdate(float * roll,float * pitch,float * yaw);						   

#define MPU6050_ACCEL_RangeSet	1
/*
1:	+-2g
2:	+-4g
3:	+-8g
4:	+-16g
*/
#define MPU6050_GYRO_RangeSet	3
/*
1:	+-250deg/s
2:	+-500deg/s
3:	+-1000deg/s
4:	+-2000deg/s
*/
//*********************************
//陀螺仪自检及测量范围
//*********************************
//典型值：0x18(不自检，2000deg/s)

#if MPU6050_GYRO_RangeSet == 1
	#define MPU_60X0_GYRO_CONFIG_REG_VALU       0x00
#endif

#if MPU6050_GYRO_RangeSet == 2
	#define MPU_60X0_GYRO_CONFIG_REG_VALU       0x08 
#endif

#if MPU6050_GYRO_RangeSet == 3
	#define MPU_60X0_GYRO_CONFIG_REG_VALU       0x10
#endif

#if MPU6050_GYRO_RangeSet == 4
	#define MPU_60X0_GYRO_CONFIG_REG_VALU       0x18   
#endif
//*********************************
//加速计自检、测量范围及高通滤波频率
//*********************************
//典型值：0x01(不自检，2G，5Hz)
#if MPU6050_ACCEL_RangeSet == 1
	#define MPU_60X0_ACCEL_CONFIG_REG_VALU      0x00 
#endif

#if MPU6050_ACCEL_RangeSet == 2
	#define MPU_60X0_ACCEL_CONFIG_REG_VALU      0x08 
#endif

#if MPU6050_ACCEL_RangeSet == 3
	#define MPU_60X0_ACCEL_CONFIG_REG_VALU      0x10 
#endif

#if MPU6050_ACCEL_RangeSet == 4
	#define MPU_60X0_ACCEL_CONFIG_REG_VALU      0x18 
#endif
//*********************************
//数据通道
//********************************* 
#define ACCEL_XOUT_H    0x3B  
#define ACCEL_XOUT_L    0x3C  
#define ACCEL_YOUT_H    0x3D  
#define ACCEL_YOUT_L    0x3E  
#define ACCEL_ZOUT_H    0x3F  
#define ACCEL_ZOUT_L    0x40  
#define TEMP_OUT_H      0x41  
#define TEMP_OUT_L      0x42  
#define GYRO_XOUT_H     0x43  
#define GYRO_XOUT_L     0x44      
#define GYRO_YOUT_H     0x45  
#define GYRO_YOUT_L     0x46  
#define GYRO_ZOUT_H     0x47  
#define GYRO_ZOUT_L     0x48  

#define PWR_MGMT_1      0x6B    //电源管理，典型值：0x00(正常启用)  
#define WHO_AM_I        0x75    //I2C地址寄存器(默认数值0x68，只读)  
#define SlaveAddress    0xD0    //I2C写入时的地址字节数据，+1为读取  
                      

#define MPU_60X0_PWR_MGMT_1_REG_ADDR        0x6B  
#define MPU_60X0_USER_CTRL_REG_ADDR         0x6A  
#define MPU_60X0_SMPLRT_DIV_REG_ADDR        0x19  
#define MPU_60X0_CONFIG_REG_ADDR            0x1A  
#define MPU_60X0_GYRO_CONFIG_REG_ADDR       0x1B  
#define MPU_60X0_ACCEL_CONFIG_REG_ADDR      0x1C  
#define MPU_60X0_FIFO_EN_REG_ADDR           0x23  
  
#define MPU_60X0_RESET_REG_VALU             0x80  
#define MPU_60X0_PWR_MGMT_1_REG_VALU        0x09    // Disable temperature sensor, PLL with X axis gyroscope reference  
#define MPU_60X0_USER_CTRL_REG_VALU         0x45    // Enable FIFO. Reset FIFO and signal paths for all sensors  
#define MPU_60X0_SMPLRT_DIV_REG_VALU        0x00    // DLPF_CFG is 0x01, so Gyroscope Output Rate = 1kHz, divided by 1, still 1kHz  
#define MPU_60X0_CONFIG_REG_VALU            0x03    // 184Hz 2.0ms 188Hz 1.9ms 1kHz. So there will be 6x2 bytes new data in FIFO every 1ms  

#define MPU_60X0_FIFO_EN_REG_VALU           0x78    // Only enable accel and gyro   

#endif