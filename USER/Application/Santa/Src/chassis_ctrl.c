#include "chassis.h"
#include "6020.h"
#include "remote_control.h"
#include "pid.h"
#include "CAN_receive.h"
#include "gimbal_task.h"
#include "gyro.h"
#include "mocalun.h"
#include "gimbal.h"
#include <math.h>
#include "ramp.h"
#include "A101.h"
#include "INS_task.h"
#include "user_lib.h"
Ramp chassis_x_ramp;			//����xб��
Ramp chassis_y_ramp;			//����yб��
Ramp chassis_yaw_ramp;			//����yawб��

extern volatile uint32_t judge_data_cnt;
extern RC_ctrl_t rc_ctrl;
float vx,vy,vyaw;
extern gimbal_motor_t gimbal_motor_info[2];
const fp32 chassis_3508_pid[3]={12.0f,0.0f,0.0f},chassis_3508_angle[3]={0.0041f,0.0f,0.01f},chassis_floaw_pid[3]={0.1,0,0}; 
pid_type_def chassis_pid[4],chassis_angle_pid,chassis_floaw;              //����PID���ݽṹ��
const motor_measure_t *chassis_3508[4];  //��������ṹ��ָ��
extern volatile float GYRO_YAW_ANGLE_GET_X;
extern int ZGyroData;
void chassis_init(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		PID_init(&chassis_pid[i],0,chassis_3508_pid,10000.0f,3000.0f);
		chassis_3508[i]=get_chassis_motor_measure_point(i);
	}
	PID_init(&chassis_angle_pid,0,chassis_3508_angle,4.0f,0.5f);
	PID_init(&chassis_floaw,0,chassis_floaw_pid,3.0f,0.5f);
}
void chassis_3508_spd(void)
{
	Chassis_Ctrl();
	uint8_t t;
	float chassis_spd[4],v[4];
	chassis_spd[3] =  vx - vy - vyaw ;			//��ǰ
	chassis_spd[2] =  vx + vy + vyaw ;			//��ǰ
	chassis_spd[1] = 	vx - vy + vyaw ;			//���
	chassis_spd[0] =  vx + vy - vyaw ;			//�Һ�
	for( t = 0; t < 4; ++t)
	{
		chassis_spd[t] = chassis_spd[t] * 2 * 19 *5;			//�ٶ�ת��Ϊת��
	}
	for(int i=0;i<4;i++)
		v[i]=PID_calc(&chassis_pid[i],chassis_3508[i]->speed_rpm,chassis_spd[i]);
	//CAN_cmd_chassis(v[3],v[2],v[1],v[0]);
}
void Chassis_Ctrl(void)
{
	fp32 delta_angle = 0.0f;
	//��������������·�
	if(rc_ctrl.rc.s[1]==2)
	{
		vx=vy=vyaw=0;
	}
	else {
		
		
	//��̨����ģʽ����м�

	//ң��������ӳ�䵽�ٶ�
	//ǰ������
	vx =  (float)(rc_ctrl.rc.ch[3]) * ( CHASSIS_VX_MAX / 660.0f) ;
	//����ƽ��		
	vy =   (float)(rc_ctrl.rc.ch[2]) * ( CHASSIS_VX_MAX / 660.0f) ;
		vyaw=(float)(rc_ctrl.rc.ch[0]) *(CHASSIS_VYAW_MAX/660.0f);
		
		
	}
	
//	if(rc_ctrl.rc.s[1]==1)
//	{
//		vyaw=1.5;
//	}
//	//ң��������ӳ�䵽�ٶ�
//	//ǰ������
//	vx =  (float)(rc_ctrl.rc.ch[3]) * ( CHASSIS_VX_MAX / 660.0f) * cos(0)
//		- (float)(rc_ctrl.rc.ch[2]) * ( CHASSIS_VX_MAX / 660.0f) * sin( 0);
//	//����ƽ��		
//	vy = -(float)(rc_ctrl.rc.ch[3]) * ( CHASSIS_VX_MAX / 660.0f) * sin( 0) 
//		+ (float)(rc_ctrl.rc.ch[2]) * ( CHASSIS_VX_MAX / 660.0f) * cos( 0);
////	delta_angle=rad_format(GYRO_YAW_ANGLE_GET_X-ZGyroData);
////	vyaw =PID_calc(&chassis_angle_pid,0.0f,delta_angle);
//	vyaw =(float)(rc_ctrl.rc.ch[0]) *(CHASSIS_VYAW_MAX/660.0f);

}
//���̣���ȫ
/**

void Chassis_Ctrl_k()
{
	static float CHASSIS_VX_KEY_MAX=1.5,CHASSIS_VY_KEY_MAX=1.5,CHASSIS_VYAW_KEY_MAX=0.6;
	static float vx_temp,vy_temp;
	
	if(RC_Key_Read(RC_KEY_SHIFT)==1)
	{
		CHASSIS_VX_KEY_MAX=3.6;
		CHASSIS_VY_KEY_MAX=3.6;
		CHASSIS_VYAW_KEY_MAX=3.0;
	}
	else
	{
		CHASSIS_VX_KEY_MAX=2.0;
		CHASSIS_VY_KEY_MAX=2.0;
		CHASSIS_VYAW_KEY_MAX=3.0;
	}
		

	//w���¶���
	if( RC_Key_Action_Read( RC_KEY_W, RC_ACTION_KEY_PRESS) == 1)
	{	
		Ramp_Set( &chassis_x_ramp);
	}//s���¶���
	else if( RC_Key_Action_Read( RC_KEY_S, RC_ACTION_KEY_PRESS) == 1)
	{
		Ramp_Set( &chassis_x_ramp);
	}
	//a���¶���
	if( RC_Key_Action_Read( RC_KEY_A, RC_ACTION_KEY_PRESS) == 1)
	{	
		Ramp_Set( &chassis_y_ramp);
	}//d���¶���
	else if( RC_Key_Action_Read( RC_KEY_D, RC_ACTION_KEY_PRESS) == 1)
	{
		Ramp_Set( &chassis_y_ramp);
	}

	//w����
	if( RC_Key_Read( RC_KEY_W) == 1 && vx_temp >= 0.0f)
	{
		vx_temp = CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 2000, RAMP_UP);
	}//s����
	else if( RC_Key_Read( RC_KEY_S) == 1 && vx_temp <= 0.0f)
	{
		vx_temp = -CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 2000, RAMP_UP);
	}
	
	//wş��
	if( RC_Key_Read( RC_KEY_W) == 0 || RC_Key_Read( RC_KEY_S) == 0)
	{
		if( vx_temp > 0.0f)
			vx_temp = CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 1000, RAMP_DOWN);
		if( vx_temp < 0.0f)
			vx_temp = -CHASSIS_VX_KEY_MAX * Ramp_Calc( &chassis_x_ramp, 1000, RAMP_DOWN);
	}
	
	//a����
	if( RC_Key_Read( RC_KEY_A) == 1 && vy_temp <= 0.0f)
	{
		vy_temp = -CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 2000, RAMP_UP);
	}//d����
	else if( RC_Key_Read( RC_KEY_D) == 1 && vy_temp >= 0.0f)
	{
		vy_temp = CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 2000, RAMP_UP);
	}
	
	//aḑ��
	if( RC_Key_Read( RC_KEY_A) == 0 || RC_Key_Read( RC_KEY_D) == 0)
	{
		if( vy_temp > 0.0f)
			vy_temp = CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 1000, RAMP_DOWN);
		if( vy_temp < 0.0f)
			vy_temp = -CHASSIS_VY_KEY_MAX * Ramp_Calc( &chassis_y_ramp, 1000, RAMP_DOWN);
	}
	

	
	vx =  vx_temp * cos( (float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f) - vy_temp * sin( -(float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f);
			
	vy = -vx_temp * sin( (float)(gimbal_motor_info[0].angle )/8192.0f * PI*2.0f) + vy_temp * cos( -(float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f);
	
	//vy =  -vy_temp * cos( (float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f) + vx_temp *  sin( -(float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f);
	//vx = +vy_temp * sin( (float)(gimbal_motor_info[0].angle )/8192.0f * PI*2.0f) - vx_temp * cos( -(float)(gimbal_motor_info[0].angle)/8192.0f * PI*2.0f);

	//vx=vx_temp;
	//vy=vy_temp;

}

**/
float Chassis_Follow( void)
{
	if( GYRO_YAW_ANGLE_GET_X-ZGyroData > CHASSIS_FOLLOW_ANGLE_ALLOWANCE || GYRO_YAW_ANGLE_GET_X-ZGyroData < -CHASSIS_FOLLOW_ANGLE_ALLOWANCE)		
	{	
		while(ABS(GYRO_YAW_ANGLE_GET_X+ZGyroData)>360)
		{
		if(GYRO_YAW_ANGLE_GET_X<ZGyroData)
				ZGyroData-=360	;
		if(GYRO_YAW_ANGLE_GET_X>ZGyroData)
				ZGyroData+=360;
		}
//		if(RC_Key_Read(RC_KEY_Q)!=1)
//		{
//			return PID_calc(&chassis_floaw,ZGyroData,-GYRO_YAW_ANGLE_GET_X);
//		}
	}
	return 0.0f;
}
