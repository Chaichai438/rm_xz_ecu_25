#include "ammunition_feed.h"
#include "pid_st.h"
//#include "pwm.h"
#include "can.h"
#include "motor.h"

//该文件与chassis.c大致相同

pid_t ammunition_spd_pid;
pid_t ammunition_pos_pid;

ammunition_motor_back ammunition_motor_info;

//拨弹初始化
void Ammunition_pid_init(void)						
{
	//pid的值自己修改，下面函数里的数字跟pid有关，pid对应那个数字自己goto Define去看
	pid_init( &ammunition_pos_pid, POSITION_PID, 3000, 3000, 0.02f, 0.000000f, 0.00001f);			//位置环初始化
	pid_init( &ammunition_spd_pid, POSITION_PID, 16000, 16000, 5.0f, 0.0f, 0.2f);					//速度环初始化

	ammunition_motor_info.angle 	= 0;
	ammunition_motor_info.angle_set = 0;
	
}




void Get_Ammunition_Motor_Info( uint32_t motor_id, u8 *canbuf_receive)
{
	static u32 cnt = 0;
	short diff;
	
	if( motor_id == 0x207)
	{
		ammunition_motor_info.last_real_angle = ammunition_motor_info.real_angle;			//保存上次角度
		
		//读取当前信息
		ammunition_motor_info.real_angle 	= canbuf_receive[0] << 8 | canbuf_receive[1];
		ammunition_motor_info.rpm 			= (signed short)(canbuf_receive[2] << 8 | canbuf_receive[3]);
		ammunition_motor_info.real_current 	= (signed short)(canbuf_receive[4] << 8 | canbuf_receive[5]);
		ammunition_motor_info.temperature 	= canbuf_receive[6];
				
		//过零点处理
		diff = ammunition_motor_info.real_angle - ammunition_motor_info.last_real_angle;
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


	u8 ammunition_canbuf[8];
	short pid_out;

void Ammunition_Motor_Speed_Set( short motor_speed)//拨弹电机转速/转向设置
{		
//	u8 canbuf[8];
	if( motor_speed == 0)//卡弹处理函数，算法与跟随相似，同时解决掉了刚开始就反转的问题。串联双环
	{
		pid_calc( &ammunition_pos_pid,0,0);		//外环，位置环，实际值（机械角度），目标值（保存角度）
		pid_out = pid_calc( &ammunition_spd_pid, ammunition_motor_info.rpm, ammunition_pos_pid.pos_out);					//内环，速度环，实际值（目标速度），目标值（外环输出）
	}
	else
	{
		pid_out = pid_calc( &ammunition_spd_pid, ammunition_motor_info.rpm, motor_speed);			//单环
		ammunition_motor_info.angle_set = ammunition_motor_info.angle;
	}
	
	ammunition_canbuf[0] = 0;
	ammunition_canbuf[1] = 0;
	ammunition_canbuf[2] = 0;
	ammunition_canbuf[3] = 0;
	ammunition_canbuf[4] = pid_out >> 8;
	ammunition_canbuf[5] = pid_out;
	ammunition_canbuf[6] = 0;
	ammunition_canbuf[7] = 0;

	
//	CAN1_Send_Msg( 0x1ff, ammunition_canbuf, 8);		//发送数据

}






