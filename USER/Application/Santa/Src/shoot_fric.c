#include "shoot_fric.h"
#include "ammunition_feed.h"
#include "can.h"
#include "pid_regulator.h"
#include "remote_control.h"
#include "motor.h"
#include "referee_st.h"
ammunition_motor_back shoot_motor_info[2];
PID_Regulator_t SHOOT_SPD_PID[2];
extern Game_info game_info;
extern RC_ctrl_t rc_ctrl;
int a=0;

void Shoot_fric_pid_init(void)
{
	u8 t;
	for( t = 0; t < 2; ++t)
	{	
		SHOOT_SPD_PID[t].kp=5.0f;
		SHOOT_SPD_PID[t].ki=0.00f;
		SHOOT_SPD_PID[t].kd=0.01f;
		SHOOT_SPD_PID[t].componentKpMax=13000;
		SHOOT_SPD_PID[t].componentKiMax=3000;
		SHOOT_SPD_PID[t].componentKdMax=0;
		SHOOT_SPD_PID[t].outputMax=16000;
		
		shoot_motor_info[t].angle 	= 0;
		shoot_motor_info[t].angle_set = 0;
	}
}
int shoot_spd;
void Shoot_fric_Ctrl(void)//根据限制来改变电机转速
{
		if(rc_ctrl.rc.s[0] == 3||rc_ctrl.rc.s[0] == 1)
		{		//	升级弹速
			//a=-5000;
//			if(game_info.game_robot_state.shooter_id1_17mm_speed_limit == 15)
//				{
//							shoot_spd=4170;  //4350  4270
//				}
//			else if(game_info.game_robot_state.shooter_id1_17mm_speed_limit == 18)
//				{
//							shoot_spd=4600;   //4770
//				}
//			else if(game_info.game_robot_state.shooter_id1_17mm_speed_limit == 22)
//				{
//							shoot_spd=5300;
//				}
//			else if(game_info.game_robot_state.shooter_id1_17mm_speed_limit == 30)
//				{
//							shoot_spd=7200;
//				}	
//				else shoot_spd=4270;
			shoot_spd=5300;
		}
	else
	{	shoot_spd=0;
	a=0;}
	Shoot_Speed_Set(-shoot_spd);
}
void Shoot_Speed_Set(int shoot_spd)
{
	short pid_out[2];
	u8 canbuf[8];
	
	SHOOT_SPD_PID[0].fdb=shoot_motor_info[0].rpm;
	SHOOT_SPD_PID[0].ref=-shoot_spd;
	PID_Calc(&SHOOT_SPD_PID[0]);
	pid_out[0]=SHOOT_SPD_PID[0].output;
	SHOOT_SPD_PID[1].fdb=shoot_motor_info[1].rpm;
	SHOOT_SPD_PID[1].ref=shoot_spd;
	PID_Calc(&SHOOT_SPD_PID[1]);
	pid_out[1]=SHOOT_SPD_PID[1].output;
	canbuf[0]=pid_out[0]>>8;
	canbuf[1]=pid_out[0];
	canbuf[2]=pid_out[1]>>8;
	canbuf[3]=pid_out[1];
	canbuf[4]=0;
	canbuf[5]=0;
	canbuf[6]=0;
	canbuf[7]=0;
	//CAN2_Send_Msg(0x200,canbuf,8);
}
void Get_Shoot_Motor_Info( uint32_t motor_id, u8 *canbuf_receive)//数据处理
{
	static u32 cnt = 0;
	short diff;
	
	if( motor_id-0x200>0&& motor_id-0x200<3)
	{
		shoot_motor_info[motor_id - 0x201].last_real_angle = shoot_motor_info[motor_id - 0x201].real_angle;			//保存上次角度
		
		//读取当前信息
		shoot_motor_info[motor_id - 0x201].real_angle 	= canbuf_receive[0] << 8 | canbuf_receive[1];
		shoot_motor_info[motor_id - 0x201].rpm 			= (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		shoot_motor_info[motor_id - 0x201].real_current 	= (signed short)(canbuf_receive[4] << 8 | canbuf_receive[5]);
		shoot_motor_info[motor_id - 0x201].temperature 	= canbuf_receive[6];
				
		//过零点处理
		diff = shoot_motor_info[motor_id - 0x201].real_angle - shoot_motor_info[motor_id - 0x201].last_real_angle;
		if( diff > 7500)
		{
			diff = -(8192 - diff);
		}
		else if( diff < -7500)
		{
			diff = (8192 + diff);
		}
		
		
		//排除第一次的数据	
		if( cnt == 0)
			diff = 0;				
		++cnt;	
		
		ammunition_motor_info.last_angle = ammunition_motor_info.angle;
		
		ammunition_motor_info.angle += diff;
   
		
		
	}
	
}