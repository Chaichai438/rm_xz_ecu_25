#ifndef GIMBAL_CTRL_H_INCLUDED
#define GIMBAL_CTRL_H_INCLUDED

#include "struct_typedef.h"


//#define PITCH_CHANGE_MAX		4.1f//0.18f					//pitch轴角速度最大值（度/10ms）
//#define YAW_CHANGE_MAX			4.1f//0.18f				//yaw轴角速度最大值


//#define PITCH_CHANGE_MAX		20.5f//0.18f				//pitch轴角速度最大值（度/10ms）
//#define YAW_CHANGE_MAX			20.5f//0.18f			//yaw轴角速度最大值



#define PITCH_CHANGE_MAX		10.0f//0.18f				//pitch轴角速度最大值（度/10ms）
#define YAW_CHANGE_MAX			8.0f//0.18f					//yaw轴角速度最大值


#define PITCH_ZERO_OFFSET		0.0f						//pitch轴零点偏移
#define YAW_ZERO_OFFSET			0.0f						//yaw轴零点偏移

//步兵1限位
#define PITCH_LIMIT_UP				 100//pitch轴限位 2640
#define PITCH_LIMIT_DOWN			 	1000	//pitch轴限位3717

#define YAW_LIMIT				4000						//yaw轴限位

#define YAW_CHANGEMAX      4.0f
#define PITCH_CHANGEMAX    0.5f


void Gimbal_Init(void);
void Gimbal_Ctrl(void);
float Gimbal_Follow( u8 Command, float RC_Data, float Vision_Data);

void Vision_get(int v_yaw,int v_pitch);






#endif // GIMBAL_CTRL_H_INCLUDED
