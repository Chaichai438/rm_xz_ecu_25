#ifndef  __GYRO_H
#define  __GYRO_H
#include "INS_task.h"
#include "struct_typedef.h"

extern float GYRO_YAW_ANGLE_C_X,GYRO_YAW_ANGLE_C_LAST_X;
extern float GYRO_PITCH_ANGLE_C_X,GYRO_PITCH_ANGLE_C_LAST_X;
extern float GYRO_YAW_XHH,GYRO_PITCH_XHH;
extern volatile float GYRO_YAW_ANGLE_GET_X,GYRO_PITCH_ANGLE_GET_X,GYRO_G_Z_XHH,GYRO_G_X_XHH;
extern float GYRO_diff_x,GYRO_diff_y;
extern int GYRO_n_xhh;

extern fp32 INS_angle[3];      //euler angle, unit rad.欧拉角 单位 rad

extern fp32 GYRO_euler_angle_fp32[3];
extern fp32 GYRO_gyro_fp32[3];

//extern fp32 INS_angle[3];      //euler angle, unit rad.欧拉角 单位 rad

void groy_get(void);
#endif
