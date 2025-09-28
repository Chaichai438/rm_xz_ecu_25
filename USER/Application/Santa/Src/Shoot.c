#include "Shoot.h"
#include "remote_control.h"
#include "Motor_ALL.h"
#include "All_PID.h"
#include "pid.h"
#include "CAN_receive.h"
/*****
 * 
 * 外部变量
 * 
 */
extern RC_ctrl_t rc_ctrl;
extern pid_type_def GIMBAL_FRIC_STRUCT[2];
extern Raw_Motor_Back Motor_Fric_Info[2];
extern float pitchspeed_pidout;
/***
 * 
 * 全局变量
 * 
 */
unsigned char Start_Flag=1;
float Fric1_out=0;
float Fric2_out=0;
float Rpm_Set = 9000;

void sc_Shoot_Init(void)
{

   Gimbal_Fric_PID_Init();
}

void Shoot_ctrl(void)
{
#if OPENSHOOT

//if(rc_ctrl.rc.s[1]==1&&Start_Flag==1)
	if((rc_ctrl.rc.s[0]==3||rc_ctrl.mouse.press_r==1)&&Start_Flag==1)
    {
		Fric1_out = PID_calc(&GIMBAL_FRIC_STRUCT[0],Motor_Fric_Info[0].rpm,-Rpm_Set);
		Fric2_out = PID_calc(&GIMBAL_FRIC_STRUCT[1],Motor_Fric_Info[1].rpm,Rpm_Set);
	}else if (rc_ctrl.rc.s[0]==1)
		{
		Fric1_out = PID_calc(&GIMBAL_FRIC_STRUCT[0],Motor_Fric_Info[0].rpm+5000,-Rpm_Set);
		Fric2_out = PID_calc(&GIMBAL_FRIC_STRUCT[1],Motor_Fric_Info[1].rpm-5000,Rpm_Set);
		}
		else if(rc_ctrl.rc.s[0]==2||Start_Flag==0)
	{
		Fric1_out = PID_calc(&GIMBAL_FRIC_STRUCT[0],Motor_Fric_Info[0].rpm,0);
		Fric2_out = PID_calc(&GIMBAL_FRIC_STRUCT[1],Motor_Fric_Info[1].rpm,0);
	}
	CAN2_cmd_(Fric1_out,Fric2_out,pitchspeed_pidout,0);
#endif		
}
