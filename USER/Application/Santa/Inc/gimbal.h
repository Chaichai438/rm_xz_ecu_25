#ifndef GIMBAL_H_INCLUDED
#define GIMBAL_H_INCLUDED

#include "struct_typedef.h"
#include "pid_st.h"
#include "pid_regulator.h"

// 步兵3
#define GIMBAL_YAW_OFFSET  	-737	//零点//定义的车的前面
#define GIMBAL_PITCH_OFFSET	5900		//零点

// 在 gimbal_ctrl.h 中添加
#define constrain_float(value, min, max) \
    ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

// 达妙4310电机控制结构
typedef struct {
    float pos;       // 位置反馈
    float vel;       // 速度反馈
    float tor;       // 扭矩反馈
    float pos_set;   // 位置设定值
    float vel_set;   // 速度设定值
    uint8_t mode;    // 控制模式 (0:位置模式, 1:速度模式)
} dm4310_motor_t;

// 视觉控制模式定义
#define PITCH_VISION_MODE 1
#define PITCH_REMOTE_MODE 2

typedef struct
{
	//实际角度
	u16 real_angle;
	u16 last_real_angle;
							
	signed short real_current;		//实际转矩电流
	signed short set_current;		//给定转矩电流
	
	int angle;						//处理后角度，一圈8192
	int last_angle;	
	float raw_rate;					//转速（两次接收角度差）
		
	float angle_set;					//设定角度，一圈8192
	
} gimbal_motor_back;


void Gimbal_pid_init(void);
void Gimbal_Motor_Speed_Set( float yaw_motor_pos, float pitch_motor_pos);			//设置两个云台电机的位置
void Get_Gimbal_Motor_Info( u32 motor_id, u8 *canbuf_receive);

extern PID_Regulator_t PITCH_SPD_PID,YAW_SPD_PID;
extern PID_Regulator_t PITCH_POS_PID,YAW_POS_PID;
extern PID_Regulator_t PITCH_POS_VISION_PID,YAW_POS_VISION_PID;

extern pid_t gimbal_pitch_spd_pid,gimbal_yaw_spd_pid;
extern pid_t gimbal_pitch_pos_pid,gimbal_yaw_pos_pid;
extern gimbal_motor_back gimbal_motor_info[2];
extern int mode_flag;
extern float pitch_test,yaw_test; 
extern float last_num,last_pos;

// 达妙4310电机控制函数
extern void DM4310_Init(void);
extern void DM4310_Set_Speed(float vel_set);
extern void DM4310_Set_Position(float pos_set);
extern void DM4310_Enable(void);
extern void DM4310_Disable(void);
extern dm4310_motor_t pitch_dm4310;

#endif // GIMBAL_H_INCLUDED
