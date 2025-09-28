#include "gyro.h"
//#include "BMI088.h"


float GYRO_YAW_ANGLE_C_X=0,GYRO_YAW_ANGLE_C_LAST_X=0;
float GYRO_PITCH_ANGLE_C_X=0,GYRO_PITCH_ANGLE_C_LAST_X=0;
float GYRO_YAW_XHH,GYRO_PITCH_XHH;
volatile float GYRO_YAW_ANGLE_GET_X,GYRO_PITCH_ANGLE_GET_X,GYRO_G_Z_XHH,GYRO_G_X_XHH;
float GYRO_diff_x=0,GYRO_diff_y;
int GYRO_n_xhh=0;

extern fp32 INS_angle[3];      //euler angle, unit rad.Å·À­½Ç µ¥Î» rad

fp32 GYRO_euler_angle_fp32[3];
fp32 GYRO_gyro_fp32[3];

float del=0;
int round_count=0;//+ is clock ; - is unclock
void groy_get(void)
{
	GYRO_YAW_XHH=(GYRO_euler_angle_fp32[0]/(3.1415926*2))*360.0;
	GYRO_PITCH_XHH=(GYRO_euler_angle_fp32[1]/(3.1415926*2))*360.0;
	GYRO_G_Z_XHH=GYRO_gyro_fp32[2]*100;
	GYRO_G_X_XHH=GYRO_gyro_fp32[1]*100;	


	GYRO_YAW_ANGLE_C_X=GYRO_YAW_XHH;
	if(GYRO_n_xhh>400){
		GYRO_diff_x=GYRO_YAW_ANGLE_C_X-GYRO_YAW_ANGLE_C_LAST_X;
	}
	if(GYRO_diff_x>=300)  
	{	
		round_count--;
		GYRO_diff_x=GYRO_diff_x-360;
	}
	else if (GYRO_diff_x<=-300)
	{
		round_count++;
		GYRO_diff_x=GYRO_diff_x+360;
	}
	GYRO_YAW_ANGLE_C_LAST_X=GYRO_YAW_ANGLE_C_X;
	GYRO_YAW_ANGLE_GET_X+=GYRO_diff_x;

	GYRO_PITCH_ANGLE_C_X=GYRO_PITCH_XHH;		
	if(GYRO_n_xhh>400){
		GYRO_diff_y=GYRO_PITCH_ANGLE_C_X-GYRO_PITCH_ANGLE_C_LAST_X;
	}
	if(GYRO_diff_y>=300)  {	
		GYRO_diff_y=GYRO_diff_y-360;
	}
	else if (GYRO_diff_y<=-300){
		GYRO_diff_y=GYRO_diff_y+360;
	}
	GYRO_PITCH_ANGLE_C_LAST_X=GYRO_PITCH_ANGLE_C_X;
	GYRO_PITCH_ANGLE_GET_X+=GYRO_diff_y;



	if(GYRO_n_xhh<500)GYRO_n_xhh++;
		
}



