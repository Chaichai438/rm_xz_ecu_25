#ifndef __BMI088_H
#define __BMI088_H
#include "main.h"
#include "struct_typedef.h"

typedef struct
{
	 uint8_t quat_euler:1;
	 uint8_t gyro_rangle:3;
	 uint8_t accel_rangle:2;
	 uint8_t imu_sensor_rotation:5;
	 uint8_t ahrs_rotation_sequence:3;
	 int16_t quat[4];
	 fp32 quat_fp32[4];
	 int16_t euler_angle[3];
	 fp32 euler_angle_fp32[3];
	 int16_t gyro_int16[3];
	 int16_t accel_int16[3];
	 int16_t mag_int16[3];
	 fp32 gyro_fp32[3];
	 fp32 accel_fp32[3];
	 uint16_t sensor_time;
	 uint16_t sensor_temperature;
	 int16_t sensor_control_temperature;
	 fp32 gyro_sen;
	 fp32 accel_sen;
}rm_imu_data_t;

extern float YAW_ANGLE_C_X,YAW_ANGLE_C_LAST_X;
extern float PITCH_ANGLE_C_X,PITCH_ANGLE_C_LAST_X;
extern float YAW_XHH,PITCH_XHH;
extern volatile float YAW_ANGLE_GET_X,PITCH_ANGLE_GET_X,G_Z_XHH,G_X_XHH;
extern float diff_x,diff_y;
extern int n_xhh;

extern rm_imu_data_t rm_imu_data;
void IMU_BIM088(CAN_HandleTypeDef *hcan,CAN_RxHeaderTypeDef rx_message,uint8_t *IMU_data);

#endif
