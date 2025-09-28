#include "chassis_ctrl.h"
#include "remote_control.h"
#include "chassis.h"
#include "usart.h"
#include "gimbal.h"
#include "gimbal_ctrl.h"
#include "ramp.h"
#include "chassis_power.h"
//#include "mpu6500.h"
#include "pid_regulator.h"
#include "motor.h"
#include "tim.h"
#include "referee_st.h"
#include <math.h>
#include "init.h"
extern volatile uint32_t judge_data_cnt;
extern RC_ctrl_t rc_ctrl;
extern float Power,Power_Buffer;
extern float powerdata[4];
extern int pitch_duandian;
ext_game_robot_state_t robo_level_DJ;
int biansu=0;

Ramp chassis_x_ramp;			//底盘x斜坡
Ramp chassis_y_ramp;			//底盘y斜坡
Ramp chassis_yaw_ramp;			//底盘yaw斜坡
Ramp chassis_Q_ramp;			//风车斜坡

PID_Regulator_t CHASSIS_FOLLOW_PID;

u8 s_yaw;

void Chassis_Ctrl_k();		//键盘控制底盘


//pid自己修改
void Chassis_Init(void)
{
	Chassis_pid_init();                        //底盘跟随pid
	CHASSIS_FOLLOW_PID.kp=0.005f;
	CHASSIS_FOLLOW_PID.ki=0.0f;
	CHASSIS_FOLLOW_PID.kd=0.001f;
	CHASSIS_FOLLOW_PID.componentKpMax=3.0f;
	CHASSIS_FOLLOW_PID.componentKiMax=0.5f;
	CHASSIS_FOLLOW_PID.componentKdMax=0.5f;
	CHASSIS_FOLLOW_PID.outputMax=3.0f;
	

}


static float vx,vy,vyaw;

void Chassis_Ctrl(void)
{
	u8 t;
	int tttt=0;
	short motor[4];
	float chassis_spd[4];
	static u8 last_last_s=0,last_s=0,s=0;

	float current[4],last_chassis_spd[4]={0},accel[4],accel_limit=1.0f,accel_sum=0; 
	

	if(rc_ctrl.rc.s[1] == RC_SW_DOWN)//2
	{
		vyaw = 0;//Chassis_Follow();
	}
	
	if(rc_ctrl.rc.s[1] == RC_SW_UP)//1
	{
		vyaw = 0;//Chassis_Follow();
	}


	//遥控器数据映射到速度（底盘无头模式） 可以改CHASSIS_VX_MAX变量 控制缓冲功率不足时最大速度不同
	//前进后退
	vx =  -(float)(rc_ctrl.rc.ch[3]) * ( CHASSIS_VX_MAX / 660.0f)* cos( (float)(gimbal_motor_info[0].angle/* -GIMBAL_OFFSET*/)/8192.0f * PI*2.0f) 
		+(float)(rc_ctrl.rc.ch[2]) * ( CHASSIS_VX_MAX / 660.0f) * sin( -(float)(gimbal_motor_info[0].angle/* -GIMBAL_OFFSET*/)/8192.0f * PI*2.0f);
	//左右平移		
	vy =(float)(rc_ctrl.rc.ch[3]) * ( CHASSIS_VX_MAX / 660.0f) * sin( (float)(gimbal_motor_info[0].angle /*-GIMBAL_OFFSET*/)/8192.0f * PI*2.0f) 
		- (float)(rc_ctrl.rc.ch[2]) * ( CHASSIS_VX_MAX / 660.0f) * cos( -(float)(gimbal_motor_info[0].angle/* -GIMBAL_OFFSET*/)/8192.0f * PI*2.0f);//全向轮步兵计算符号相反
			
	if(rc_ctrl.rc.s[1]==1)
	{
		if(vx>1)vx=1;
		if(vy>1)vy=1;
		if(vx<-1)vx=-1;
		if(vy<-1)vy=-1;
	}
	

		
	if(rc_ctrl.rc.s[1] == RC_SW_MID)  //3
	{
		Chassis_Ctrl_k();			//键盘控制
		vyaw = 0;//Chassis_Follow();	//底盘跟随(云台)
		if(RC_Key_Read(RC_KEY_C)==1)vyaw=3.0;                        
	}		
	Chassis_Motor_Speed_Set(vx,vy,vyaw);
}

extern ext_game_robot_state_t             robot_state;                    // 0x0201  比赛机器人状态
extern Game_info game_info;						//裁判系统数据
u16 temp=0;

void chaojidianrong_can(void)
{
	u8 send_can[8];
	temp=(game_info.game_robot_state.chassis_power_limit )* 100.0f;
	
	
	send_can[0]=temp >> 8;
	send_can[1]=temp;
	CAN1_Send_Msg(0x210,send_can,8);
}


float vx_temp=0,vy_temp=0;
int cs=0,co=0;
//键盘控制
void Chassis_Ctrl_k()
{
	
	static float CHASSIS_VX_KEY_MAX=0.9,CHASSIS_VY_KEY_MAX=0.9,CHASSIS_VYAW_KEY_MAX=0.6;    //如果缓冲功率不足时，可以改小最大值
	static int x_R=0,flag_R=0;
	if(RC_Key_Read(RC_KEY_X)==1)	
	{
		pitch_duandian=0;
	}
	else
		pitch_duandian=1;
	if(RC_Key_Read(RC_KEY_SHIFT)==1)
	{
		CHASSIS_VX_KEY_MAX=4;
		CHASSIS_VY_KEY_MAX=4;
		CHASSIS_VYAW_KEY_MAX=4;//
//		CHASSIS_VX_KEY_MAX+=1.3;
//		CHASSIS_VY_KEY_MAX+=1.3;
//		CHASSIS_VYAW_KEY_MAX+=1.3;//
	}
	else
	{
			if(temp==4500)
		{
			CHASSIS_VX_KEY_MAX=1.9;
			CHASSIS_VY_KEY_MAX=1.9;
			CHASSIS_VYAW_KEY_MAX=2.0;
		}
		if(temp==5000)
		{
			CHASSIS_VX_KEY_MAX=2.0;
			CHASSIS_VY_KEY_MAX=2.0;
			CHASSIS_VYAW_KEY_MAX=2.3;
		}
			if(temp==5500)
		{
			CHASSIS_VX_KEY_MAX=2.3;
			CHASSIS_VY_KEY_MAX=2.3;
			CHASSIS_VYAW_KEY_MAX=2.6;
		}
			if(temp==6000)
		{
		CHASSIS_VX_KEY_MAX=2.4;
		CHASSIS_VY_KEY_MAX=2.4;
		CHASSIS_VYAW_KEY_MAX=2.4;
		}
			if(temp==6500)
		{
		CHASSIS_VX_KEY_MAX=2.7;
		CHASSIS_VY_KEY_MAX=2.7;
		CHASSIS_VYAW_KEY_MAX=2.7;
		}
			if(temp==7000)
		{
		CHASSIS_VX_KEY_MAX=3.0;
		CHASSIS_VY_KEY_MAX=3.0;
		CHASSIS_VYAW_KEY_MAX=3.0;
		}
			if(temp==7500)
		{
		CHASSIS_VX_KEY_MAX=3.0;
		CHASSIS_VY_KEY_MAX=3.0;
		CHASSIS_VYAW_KEY_MAX=3.0;
		}
			if(temp==8000)
		{
		CHASSIS_VX_KEY_MAX=3.1;
		CHASSIS_VY_KEY_MAX=3.1;
		CHASSIS_VYAW_KEY_MAX=3.1;
		}
			if(temp==8500)
		{
		CHASSIS_VX_KEY_MAX=3.2;
		CHASSIS_VY_KEY_MAX=3.2;
		CHASSIS_VYAW_KEY_MAX=3.2;
		}
			if(temp==9000)
		{
		CHASSIS_VX_KEY_MAX=3.2;
		CHASSIS_VY_KEY_MAX=3.2;
		CHASSIS_VYAW_KEY_MAX=3.2;
		}
			if(temp>=9500)
		{
  	CHASSIS_VX_KEY_MAX=3.3;
		CHASSIS_VY_KEY_MAX=3.3;
		CHASSIS_VYAW_KEY_MAX=3.3;
		}
//		else//自己加的
//		{
//			CHASSIS_VX_KEY_MAX=0.9;
//			CHASSIS_VY_KEY_MAX=0.9;
//			CHASSIS_VYAW_KEY_MAX=0.6;
//		}
	}
		


//	else
//	{
//			if(temp==3500)
//		{
//		CHASSIS_VX_KEY_MAX=1.7;
//		CHASSIS_VY_KEY_MAX=1.7;
//		CHASSIS_VYAW_KEY_MAX=2.0;
//		}
//		if(temp==4000)
//		{
//		CHASSIS_VX_KEY_MAX=1.9;
//		CHASSIS_VY_KEY_MAX=1.9;
//		CHASSIS_VYAW_KEY_MAX=2.0;
//		}
//			if(temp==4500)
//		{
//		CHASSIS_VX_KEY_MAX=1.9;
//		CHASSIS_VY_KEY_MAX=1.9;
//		CHASSIS_VYAW_KEY_MAX=2.0;
//		}
//			if(temp==5000)
//		{
//		CHASSIS_VX_KEY_MAX=2.0;
//		CHASSIS_VY_KEY_MAX=2.0;
//		CHASSIS_VYAW_KEY_MAX=2.3;
//		}
//			if(temp==5500)
//		{
//		CHASSIS_VX_KEY_MAX=2.3;
//		CHASSIS_VY_KEY_MAX=2.3;
//		CHASSIS_VYAW_KEY_MAX=2.6;
//		}
//			if(temp==7500)
//		{
//		CHASSIS_VX_KEY_MAX=3.0;
//		CHASSIS_VY_KEY_MAX=3.0;
//		CHASSIS_VYAW_KEY_MAX=3.0;
//		}
//			if(temp==9500)
//		{
//		CHASSIS_VX_KEY_MAX=3.3;
//		CHASSIS_VY_KEY_MAX=3.3;
//		CHASSIS_VYAW_KEY_MAX=3.3;
//		}
//		else//自己加的
//		{
//			CHASSIS_VX_KEY_MAX=0.9;
//			CHASSIS_VY_KEY_MAX=0.9;
//			CHASSIS_VYAW_KEY_MAX=0.6;
//		}
//	}
//		
	
	
	//w按下动作
	if( RC_Key_Action_Read( RC_KEY_W, RC_ACTION_KEY_PRESS) == 1)
	{	
		Ramp_Set( &chassis_x_ramp);
	}
	//s按下动作
	else if( RC_Key_Action_Read( RC_KEY_S, RC_ACTION_KEY_PRESS) == 1)
	{
		Ramp_Set( &chassis_x_ramp);
	}
	//a按下动作
	if( RC_Key_Action_Read( RC_KEY_A, RC_ACTION_KEY_PRESS) == 1)
	{	
		Ramp_Set( &chassis_y_ramp);
	}
	//d按下动作
	else if( RC_Key_Action_Read( RC_KEY_D, RC_ACTION_KEY_PRESS) == 1)
	{
		Ramp_Set( &chassis_y_ramp);
	}
	

	//w按下
	if( RC_Key_Read( RC_KEY_W) == 1 && vx_temp >= 0.0f)
	{
		vx_temp = CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 500, RAMP_UP);   //改变数值，可以改变键盘启动速度
	}//s按下
	else if( RC_Key_Read( RC_KEY_S) == 1 && vx_temp <= 0.0f)
	{
		vx_temp = -CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 500, RAMP_UP);
	}
	//ws抬起
	if( RC_Key_Read( RC_KEY_W) == 0 || RC_Key_Read( RC_KEY_S) == 0)
	{
		if( vx_temp > 0.0f)
			vx_temp = CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 500, RAMP_DOWN);
		if( vx_temp <0.0f)
			vx_temp =- CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 500, RAMP_DOWN);
	}

	//D按下
	if( RC_Key_Read( RC_KEY_D) == 1 && vy_temp >= 0.0f)
	{
		vy_temp = CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 500, RAMP_UP);
	}//A按下
	else if( RC_Key_Read( RC_KEY_A) == 1 && vy_temp <= 0.0f)
	{
		vy_temp = -CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 500, RAMP_UP);
	}
	//ad抬起
	if( RC_Key_Read( RC_KEY_A) == 0 || RC_Key_Read( RC_KEY_D) == 0)
	{
		if( vy_temp > 0.0f)
			vy_temp = CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 500, RAMP_DOWN);
		if( vy_temp < 0.0f)
			vy_temp = -CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 500, RAMP_DOWN);
	}
	
	vx = - vx_temp * cos( (float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f) + vy_temp * sin( -(float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f);
			
	vy = vx_temp * sin( (float)(gimbal_motor_info[0].angle )/8192.0f * PI*2.0f) -vy_temp * cos( -(float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f);
	


}
	


//底盘跟随（云台）
float Chassis_Follow( void)
{
	if( gimbal_motor_info[0].angle > CHASSIS_FOLLOW_ANGLE_ALLOWANCE || gimbal_motor_info[0].angle < -CHASSIS_FOLLOW_ANGLE_ALLOWANCE)		//3395
	{	
		while(ABS(gimbal_motor_info[0].angle)>8192)
		{
		if(gimbal_motor_info[0].angle<0)
				gimbal_motor_info[0].angle+=8192;
		if(gimbal_motor_info[0].angle>8192)
				gimbal_motor_info[0].angle-=8192;
		}
		if(RC_Key_Read(RC_KEY_F)!=1)
		{
			CHASSIS_FOLLOW_PID.ref=0;
			CHASSIS_FOLLOW_PID.fdb=gimbal_motor_info[0].angle;
			PID_Calc(&CHASSIS_FOLLOW_PID);
			return CHASSIS_FOLLOW_PID.output;
		}
	}
	return 0.0f;
}

////扭腰（云台为轴，底盘左右等幅度转动）
//float Chassis_Swing( u8 Command, float chassis_yaw_speed)
//{
//	static u8 dir ;//= CHASSIS_SWING_CLOCKWISE;
//	float temp ;
////printf("dir = %d\r\n", dir);
//	if( Command == SET)
//	{
//		//不细分方向，达到扭腰目的即可
//		switch( dir)
//		{
//			case CHASSIS_SWING_CLOCKWISE:
//			{
//				//底盘与云台达一定角度，底盘转动方向换向
//				if( gimbal_motor_info[0].angle > CHASSIS_SWING_ANGLE)
//				{
//					dir = CHASSIS_SWING_ANTICLOCKWISE;
//				}
//					temp = - chassis_yaw_speed;
////					printf("llll\r\n");
////					return -chassis_yaw_speed;
//				
//			}
//				break;
//			case CHASSIS_SWING_ANTICLOCKWISE:
//			{
//				if( gimbal_motor_info[0].angle < -CHASSIS_SWING_ANGLE)
//				{
//					dir = CHASSIS_SWING_CLOCKWISE;
//				}
//					temp = chassis_yaw_speed;
////					return -chassis_yaw_speed;
//				
//			}
//				break;
//			default :
//			{
//				if( gimbal_motor_info[0].angle > CHASSIS_SWING_ANGLE)
//				{
//					dir = CHASSIS_SWING_ANTICLOCKWISE;
//				}
//					temp = - chassis_yaw_speed;
////					return -chassis_yaw_speed;
//				
//			}
//				break;
//				
//		}
//	}
//	else 
//	{
//		dir = CHASSIS_SWING_CLOCKWISE;
//		temp = - chassis_yaw_speed;

////		return -chassis_yaw_speed;
//	}
//	return temp;
//}

