#ifndef  MOTOR__ALL__H__
#define  MOTOR__ALL__H__

#include "struct_typedef.h"
#include <stdint.h>
/*******************电机ID******************/
/*
can1
底盘0x201，2，3，4
云台yaw0x205
pitch0x206
Feed0x207
can2


*/
#define Chassis1_ID 0X201
#define Chassis2_ID 0X202
#define Chassis3_ID 0X203
#define Chassis4_ID 0X204	
#define YAW_ID 0x205	
#define PITCH_ID 0x206
#define FEED_ID 0x207
#define Fric0_ID 0x201
#define Fric1_ID 0x202

#define  YAW_ANGLE_OFFSET -5600

typedef struct
{
	uint16_t        real_angle;						//实际角度，一圈8192
	fp32       			rpm;
	int16_t        	real_current;
	uint8_t         temperature;

	uint16_t        last_real_angle;				//上次实际角度
	int32_t         vis_angle;							//处理后角度值
	int32_t         angle_set;						//设定角度，一圈8192	
	fp32            current;						//处理后的实际电流
} chassis_motor_back;

typedef struct
{
	//实际角度
	uint16_t 		real_angle;
	fp32 				rpm;	
	int16_t 		real_current;
	uint8_t     temperature;

	uint16_t 		last_real_angle;				//上次实际角度	
	fp32 		vis_angle;						//处理后角度，一圈8192
	fp32 				angle_set;					//设定角度，一圈8192

	int16_t 		set_current;	//给定转矩电流
	int32_t 		last_vis_angle;		
} gimbal_motor_back;

typedef struct
{
	uint16_t        real_angle;						//实际角度，一圈8192
	fp32       			rpm;
	int16_t        real_current;
	uint8_t         temperature;
}Raw_Motor_Back;
typedef struct
{
	int16_t        motor0set;					
	int16_t        motor1set;		
	int16_t        motor2set;		
	int16_t        motor3set;		
}updown_chassisSet;

#define  NO_HEAD 1

void Get_Gimbal_Yaw_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive);

void Get_Gimbal_Pitch_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive);

void Get_Chassis_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive);

void Get_Fric_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive);

void Get_Feed_Motor_Info( uint32_t motor_id, uint8_t *canbuf_receive);

void Get_UP_Board_Chassis_Info( uint32_t motor_id, uint8_t *canbuf_receive);
#endif
