//#include "chassis.h"
//
//#include "remote_control.h"
//#include "pid.h"
//#include "CAN_receive.h"
//#include "gimbal_task.h"
//#include "mocalun.h"
////#include "gimbal.h"
//#include <math.h>
//#include "ramp.h"
//#include "A101.h"
//#include "INS_task.h"
//#include "user_lib.h"
//#include "Motor_ALL.h"
//#include "All_PID.h"
//#include <stdio.h>
//#include "referee_st.h"
//Ramp chassis_x_ramp;			//����xб��
//Ramp chassis_y_ramp;			//����yб��
//Ramp chassis_yaw_ramp;			//����yawб��
//
///*******�ⲿ����*******/
//extern chassis_motor_back Motor_Whell_Info[4];
//extern pid_type_def CHASSIS_WHELL_PID_STRUCT[4];
//extern gimbal_motor_back Motor_Yaw_Info;
//extern RC_ctrl_t rc_ctrl;
//extern uint16_t Chassis_powerlim2_Sp;
////extern ext_robot_state_t             game_robot_state;                    // 0x0201  ����������״̬
//extern Game_info game_info;
//extern float Power,Power_Buffer;
//extern volatile float PITCH_VISION,YAW_VISION;
//extern float Pitch_angle;
//extern volatile float GYRO_YAW_ANGLE_GET_X;
//extern float Motor_Yaw_set,Motor_Pitch_set;
//extern updown_chassisSet UDC_Set;
///******************/
//float vx_temp=0,vy_temp=0;
//float chassis_spd[4],v[4];
//float Chassis_pidout_max;
//double Chassis_pidout;
//static double Scaling1,Scaling2,Scaling3,Scaling4;
//float Klimit=1;
//float Plimit=0;
//static uint8_t OPEN_POWER_LIM=0;
//float vx,vy,vyaw;
//
//
//
//extern float kalmanrpm;
//extern volatile float GYRO_YAW_ANGLE_GET_X;
//extern int ZGyroData;
//extern pid_type_def CHASSIS_FOLLOW_STRUCT;
//
//
////void chassis_init(void)
////{
////	 Chassis_Whell_PID_Init();
////   Chassis_Follow_PID_Init();
////
////}
//
//
//void chassis_3508_spd(void)
//{
//#if CHASSIS_OPEN_FLAG
//chassis_spd[0] = UDC_Set.motor0set;
//chassis_spd[1] = UDC_Set.motor1set;
//chassis_spd[2] = UDC_Set.motor2set;
//chassis_spd[3] = UDC_Set.motor3set;
////	uint8_t t=0;
////	float chassis_spd[4],v[4];
//
//	if(OPEN_POWER_LIM)
//	{
//		Chassis_Power_Limit();
//	}else
//	{
//		for(int i=0;i<4;i++)//PID����
//			PID_calc(&CHASSIS_WHELL_PID_STRUCT[i],Motor_Whell_Info[i].rpm,chassis_spd[i]);
//	}
//
//
//	CAN2_cmd_(CHASSIS_WHELL_PID_STRUCT[0].out,CHASSIS_WHELL_PID_STRUCT[1].out,CHASSIS_WHELL_PID_STRUCT[2].out,CHASSIS_WHELL_PID_STRUCT[3].out);
//	//printf("%f,%f,%f,%f\n",PITCH_VISION,Motor_Pitch_set,Pitch_angle,kalmanrpm);
//	//printf("%f,%f,%f\n",YAW_VISION,GYRO_YAW_ANGLE_GET_X,Motor_Yaw_set);
//#endif
//}
////ң�������ݽ��պͽ���
////void Chassis_Ctrl(void)
////{
////	fp32 delta_angle = 0.0f;
//////	if(rc_ctrl.rc.s[1]==3)
//////	{
//////
//////	}
////	if(rc_ctrl.rc.s[1]==3)
////		{
////
////			vyaw=-Chassis_Follow();
//////			if(rc_ctrl.rc.s[1]>=3)
//////				vyaw=1.5;
//
////			vx = 	(float)(rc_ctrl.rc.ch[3])
////						* ( CHASSIS_VX_MAX / 660.0f)
////						* cos(  (float)(Motor_Yaw_Info.vis_angle)/8192.0f * PI*2.0f)
////
////						- (float)(rc_ctrl.rc.ch[2])
////						* ( CHASSIS_VX_MAX / 660.0f)
////						* sin( -(float)(Motor_Yaw_Info.vis_angle)/8192.0f * PI*2.0f)
////						;
////
////			vy =  - (float)(rc_ctrl.rc.ch[3])
//
////						* ( CHASSIS_VX_MAX / 660.0f)
////						* sin(  (float)(Motor_Yaw_Info.vis_angle)/8192.0f * PI*2.0f)
////
////						- (float)(rc_ctrl.rc.ch[2])
////						* ( CHASSIS_VX_MAX / 660.0f)
////						* cos( -(float)(Motor_Yaw_Info.vis_angle)/8192.0f * PI*2.0f)
////						;
//
////		}
////		else
////		vx=vy=vyaw=0;
////		//Chassis_Ctrl_k();
////
////}
//
////float Chassis_Follow( void)
////{
////	if( Motor_Yaw_Info.vis_angle > CHASSIS_FOLLOW_ANGLE_ALLOWANCE || Motor_Yaw_Info.vis_angle < -CHASSIS_FOLLOW_ANGLE_ALLOWANCE)		//3395
////	{
////		while(ABS(Motor_Yaw_Info.vis_angle)>8192)
////		{
////		if(Motor_Yaw_Info.vis_angle<0)
////				Motor_Yaw_Info.vis_angle+=8192;
////		if(Motor_Yaw_Info.vis_angle>8192)
////				Motor_Yaw_Info.vis_angle-=8192;
////		}
////		if(rc_ctrl.rc.s[1]==3)
////		{
////			return PID_calc(&CHASSIS_FOLLOW_STRUCT,Motor_Yaw_Info.vis_angle,0);
////		}
////	}
////	return 0.0f;
////}
//
//
//
////void Chassis_Ctrl_k()
////{
////
////	static float CHASSIS_VX_KEY_MAX=0.9,CHASSIS_VY_KEY_MAX=0.9,CHASSIS_VYAW_KEY_MAX=0.6;    //������幦�ʲ���ʱ�����Ը�С���ֵ
////	static int x_R=0,flag_R=0;
//////	if(RC_Key_Read(RC_KEY_X)==1)
//////	{
//////		pitch_duandian=0;
//////	}
//////	else
//////		pitch_duandian=1;
////	if(RC_Key_Read(RC_KEY_SHIFT)==1)
////	{
////		CHASSIS_VX_KEY_MAX=4;
////		CHASSIS_VY_KEY_MAX=4;
////		CHASSIS_VYAW_KEY_MAX=4;//
//////		CHASSIS_VX_KEY_MAX+=1.3;
//////		CHASSIS_VY_KEY_MAX+=1.3;
//////		CHASSIS_VYAW_KEY_MAX+=1.3;//
////	}
////	else
////	{
////			if(Chassis_powerlim2_Sp==7000)
////		{
////			CHASSIS_VX_KEY_MAX=1.9;
////			CHASSIS_VY_KEY_MAX=1.9;
////			CHASSIS_VYAW_KEY_MAX=2.0;
////		}
////		if(Chassis_powerlim2_Sp==7500)
////		{
////			CHASSIS_VX_KEY_MAX=2.0;
////			CHASSIS_VY_KEY_MAX=2.0;
////			CHASSIS_VYAW_KEY_MAX=2.3;
////		}
////			if(Chassis_powerlim2_Sp==8000)
////		{
////			CHASSIS_VX_KEY_MAX=2.3;
////			CHASSIS_VY_KEY_MAX=2.3;
////			CHASSIS_VYAW_KEY_MAX=2.6;
////		}
////			if(Chassis_powerlim2_Sp==8500)
////		{
////		CHASSIS_VX_KEY_MAX=2.4;
////		CHASSIS_VY_KEY_MAX=2.4;
////		CHASSIS_VYAW_KEY_MAX=2.4;
////		}
////			if(Chassis_powerlim2_Sp==9000)
////		{
////		CHASSIS_VX_KEY_MAX=2.7;
////		CHASSIS_VY_KEY_MAX=2.7;
////		CHASSIS_VYAW_KEY_MAX=2.7;
////		}
////			if(Chassis_powerlim2_Sp==9500)
////		{
////		CHASSIS_VX_KEY_MAX=3.0;
////		CHASSIS_VY_KEY_MAX=3.0;
////		CHASSIS_VYAW_KEY_MAX=3.0;
////		}
////			if(Chassis_powerlim2_Sp==10000)
////		{
////		CHASSIS_VX_KEY_MAX=3.0;
////		CHASSIS_VY_KEY_MAX=3.0;
////		CHASSIS_VYAW_KEY_MAX=3.0;
////		}
////			if(Chassis_powerlim2_Sp==10500)
////		{
////		CHASSIS_VX_KEY_MAX=3.1;
////		CHASSIS_VY_KEY_MAX=3.1;
////		CHASSIS_VYAW_KEY_MAX=3.1;
////		}
////			if(Chassis_powerlim2_Sp==11000)
////		{
////		CHASSIS_VX_KEY_MAX=3.2;
////		CHASSIS_VY_KEY_MAX=3.2;
////		CHASSIS_VYAW_KEY_MAX=3.2;
////		}
////			if(Chassis_powerlim2_Sp==12000)
////		{
////		CHASSIS_VX_KEY_MAX=3.2;
////		CHASSIS_VY_KEY_MAX=3.2;
////		CHASSIS_VYAW_KEY_MAX=3.2;
////		}
//////			if(Chassis_powerlim2_Sp>=9500)
//////		{
//////  	CHASSIS_VX_KEY_MAX=3.3;
//////		CHASSIS_VY_KEY_MAX=3.3;
//////		CHASSIS_VYAW_KEY_MAX=3.3;
//////	}
////	}
//
////	//w���¶���
////	if( RC_Key_Action_Read( RC_KEY_W, RC_ACTION_KEY_PRESS) == 1)
////	{
////		Ramp_Set( &chassis_x_ramp);
////	}
////	//s���¶���
////	else if( RC_Key_Action_Read( RC_KEY_S, RC_ACTION_KEY_PRESS) == 1)
////	{
////		Ramp_Set( &chassis_x_ramp);
////	}
////	//a���¶���
////	if( RC_Key_Action_Read( RC_KEY_A, RC_ACTION_KEY_PRESS) == 1)
////	{
////		Ramp_Set( &chassis_y_ramp);
////	}
////	//d���¶���
////	else if( RC_Key_Action_Read( RC_KEY_D, RC_ACTION_KEY_PRESS) == 1)
////	{
////		Ramp_Set( &chassis_y_ramp);
////	}
////
//
////	//w����
////	if( RC_Key_Read( RC_KEY_W) == 1 && vx_temp >= 0.0f)
////	{
////		vx_temp = CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 500, RAMP_UP);   //�ı���ֵ�����Ըı���������ٶ�
////	}//s����
////	else if( RC_Key_Read( RC_KEY_S) == 1 && vx_temp <= 0.0f)
////	{
////		vx_temp = -CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 500, RAMP_UP);
////	}
////	//wş��
////	if( RC_Key_Read( RC_KEY_W) == 0 || RC_Key_Read( RC_KEY_S) == 0)
////	{
////		if( vx_temp > 0.0f)
////			vx_temp = CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 500, RAMP_DOWN);
////		if( vx_temp <0.0f)
////			vx_temp =- CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 500, RAMP_DOWN);
////	}
//
////	//D����
////	if( RC_Key_Read( RC_KEY_D) == 1 && vy_temp >= 0.0f)
////	{
////		vy_temp = CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 500, RAMP_UP);
////	}//A����
////	else if( RC_Key_Read( RC_KEY_A) == 1 && vy_temp <= 0.0f)
////	{
////		vy_temp = -CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 500, RAMP_UP);
////	}
////	//aḑ��
////	if( RC_Key_Read( RC_KEY_A) == 0 || RC_Key_Read( RC_KEY_D) == 0)
////	{
////		if( vy_temp > 0.0f)
////			vy_temp = CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 500, RAMP_DOWN);
////		if( vy_temp < 0.0f)
////			vy_temp = -CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 500, RAMP_DOWN);
////	}
////
////	vx =   vx_temp * cos( (float)(Motor_Yaw_Info.vis_angle)/8192.0f * PI*2.0f)
////		   - vy_temp * sin( -(float)(Motor_Yaw_Info.vis_angle)/8192.0f * PI*2.0f);
////
////	vy = - vx_temp * sin( (float)(Motor_Yaw_Info.vis_angle )/8192.0f * PI*2.0f)
////		   - vy_temp * cos( -(float)(Motor_Yaw_Info.vis_angle)/8192.0f * PI*2.0f);
////
//////		vx = - vx_temp * cos( (float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f) + vy_temp * sin( -(float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f);
//////
//////	vy = vx_temp * sin( (float)(gimbal_motor_info[0].angle )/8192.0f * PI*2.0f) -vy_temp * cos( -(float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f);
////
////			;
//
//
////}
//
//void Chassis_Power_Limit(void)
//{
//	get_chassis_power_and_buffer(&Power, &Power_Buffer);
//
//		Chassis_pidout_max=3267;//1536;//32768��40��960
//		for(int i=0;i<4;i++)//PID����
//		v[i]=PID_calc(&CHASSIS_WHELL_PID_STRUCT[i],Motor_Whell_Info[i].rpm,chassis_spd[i]);
//		Chassis_pidout=(
//						ABS(CHASSIS_WHELL_PID_STRUCT[0].set-CHASSIS_WHELL_PID_STRUCT[0].fdb)+
//						ABS(CHASSIS_WHELL_PID_STRUCT[1].set-CHASSIS_WHELL_PID_STRUCT[1].fdb)+
//						ABS(CHASSIS_WHELL_PID_STRUCT[2].set-CHASSIS_WHELL_PID_STRUCT[2].fdb)+
//						ABS(CHASSIS_WHELL_PID_STRUCT[3].set-CHASSIS_WHELL_PID_STRUCT[3].fdb));
//
//		Scaling1=(CHASSIS_WHELL_PID_STRUCT[0].set-CHASSIS_WHELL_PID_STRUCT[0].fdb)/Chassis_pidout;
//		Scaling2=(CHASSIS_WHELL_PID_STRUCT[1].set-CHASSIS_WHELL_PID_STRUCT[1].fdb)/Chassis_pidout;
//		Scaling3=(CHASSIS_WHELL_PID_STRUCT[2].set-CHASSIS_WHELL_PID_STRUCT[2].fdb)/Chassis_pidout;
//		Scaling4=(CHASSIS_WHELL_PID_STRUCT[3].set-CHASSIS_WHELL_PID_STRUCT[3].fdb)/Chassis_pidout;
//		Klimit=Chassis_pidout/1500;
//		if(Power_Buffer<50&&Power_Buffer>=40)	Plimit=0.8;//15
//		else if(Power_Buffer<40&&Power_Buffer>=35)	Plimit=0.6;
//		else if(Power_Buffer<35&&Power_Buffer>=30)	Plimit=0.45;
//		else if(Power_Buffer<30&&Power_Buffer>=20)	Plimit=0.25;
//		else if(Power_Buffer<20&&Power_Buffer>=10)	Plimit=0.125;
//		else if(Power_Buffer<10&&Power_Buffer>=0)	Plimit=0.05;
//		else if(Power_Buffer==60)					Plimit=1;
//		 CHASSIS_WHELL_PID_STRUCT[0].out=Scaling1*Chassis_pidout_max*Klimit*Plimit;
//	   CHASSIS_WHELL_PID_STRUCT[1].out=Scaling2*Chassis_pidout_max*Klimit*Plimit;
//     CHASSIS_WHELL_PID_STRUCT[2].out=Scaling3*Chassis_pidout_max*Klimit*Plimit;
//     CHASSIS_WHELL_PID_STRUCT[3].out=Scaling4*Chassis_pidout_max*Klimit*Plimit;
//
//
//}
