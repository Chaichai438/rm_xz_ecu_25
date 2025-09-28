#ifndef CHASSIS_CTRL_H_INCLUDED
#define CHASSIS_CTRL_H_INCLUDED

#include "struct_typedef.h"
#include "pid_regulator.h"

//以下宏定义跟底盘速度有关系，自己测试修改（盲调自己看感觉吧）

#define CHASSIS_VX_MAX 		5.5f				//x轴速度最大值5.5
#define CHASSIS_VY_MAX 		5.5f				//y轴速度最大值
#define CHASSIS_VYAW_MAX	4.0f  				//旋转速度最大值
#define CHASSIS_A			0.3f				//底盘宽度
#define CHASSIS_B			0.3f				//底盘长度

#define CHASSIS_SWING_CLOCKWISE			1		//扭腰，顺时针
#define CHASSIS_SWING_ANTICLOCKWISE		2		//扭腰，逆时针
#define CHASSIS_SWING_ANGLE				1000	//扭腰角度（一圈8192）

//#define CHASSIS_VX_KEY_MAX				1.0f	//键盘控制时x轴速度最大值
//#define CHASSIS_VY_KEY_MAX				1.0f	//键盘控制时y轴速度最大值
//#define CHASSIS_VYAW_KEY_MAX			0.6f	//键盘控制时旋转速度最大值
#define CHASSIS_FOLLOW_ANGLE_ALLOWANCE	30		//底盘跟随角度余量



void Chassis_Init(void);		//底盘初始化
void Chassis_Ctrl(void);		//底盘控制
float Chassis_Swing( u8 Command, float chassis_yaw_speed);			//扭腰
float Chassis_Follow( void);										//底盘跟随
void chaojidianrong_can(void);

extern PID_Regulator_t CHASSIS_FOLLOW_PID;

#endif // CHASSIS_H_INCLUDED
