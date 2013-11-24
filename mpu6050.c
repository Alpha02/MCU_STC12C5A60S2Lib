#include "mpu6050.h"
#include "serial.h"
#include <math.h>
uchar xdata MPU6050_Record[14];			//读取的数据存储于此。
ulong xdata MPU6050_offset[6];
/*
如果要读取MPU-60X0 寄存器的值，
（1）首先由主设备产生开始信号（S）。
（2）然后发送从设备地址位和一个写数据位。
（3）然后发送寄存器地址，才能开始读寄存器。
（4）紧接着，收到应答信号后，主设备再发一个开始信号。
（5）然后发送从设备地址位和一个读数据位。
（6）然后，作为从设备的MPU-60X0 产生应答信号并开始发送寄存器数据。
（*）通信以主设备产生的拒绝应答信号（NACK）和结束标志（P）结束。
（*）拒绝应答信号（NACK）产生定义为SDA数据在第9个时钟周期一直为高。
*/
/*单字节写*/
void Single_Write_MPU6050(uchar Address,uchar Dat)
{
    I2C_Start();
    I2C_Send_Byte(SlaveAddress);
    I2C_Send_Byte(Address);
    I2C_Send_Byte(Dat);
    I2C_Stop();
}
//单字节读MPU6050
/*
uchar Single_Read_MPU6050(uchar Addr)
{
    uchar Value;
    IIC_Start();//（1）首先由主设备产生开始信号（S）
	
    IIC_Send_Byte(SlaveAddress);//（2）然后发送从设备地址位和一个写数据位。（合起来就是SlaveAddress）

   IIC_Send_Byte(Addr); //发送存储单元地址
	
    IIC_Start();	 //（4）紧接着，收到应答信号后，主设备再发一个开始信号。
	
    IIC_Send_Byte(SlaveAddress+1);	// （5）然后发送从设备地址位和一个读数据位。（合起来就是SlaveAddress+1）
    
	Value=IIC_Rec_Byte(); //读取地址数据，存储在Value
    IIC_SendAck(1);		  //回NOACK
    IIC_Stop();
	
//	show_number((int)(Value));
	delay(10);	
    return Value;
}	
*/
//多字节读HMC5833
void MPU6050_Multiple_Read()
{
    uchar i;  //连续读出HMC5883内部角度数据，地址范围0x3~0x5
    I2C_Start();		//（1）首先由主设备产生开始信号（S）
    I2C_Send_Byte(SlaveAddress); //（2）然后发送从设备地址位和一个写数据位。（合起来就是SlaveAddress）
    I2C_Send_Byte(0x3B);//发送存储单元地址，从0x3B开始 
    I2C_Start();		  //（4）紧接着，收到应答信号后，主设备再发一个开始信号。
    I2C_Send_Byte(SlaveAddress+1);	// （5）然后发送从设备地址位和一个读数据位。（合起来就是SlaveAddress+1）
    for(i=0;i<14;i++) //连续读取6个地址数据，存储在Rec_Data
    {
        MPU6050_Record[i]=I2C_Rec_Byte();
        if(i==13)
            I2C_SendAck(1); //最后一个数据需要回NOACK
        else
            I2C_SendAck(0); //回应ACK
    }
    I2C_Stop();
}

//**************************************  
//初始化MPU6050  
//**************************************  
void MPU6050_Init()  
{  
	uint i,id;
    Single_Write_MPU6050(MPU_60X0_PWR_MGMT_1_REG_ADDR, MPU_60X0_PWR_MGMT_1_REG_VALU);  
    Single_Write_MPU6050(MPU_60X0_USER_CTRL_REG_ADDR, MPU_60X0_USER_CTRL_REG_VALU);  
    Single_Write_MPU6050(MPU_60X0_SMPLRT_DIV_REG_ADDR, MPU_60X0_SMPLRT_DIV_REG_VALU);  
    Single_Write_MPU6050(MPU_60X0_CONFIG_REG_ADDR, MPU_60X0_CONFIG_REG_VALU);  
    Single_Write_MPU6050(MPU_60X0_GYRO_CONFIG_REG_ADDR, MPU_60X0_GYRO_CONFIG_REG_VALU);  
    Single_Write_MPU6050(MPU_60X0_ACCEL_CONFIG_REG_ADDR, MPU_60X0_ACCEL_CONFIG_REG_VALU);    
    Single_Write_MPU6050(MPU_60X0_FIFO_EN_REG_ADDR, MPU_60X0_FIFO_EN_REG_VALU);  
	send_ln();
	send_str("MPU6050_Init");
	for(id=0;id<6;id++){
			MPU6050_offset[id]=0;
		}
		delay(200);
  	for(i=0;i<100;i++){
		MPU6050_Multiple_Read();
		for(id=3;id<6;id++){
			MPU6050_offset[id]+=MPU6050_getRaw(id);
			//计算零点偏移
		}	
		delay(10);
		if(i%10==0)send('.');

	}
	for(id=0;id<6;id++){
		MPU6050_offset[id]=MPU6050_offset[id]/100;
	}
	for(id=0;id<6;id++){
		_send_value(MPU6050_offset[id]);
	}
	send_str("Complete!");
	send_ln();
	delay(100);
}
int dataRead(int s){
	if(s>127){return s-256;}//将255视为-1

	return s;
}
//**********************************
//
//***********************************
int MPU6050_getRaw(uchar dat_idx){
	int tmp;
	switch(dat_idx){
		case IDD_ACCEL_X:tmp=(((int)MPU6050_Record[0]<<8)+(int)MPU6050_Record[1]);break;		
		case IDD_ACCEL_Y:tmp=(((int)MPU6050_Record[2]<<8)+(int)MPU6050_Record[3]);break;
		case IDD_ACCEL_Z:tmp=(((int)MPU6050_Record[4]<<8)+(int)MPU6050_Record[5]);break;
		case IDD_GYRO_X:tmp=(((int)MPU6050_Record[8]<<8)+(int)MPU6050_Record[9]);break;
		case IDD_GYRO_Y:tmp=(((int)MPU6050_Record[10]<<8)+(int)MPU6050_Record[11]);break;
		case IDD_GYRO_Z:tmp=(((int)MPU6050_Record[12]<<8)+(int)MPU6050_Record[13]);break;
	}
	return tmp;
}
//**********************************
//获得整形数据
//***********************************
int MPU6050_get(uchar dat_idx){
	int tmp;
	switch(dat_idx){
		case IDD_ACCEL_X:tmp=(((int)MPU6050_Record[0]<<8)+(int)MPU6050_Record[1]-MPU6050_offset[0]);break;		
		case IDD_ACCEL_Y:tmp=(((int)MPU6050_Record[2]<<8)+(int)MPU6050_Record[3]-MPU6050_offset[1]);break;
		case IDD_ACCEL_Z:tmp=(((int)MPU6050_Record[4]<<8)+(int)MPU6050_Record[5]-MPU6050_offset[2]);break;
		case IDD_GYRO_X:tmp=(((int)MPU6050_Record[8]<<8)+(int)MPU6050_Record[9]-MPU6050_offset[3]);break;
		case IDD_GYRO_Y:tmp=(((int)MPU6050_Record[10]<<8)+(int)MPU6050_Record[11]-MPU6050_offset[4]);break;
		case IDD_GYRO_Z:tmp=(((int)MPU6050_Record[12]<<8)+(int)MPU6050_Record[13]-MPU6050_offset[5]);break;
	}
	return tmp;
}
//*******************************************************
//获得浮点型真实数据。单位分别是reg/s和g.
//******************************************************
float MPU6050_getReal(uchar dat_idx){
	float tmp;
	if(dat_idx<3){
		tmp=(float)MPU6050_get(dat_idx)*2*MPU6050_ACCEL_RangeSet/32768;		
	}
	else{	
		tmp=(float)MPU6050_get(dat_idx)*250*MPU6050_GYRO_RangeSet/32768;
	}
   	return tmp;
}
//**********************************
//三维向量归一化函数
//***********************************
void normalization_3(float * x,float * y,float * z){
	float m=sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z));
	(*x)/=m;
	(*y)/=m;
	(*z)/=m;	 
}
//**********************************
//四维向量归一化函数
//***********************************
void normalization_4(float * x,float * y,float * z,float *w){
	float m=sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z)+(*w)*(*w));
	(*w)/=m;
	(*x)/=m;
	(*y)/=m;
	(*z)/=m;	 
}
//**********************************
//四元数融合+PID滤波
//***********************************

#define PRECISE_GYRO
#ifdef PRECISE_GYRO
	float xdata IMU_q0 = 1, IMU_q1 = 0, IMU_q2 = 0, IMU_q3 = 0;    // quaternion elements representing the estimated orientation
	float xdata gx=0,gy=0,gz=0,exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
	float xdata ex=0,ey=0,ez=0;
	void IMUUpdate(float * roll,float * pitch,float * yaw){
		float xdata wx,wy,wz,ax,ay,az,vx,vy,vz;
		#define IMU_T 0.001f;		
		MPU6050_Multiple_Read();
		ax=MPU6050_getReal(IDD_ACCEL_X);
		ay=MPU6050_getReal(IDD_ACCEL_Y);
		az=MPU6050_getReal(IDD_ACCEL_Z);
		wx=MPU6050_getReal(IDD_GYRO_X);
		wy=MPU6050_getReal(IDD_GYRO_Y);
		wz=MPU6050_getReal(IDD_GYRO_Z);
		/*
		send_float(ax,3);
		send_tab();	
		send_float(ay,3);
		send_tab();
		send_float(az,3);
		send_tab();
		send_float(wx,3);
		send_tab();
		send_float(wy,3);
		send_tab();
		send_float(wz,3);
		send_ln();
		*/
		#define Kp 40.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
		#define Ki 0.01f                          // integral gain governs rate of convergence of gyroscope biases
		wx = wx + Kp*ex + exInt;					   							//将误差PI后补偿到陀螺仪，即补偿零点漂移
		wy = wy + Kp*ey + eyInt;
		wz = wz + Kp*ez + ezInt;	
		IMU_q0 = IMU_q0 + (-IMU_q1*wx - IMU_q2*wy - IMU_q3*wz)*IMU_T;
		IMU_q1 = IMU_q1 + (IMU_q0*wx + IMU_q2*wz - IMU_q3*wy)*IMU_T;
		IMU_q2 = IMU_q2 + (IMU_q0*wy - IMU_q1*wz + IMU_q3*wx)*IMU_T;
		IMU_q3 = IMU_q3 + (IMU_q0*wz + IMU_q1*wy - IMU_q2*wx)*IMU_T;
		//计算出四元数。这里使用的是四元数的旋转微分方程。	
		vx = 2*(IMU_q1*IMU_q3 - IMU_q0*IMU_q2);												//四元数中xyz的表示
		vy = 2*(IMU_q0*IMU_q1 + IMU_q2*IMU_q3);
		vz = IMU_q0*IMU_q0 - IMU_q1*IMU_q1 - IMU_q2*IMU_q2 + IMU_q3*IMU_q3;
		//根据四元数推算出姿态。当然这个是非常不准确的。
		//它在某些时候只能反映动态特性，而存在静态偏差。
		//在短时间内可以根据它进行计算,长期应与加速度实测值融合。
		ex = (ay*vz - az*vy) ;                           					 //向量外积在相减得到差分就是误差
		ey = (az*vx - ax*vz) ;
		ez = (ax*vy - ay*vx) ;					
		//这3个是实际测量的加速度与计算出的重力方向向量的矢量积,
		//指明了角度的误差，
		exInt = exInt + ex * Ki;								  //对误差进行积分
		eyInt = eyInt + ey * Ki;
		ezInt = ezInt + ez * Ki;
		//这个是对该矢量积的积分
		// adjusted gyroscope measurements				
		normalization_4(&IMU_q0,&IMU_q1,&IMU_q2,&IMU_q3);
		normalization_3(&ax,&ay,&az);	
		(*pitch) = asin(-2 * IMU_q1 * IMU_q3 + 2 * IMU_q0* IMU_q2); // pitch俯仰角
		(*roll) = atan2(2 * IMU_q2 * IMU_q3 + 2 * IMU_q0 * IMU_q1, -2 * IMU_q1 * IMU_q1 - 2 * IMU_q2* IMU_q2 + 1); // roll滚翻角
		#define	PI 3.14159
		(*yaw) += wz/180*PI;	
		send_float((*roll),3);
		send_tab();	
		send_float((*pitch),3);
		send_tab();
		send_float((*yaw),3);
		send_ln();
		
	}
#endif












