#include "BoDan.h"
#include "remote_control.h"
#include "Motor_ALL.h"
#include "All_PID.h"
#include "pid.h"
#include <stdio.h>
/*****
 * 
 * 外部变量
 * 
 */
extern RC_ctrl_t rc_ctrl;
extern pid_type_def GIMBAL_FEED_PID_POS_STRUCT;
extern pid_type_def GIMBAL_FEED_PID_SPD_STRUCT;
extern gimbal_motor_back Motor_Feed_Info;
/***
 * 
 * 全局变量
 * 
 */
unsigned char ONE_SHOOT_FLAG=1;
unsigned char ROUND_BUFFnum=0;
const int ROUND_BUFF[6]={26320,26320,26060,26320,26320,26060};
float vis_set=0,lastvis_set=0;
float Bodanpos_out=0;
float Bodanspd_out=0;

void BoDan_Init(void)
{
    Gimbal_Feed_PID_Init();
}

void BoDan_ctrl(void)
{
//if(rc_ctrl.rc.s[0]==3||(rc_ctrl.mouse.press_l != 1))
	if(rc_ctrl.rc.s[0]==3)
	ONE_SHOOT_FLAG=1;
//if((rc_ctrl.rc.s[0]==2||rc_ctrl.mouse.press_l == 1)&&ONE_SHOOT_FLAG==1)
//    {
//		//vis_set+=ROUND_BUFF[ROUND_BUFFnum];//25120;//
//			vis_set+=26230;
//		++ROUND_BUFFnum;
//		if(ROUND_BUFFnum>5)
//		ROUND_BUFFnum=0;
//		ONE_SHOOT_FLAG=0;
//	}else 
//if((rc_ctrl.rc.s[0]==1||rc_ctrl.mouse.press_l == 1)&&ONE_SHOOT_FLAG==1)
	if(rc_ctrl.rc.s[0]==1&&ONE_SHOOT_FLAG==1)
	{
		//vis_set-=ROUND_BUFF[ROUND_BUFFnum];//25120;
			vis_set-=26230;
		--ROUND_BUFFnum;
		if(ROUND_BUFFnum<0)
		ROUND_BUFFnum=5;
		ONE_SHOOT_FLAG=0;
	}

	if(vis_set-lastvis_set>30000)
		vis_set=lastvis_set+26230;
	else if(vis_set-lastvis_set<-30000)
		vis_set=lastvis_set-26230;
//	if(vis_set-Motor_Feed_Info.vis_angle>2*26230)
		Bodanpos_out=PID_calc(&GIMBAL_FEED_PID_POS_STRUCT,Motor_Feed_Info.vis_angle,vis_set);
		Bodanspd_out=PID_calc(&GIMBAL_FEED_PID_SPD_STRUCT,Motor_Feed_Info.rpm,Bodanpos_out);
	lastvis_set=vis_set;	
//	printf("%f,%f\n",Motor_Feed_Info.vis_angle,vis_set);
}

void ammo_stuck(void)
{
	static uint8_t ammo_dectcnt;
	static float detect_vis_set[4],detect_visangle[4];
	ammo_dectcnt++;
	if(ammo_dectcnt>99)//每100ms触发一次
	{
			ammo_dectcnt=0;
	detect_visangle[3]=detect_visangle[2];//更新数值
	detect_visangle[2]=detect_visangle[1];
	detect_visangle[1]=detect_visangle[0];
	detect_visangle[0]=Motor_Feed_Info.vis_angle;
		
		
	detect_vis_set[1]=detect_vis_set[0];
	detect_vis_set[0]=vis_set;
		if( (detect_abs(detect_visangle[0]-detect_visangle[1])<CHANGESTACKALLOWANCE)&&
				(detect_abs(detect_visangle[1]-detect_visangle[2])<CHANGESTACKALLOWANCE)&&
				(detect_abs(detect_visangle[2]-detect_visangle[3])<CHANGESTACKALLOWANCE) )
			{
				if( (detect_abs(detect_vis_set[0]-detect_visangle[0])>STACKALLOWANCE)&&
					  (detect_abs(detect_vis_set[1]-detect_visangle[1])>STACKALLOWANCE) )
			;
			
			
			
			
			}
		
				
	}
	
	


}

float detect_abs(float anum)
{
	if(anum>=0)
		return anum+1;
	else 
		return -anum;
}
	