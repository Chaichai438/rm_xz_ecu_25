#ifndef  __GYRO_H
#define  __GYRO_H
#include "INS_task.h"
#include "sys.h"
#include "struct_typedef.h"
typedef struct 
{
    float LastP;//上次估算协方差 初始化值为0.02
    float Now_P;//当前估算协方差 初始化值为0
    float out;//卡尔曼滤波器输出 初始化值为0
    float Kg;//卡尔曼增益 初始化值为0
    float Q;//过程噪声协方差 初始化值为0.001
    float R;//观测噪声协方差 初始化值为0.543
}KFP;//Kalman Filter parameter
extern float GYRO_YAW_ANGLE_C_X,GYRO_YAW_ANGLE_C_LAST_X;
extern float GYRO_PITCH_ANGLE_C_X,GYRO_PITCH_ANGLE_C_LAST_X;
extern float GYRO_YAW_XHH,GYRO_PITCH_XHH;
extern volatile float GYRO_YAW_ANGLE_GET_X,GYRO_PITCH_ANGLE_GET_X,GYRO_G_Z_XHH,GYRO_G_X_XHH;
extern float GYRO_diff_x,GYRO_diff_y;
extern int GYRO_n_xhh;
float kalmanFilter(KFP *kfp,float input);

extern fp32 INS_angle[3];      //euler angle, unit rad.欧拉角 单位 rad

extern fp32 GYRO_euler_angle_fp32[3];
extern fp32 GYRO_gyro_fp32[3];

//extern fp32 INS_angle[3];      //euler angle, unit rad.欧拉角 单位 rad

void gyro_get(void);
#endif
