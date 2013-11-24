#include "mpu6050.h"
#include "serial.h"
#include <math.h>
uchar xdata MPU6050_Record[14];			//��ȡ�����ݴ洢�ڴˡ�
ulong xdata MPU6050_offset[6];
/*
���Ҫ��ȡMPU-60X0 �Ĵ�����ֵ��
��1�����������豸������ʼ�źţ�S����
��2��Ȼ���ʹ��豸��ַλ��һ��д����λ��
��3��Ȼ���ͼĴ�����ַ�����ܿ�ʼ���Ĵ�����
��4�������ţ��յ�Ӧ���źź����豸�ٷ�һ����ʼ�źš�
��5��Ȼ���ʹ��豸��ַλ��һ��������λ��
��6��Ȼ����Ϊ���豸��MPU-60X0 ����Ӧ���źŲ���ʼ���ͼĴ������ݡ�
��*��ͨ�������豸�����ľܾ�Ӧ���źţ�NACK���ͽ�����־��P��������
��*���ܾ�Ӧ���źţ�NACK����������ΪSDA�����ڵ�9��ʱ������һֱΪ�ߡ�
*/
/*���ֽ�д*/
void Single_Write_MPU6050(uchar Address,uchar Dat)
{
    I2C_Start();
    I2C_Send_Byte(SlaveAddress);
    I2C_Send_Byte(Address);
    I2C_Send_Byte(Dat);
    I2C_Stop();
}
//���ֽڶ�MPU6050
/*
uchar Single_Read_MPU6050(uchar Addr)
{
    uchar Value;
    IIC_Start();//��1�����������豸������ʼ�źţ�S��
	
    IIC_Send_Byte(SlaveAddress);//��2��Ȼ���ʹ��豸��ַλ��һ��д����λ��������������SlaveAddress��

   IIC_Send_Byte(Addr); //���ʹ洢��Ԫ��ַ
	
    IIC_Start();	 //��4�������ţ��յ�Ӧ���źź����豸�ٷ�һ����ʼ�źš�
	
    IIC_Send_Byte(SlaveAddress+1);	// ��5��Ȼ���ʹ��豸��ַλ��һ��������λ��������������SlaveAddress+1��
    
	Value=IIC_Rec_Byte(); //��ȡ��ַ���ݣ��洢��Value
    IIC_SendAck(1);		  //��NOACK
    IIC_Stop();
	
//	show_number((int)(Value));
	delay(10);	
    return Value;
}	
*/
//���ֽڶ�HMC5833
void MPU6050_Multiple_Read()
{
    uchar i;  //��������HMC5883�ڲ��Ƕ����ݣ���ַ��Χ0x3~0x5
    I2C_Start();		//��1�����������豸������ʼ�źţ�S��
    I2C_Send_Byte(SlaveAddress); //��2��Ȼ���ʹ��豸��ַλ��һ��д����λ��������������SlaveAddress��
    I2C_Send_Byte(0x3B);//���ʹ洢��Ԫ��ַ����0x3B��ʼ 
    I2C_Start();		  //��4�������ţ��յ�Ӧ���źź����豸�ٷ�һ����ʼ�źš�
    I2C_Send_Byte(SlaveAddress+1);	// ��5��Ȼ���ʹ��豸��ַλ��һ��������λ��������������SlaveAddress+1��
    for(i=0;i<14;i++) //������ȡ6����ַ���ݣ��洢��Rec_Data
    {
        MPU6050_Record[i]=I2C_Rec_Byte();
        if(i==13)
            I2C_SendAck(1); //���һ��������Ҫ��NOACK
        else
            I2C_SendAck(0); //��ӦACK
    }
    I2C_Stop();
}

//**************************************  
//��ʼ��MPU6050  
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
			//�������ƫ��
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
	if(s>127){return s-256;}//��255��Ϊ-1

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
//�����������
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
//��ø�������ʵ���ݡ���λ�ֱ���reg/s��g.
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
//��ά������һ������
//***********************************
void normalization_3(float * x,float * y,float * z){
	float m=sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z));
	(*x)/=m;
	(*y)/=m;
	(*z)/=m;	 
}
//**********************************
//��ά������һ������
//***********************************
void normalization_4(float * x,float * y,float * z,float *w){
	float m=sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z)+(*w)*(*w));
	(*w)/=m;
	(*x)/=m;
	(*y)/=m;
	(*z)/=m;	 
}
//**********************************
//��Ԫ���ں�+PID�˲�
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
		wx = wx + Kp*ex + exInt;					   							//�����PI�󲹳��������ǣ����������Ư��
		wy = wy + Kp*ey + eyInt;
		wz = wz + Kp*ez + ezInt;	
		IMU_q0 = IMU_q0 + (-IMU_q1*wx - IMU_q2*wy - IMU_q3*wz)*IMU_T;
		IMU_q1 = IMU_q1 + (IMU_q0*wx + IMU_q2*wz - IMU_q3*wy)*IMU_T;
		IMU_q2 = IMU_q2 + (IMU_q0*wy - IMU_q1*wz + IMU_q3*wx)*IMU_T;
		IMU_q3 = IMU_q3 + (IMU_q0*wz + IMU_q1*wy - IMU_q2*wx)*IMU_T;
		//�������Ԫ��������ʹ�õ�����Ԫ������ת΢�ַ��̡�	
		vx = 2*(IMU_q1*IMU_q3 - IMU_q0*IMU_q2);												//��Ԫ����xyz�ı�ʾ
		vy = 2*(IMU_q0*IMU_q1 + IMU_q2*IMU_q3);
		vz = IMU_q0*IMU_q0 - IMU_q1*IMU_q1 - IMU_q2*IMU_q2 + IMU_q3*IMU_q3;
		//������Ԫ���������̬����Ȼ����Ƿǳ���׼ȷ�ġ�
		//����ĳЩʱ��ֻ�ܷ�ӳ��̬���ԣ������ھ�̬ƫ�
		//�ڶ�ʱ���ڿ��Ը��������м���,����Ӧ����ٶ�ʵ��ֵ�ںϡ�
		ex = (ay*vz - az*vy) ;                           					 //�������������õ���־������
		ey = (az*vx - ax*vz) ;
		ez = (ax*vy - ay*vx) ;					
		//��3����ʵ�ʲ����ļ��ٶ�����������������������ʸ����,
		//ָ���˽Ƕȵ���
		exInt = exInt + ex * Ki;								  //�������л���
		eyInt = eyInt + ey * Ki;
		ezInt = ezInt + ez * Ki;
		//����ǶԸ�ʸ�����Ļ���
		// adjusted gyroscope measurements				
		normalization_4(&IMU_q0,&IMU_q1,&IMU_q2,&IMU_q3);
		normalization_3(&ax,&ay,&az);	
		(*pitch) = asin(-2 * IMU_q1 * IMU_q3 + 2 * IMU_q0* IMU_q2); // pitch������
		(*roll) = atan2(2 * IMU_q2 * IMU_q3 + 2 * IMU_q0 * IMU_q1, -2 * IMU_q1 * IMU_q1 - 2 * IMU_q2* IMU_q2 + 1); // roll������
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












