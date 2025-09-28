#include "can.h"
#include "remote_control.h"
#include "gyro.h"
#include "struct_typedef.h"
#include "Motor_ALL.h"
#include "All_PID.h"
#include "INS_task.h"
#include "CAN_receive.h"
#include "gimbal.h"
#include <stdio.h>
#include "usart.h"
#include "math.h"
#include "BoDan.h"
#include "Forward_ctrl.h"
#include "FFDinit.h"
#include "st_version.h"
#include "ramp.h"
/*************
 * 
 * �ⲿ��������
 * 
 */
extern volatile float GYRO_YAW_ANGLE_GET_X,GYRO_PITCH_ANGLE_GET_X,GYRO_G_Z_XHH,GYRO_G_X_XHH;
extern gimbal_motor_back Motor_Pitch_Info;
extern gimbal_motor_back Motor_Yaw_Info;
extern fp32 INS_angle[3];
extern RC_ctrl_t rc_ctrl;
extern pid_type_def GIMBAL_YAW_PID_POS_STRUCT;
extern pid_type_def GIMBAL_YAW_PID_SPD_STRUCT;
extern pid_type_def GIMBAL_PITCH_SPD_STRUCT;
extern pid_type_def GIMBAL_PITCH_POS_STRUCT;
extern float Bodanspd_out;
extern float vis_set;
extern float kalman_Pitch_G;
extern gimbal_motor_back Motor_Feed_Info;
extern FORWARD_CTRL_T YawSpdFFD_t;
extern pid_type_def GIMBAL_FRIC_STRUCT[2];
extern Raw_Motor_Back Motor_Fric_Info[2];
extern volatile float PITCH_VISION,YAW_VISION;
extern pid_type_def LowThroughFliter;
/********
 * 
 * ȫ�ֱ�������
 * 
 */
float Motor_Yaw_set=0,Motor_Pitch_set=0;
float Motor_Yaw_change=0,Motor_Pitch_change=0;
float yawpos_pidout,yawspeed_pidout;
float pitchpos_pidout,pitchspeed_pidout;
float Pitch_angle;
KFP KFPrpmfliter={0.02,0,0,0,0.001,0.543};
KFP KFPyawfliter={0.02,0,0,0,0.001,0.543};
float kalmanrpm;
float keypitch=1;
float keyyaw=1200;
int okopen=0;
float YawSpdFFDout=0;
char open_vision_flagg=0;
float Pitch_LPFout=0;
char openlpfliter=0;
int distinguish=0;
int buff_auto_status = 0;
/*****
*
*��������
*
*/
void gimbal_PID_Init(void)
{
	Gimbal_Yaw_PID_Init();
	Gimbal_Pitch_PID_Init();
	YawSpdFFD_Init();
	LowThroughFliter_Init();

}

void Gimbal_Control_Func(void)
{
	/******test***********/
//	if(okopen)
//	{
//		sint+=0.02;
//	if(sint>2000000*3.14)
//		sint=0;
//	sinout=45*sin(sint);
//	Motor_Yaw_set=sinout;
//	
//	}

	/********************/
		low_pass_filter_init();
	
	if(rc_ctrl.rc.s[1]==1)
	{
		open_vision_flagg=1;
	}
	else 
				open_vision_flagg=0;
	//	Version_Data_Send();	
	if(open_vision_flagg)
	{
//		Motor_Pitch_change = ((float)rc_ctrl.rc.ch[1]) * ( 0.15 / 660.0f);	
//		Motor_Pitch_set =

		
//		Motor_Yaw_set 	=	 YAW_VISION;
		Motor_Yaw_set = kalmanFilter(&KFPyawfliter,YAW_VISION);
//		Motor_Pitch_set = f_Ramp_Calc(Motor_Pitch_set,PITCH_VISION,0.5);
		
////		if(openlpfliter==1)
//		Motor_Pitch_set = low_pass_filter(PITCH_VISION);
//		else if(openlpfliter==2)
		kalmanrpm = kalmanFilter(&KFPrpmfliter,PITCH_VISION);
		Motor_Pitch_set = kalmanrpm;
//		else
//		Motor_Pitch_set =PITCH_VISION;
	}else
	{
		Motor_Yaw_change = 	-((float)rc_ctrl.rc.ch[0]) * ( 0.45 / 660.0f);	
		Motor_Yaw_set 	+=	Motor_Yaw_change;
		Motor_Pitch_change = ((float)rc_ctrl.rc.ch[1]) * ( 0.15 / 660.0f);	
		Motor_Pitch_set += Motor_Pitch_change;
	}

			Pitch_angle=INS_angle[1]/3.14159*180;
			if(Motor_Pitch_set<-19)
			Motor_Pitch_set=-19; 
		else if(Motor_Pitch_set>43)
			Motor_Pitch_set=43;
//			kalmanrpm = kalmanFilter(&KFPrpmfliter,Motor_Pitch_Info.rpm);
		/***Yaw�������ٶȻ�kpkikd***/
		yawpos_pidout		 =	PID_calc(&GIMBAL_YAW_PID_POS_STRUCT,GYRO_YAW_ANGLE_GET_X,Motor_Yaw_set);
		
//		yawpos_pidout		 =	PID_calc(&GIMBAL_YAW_PID_POS_STRUCT,-Motor_Yaw_Info.vis_angle,Motor_Yaw_set);
//	  yawspeed_pidout		 =	PID_calc(&GIMBAL_YAW_PID_SPD_STRUCT,GYRO_G_Z_XHH,yawpos_pidout);//Yaw�������ٶȻ���untest��
		
		
		/***Yaw�������ٶȻ�kpkikd***/
//		yawspeed_pidout		 =	PID_calc(&GIMBAL_YAW_PID_SPD_STRUCT,Motor_Yaw_Info.rpm,Motor_Yaw_set);//�ٶȻ�����
		yawspeed_pidout		 =	PID_calc(&GIMBAL_YAW_PID_SPD_STRUCT,Motor_Yaw_Info.rpm,-yawpos_pidout);//��tested��
		YawSpdFFDout       =  keyyaw*(-yawpos_pidout);//=  FDC_calc(&YawSpdFFD_t,-yawpos_pidout);
		if(YawSpdFFDout>15000)
			YawSpdFFDout=15000;
		else if(YawSpdFFDout<-15000)
				YawSpdFFDout=-15000;		
		
//		if(yawspeed_pidout>650) 
//			yawspeed_pidout+=1500;
//		else if(yawspeed_pidout<-650)
//			yawspeed_pidout-=1500;			
		
		/***PItch������λ�û�***/
		pitchpos_pidout  	= PID_calc(&GIMBAL_PITCH_POS_STRUCT,Pitch_angle,Motor_Pitch_set);
		
		/***PItch�������ٶȻ�***/		
			pitchspeed_pidout = PID_calc(&GIMBAL_PITCH_SPD_STRUCT,Motor_Pitch_Info.rpm,-pitchpos_pidout);
		//pitchspeed_pidout = PID_calc(&GIMBAL_PITCH_SPD_STRUCT,Motor_Pitch_Info.rpm,Motor_Pitch_set);
		CAN_cmd_gimbal(yawspeed_pidout+YawSpdFFDout,0,Bodanspd_out,0);
		
//CAN_cmd_gimbal(0,0,Bodanspd_out,0);


//printf("%f,%f,%d,%d\n",Motor_Fric_Info[0].rpm,Motor_Fric_Info[1].rpm,Motor_Fric_Info[0].real_current,Motor_Fric_Info[1].real_current);


}


