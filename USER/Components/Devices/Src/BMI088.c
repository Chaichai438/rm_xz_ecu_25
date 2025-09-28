#include "BMI088.h"
#include "main.h"
#include "struct_typedef.h"

#define XHH 1          //RM小黑盒
#define	RM_IMU_QUAT_ID 0x401
#define	RM_IMU_GYRO_ID 0x402
#define	RM_IMU_ACCEL_ID 0x403
#define	RM_IMU_MAG_ID 0x404
#define   RM_IMU_PARAM_ID 0x405
//转换成 m/s^2
#define  ACCEL_3G_SEN 0.0008974358974f
#define  ACCEL_6G_SEN 0.00179443359375f
#define  ACCEL_12G_SEN 0.0035888671875f
#define  ACCEL_24G_SEN 0.007177734375f
//转换成 rad/s
#define  GYRO_2000_SEN 0.00106526443603169529841533860381f
#define  GYRO_1000_SEN 0.00053263221801584764920766930190693f
#define  GYRO_500_SEN 0.00026631610900792382460383465095346f
#define  GYRO_250_SEN 0.00013315805450396191230191732547673f
#define  GYRO_125_SEN 0.000066579027251980956150958662738366f

float YAW_ANGLE_C_X=0,YAW_ANGLE_C_LAST_X=0;
float PITCH_ANGLE_C_X=0,PITCH_ANGLE_C_LAST_X=0;
float YAW_XHH,PITCH_XHH;
volatile float YAW_ANGLE_GET_X,PITCH_ANGLE_GET_X,G_Z_XHH,G_X_XHH;
float diff_x=0,diff_y;
int n_xhh=0;




rm_imu_data_t rm_imu_data;

void IMU_BIM088(CAN_HandleTypeDef *hcan,CAN_RxHeaderTypeDef rx_message,uint8_t *IMU_data){
	
	switch(rx_message.StdId)
	{
		 case RM_IMU_PARAM_ID:
		 {
				 rm_imu_data.accel_rangle = IMU_data[0] &0x0F;
				 rm_imu_data.gyro_rangle = (IMU_data[0] &0xF0) >> 4;
				 rm_imu_data.sensor_control_temperature = IMU_data[2];
				 rm_imu_data.imu_sensor_rotation = IMU_data[3] & 0x1F;
				 rm_imu_data.ahrs_rotation_sequence = (IMU_data[3] & 0xE0) >> 5;
				 rm_imu_data.quat_euler = IMU_data[4] & 0x01;
				 switch(rm_imu_data.gyro_rangle)
				 {
						 case 0: rm_imu_data.gyro_sen = GYRO_2000_SEN; break;
						 case 1: rm_imu_data.gyro_sen = GYRO_1000_SEN; break;
						 case 2: rm_imu_data.gyro_sen = GYRO_500_SEN; break;
						 case 3: rm_imu_data.gyro_sen = GYRO_250_SEN; break;
						 case 4: rm_imu_data.gyro_sen = GYRO_125_SEN; break;
				 }
				 switch(rm_imu_data.accel_rangle)
				 {
						 case 0: rm_imu_data.accel_sen = ACCEL_3G_SEN; break;
						 case 1: rm_imu_data.accel_sen = ACCEL_6G_SEN; break;
						 case 2: rm_imu_data.accel_sen = ACCEL_12G_SEN; break;
						 case 3: rm_imu_data.accel_sen = ACCEL_24G_SEN; break;
				 }
				 break;
		 }
		 case RM_IMU_QUAT_ID:
		 {
				 if(rm_imu_data.quat_euler && rx_message.DLC == 6)
				 {
						 memcpy(rm_imu_data.euler_angle, IMU_data, rx_message.DLC);
						 rm_imu_data.euler_angle_fp32[0] = rm_imu_data.euler_angle[0] * 0.0001f;
						 rm_imu_data.euler_angle_fp32[1] = rm_imu_data.euler_angle[1] * 0.0001f;
						 rm_imu_data.euler_angle_fp32[2] = rm_imu_data.euler_angle[2] * 0.0001f;
				 }
				 else if(rm_imu_data.quat_euler == 0 && rx_message.DLC == 8)
				 {
						 memcpy(rm_imu_data.quat, IMU_data, rx_message.DLC);
						 rm_imu_data.quat_fp32[0] = rm_imu_data.quat[0] * 0.0001f;
						 rm_imu_data.quat_fp32[1] = rm_imu_data.quat[1] * 0.0001f;
						 rm_imu_data.quat_fp32[2] = rm_imu_data.quat[2] * 0.0001f;
						 rm_imu_data.quat_fp32[3] = rm_imu_data.quat[3] * 0.0001f;
				 }
					break;
		 }
		 case RM_IMU_GYRO_ID:
		 {
				 memcpy(rm_imu_data.gyro_int16, IMU_data,6);
				 rm_imu_data.gyro_fp32[0] = rm_imu_data.gyro_int16[0] * rm_imu_data.gyro_sen;
				 rm_imu_data.gyro_fp32[1] = rm_imu_data.gyro_int16[1] * rm_imu_data.gyro_sen;
				 rm_imu_data.gyro_fp32[2] = rm_imu_data.gyro_int16[2] * rm_imu_data.gyro_sen;
				 rm_imu_data.sensor_temperature = (int16_t)((IMU_data[6] << 3) | (IMU_data[7] >>5));
				 if (rm_imu_data.sensor_temperature > 1023)
				 {
						rm_imu_data.sensor_temperature -= 2048;
				 }
					break;
		 }
		 case RM_IMU_ACCEL_ID:
		 {
				 memcpy(rm_imu_data.accel_int16, IMU_data,6);
				 rm_imu_data.accel_fp32[0] = rm_imu_data.accel_int16[0] * rm_imu_data.accel_sen;
				 rm_imu_data.accel_fp32[1] = rm_imu_data.accel_int16[1] * rm_imu_data.accel_sen;
				 rm_imu_data.accel_fp32[2] = rm_imu_data.accel_int16[2] * rm_imu_data.accel_sen;
				 memcpy(&rm_imu_data.sensor_time, (IMU_data + 6), 2);
				 break;
		 }
		 case RM_IMU_MAG_ID:
		 {
				 memcpy(rm_imu_data.mag_int16, IMU_data,6);
				 break;
		 } 
		}
	
	 YAW_XHH=(rm_imu_data.euler_angle_fp32[0]/(3.1415926*2))*360.0;
	 PITCH_XHH=(rm_imu_data.euler_angle_fp32[1]/(3.1415926*2))*360.0;
	 G_Z_XHH=rm_imu_data.gyro_fp32[2]*100;
	 G_X_XHH=rm_imu_data.gyro_fp32[1]*100;
	  
	
	  YAW_ANGLE_C_X=YAW_XHH;
	 if(n_xhh>400){
		diff_x=YAW_ANGLE_C_X-YAW_ANGLE_C_LAST_X;
	 }
	if(diff_x>=300)  
	{	
		diff_x=diff_x-360;
	}
	else if (diff_x<=-300)
	{
		diff_x=diff_x+360;
	}
	YAW_ANGLE_C_LAST_X=YAW_ANGLE_C_X;
	YAW_ANGLE_GET_X+=diff_x;
	
	PITCH_ANGLE_C_X=PITCH_XHH;
  if(n_xhh>400)
	diff_y=PITCH_ANGLE_C_X-PITCH_ANGLE_C_LAST_X;
	if(diff_y>=300)  
	{	
		diff_y=diff_y-360;
	}
	else if (diff_y<=-300)
	{
		diff_y=diff_y+360;
	}
	
	PITCH_ANGLE_C_LAST_X=PITCH_ANGLE_C_X;
	PITCH_ANGLE_GET_X+=diff_y;
	
	

	if(n_xhh<500){
		n_xhh++;
	}
}

