#ifndef  __CHASSIS_H
#define  __CHASSIS_H
#include "struct_typedef.h"
#include "pid_regulator.h"
#include "pid_st.h"

//可能需要修改（步兵不用）
#define CHASSIS_A			0.3f				//底盘宽度
#define CHASSIS_B			0.3f				//底盘长度

typedef struct
{
	u16 real_angle;						//实际角度，一圈8192
	signed short rpm;
	signed short real_current;
	u8 temperature;
	
	
	u16 last_real_angle;				//上次实际角度
	int angle;							//处理后角度值
	
	float current;						//处理后的实际电流
	
	int angle_set;						//设定角度，一圈8192
	
} chassis_motor_back;



void Chassis_pid_init(void);
void Chassis_Motor_Speed_Set(float vx,float vy,float vyaw);
void Get_Chassis_Motor_Info( uint32_t motor_id, u8 *canbuf_receive);
void Get_Chaojidianrong(uint32_t motor_id, u8 *canbuf_receive);

extern PID_Regulator_t CHASSIS_SPD_PID[4];
extern chassis_motor_back chassis_motor_info[4];
void Chassis_Power_Limit(void);


#endif // CHASSIS_H_INCLUDED



