#include "st_version.h"
#include "gimbal.h"
#include "usart.h"
#include "referee_st.h"
#include "remote_control.h"
u8 VERSION_DATA[VERSION_DATA_LEN];
u16 time=0; //电控收到第一组数据到发出的时间差
u8 CTRL_STATE,VERSION_STATE;
u8 ch[2]="\r\n";
extern volatile float GYRO_YAW_ANGLE_GET_X,GYRO_PITCH_ANGLE_GET_X,GYRO_G_Z_XHH,GYRO_G_X_XHH;
extern Game_info game_info;						//裁判系统数据

extern int buff_auto_status;
int bullet_speed_gyro=0;
int robot_color=0,robot_color_id=0;
extern int inforbuff;
union shijue_union pitch_shijue,yaw_shijue;
void Version_Data_Send(void)
{
	yaw_shijue.a=GYRO_YAW_ANGLE_GET_X;
	pitch_shijue.a=GYRO_PITCH_ANGLE_GET_X;
	bullet_speed_gyro=game_info.shoot_data.bullet_speed*100;
	robot_color=game_info.game_robot_state.robot_id;
		if(robot_color==103||robot_color==104||robot_color==105)		//蓝
	{
		robot_color_id=0;
	}
		if(robot_color==3||robot_color==4||robot_color==5)					//红
	{
		robot_color_id=1;
	}
	
	VERSION_DATA[0]=0x78;
	for(int i=0;i<4;i++)
		VERSION_DATA[i+1]=pitch_shijue.b[i];	
	for(int i=0;i<4;i++)
		VERSION_DATA[i+5]=yaw_shijue.b[i];
	
	VERSION_DATA[9]=(int16_t)bullet_speed_gyro>>8;
	VERSION_DATA[10]=(int16_t)bullet_speed_gyro;
	VERSION_DATA[11]=robot_color_id;
	VERSION_DATA[12]=inforbuff;		
	VERSION_DATA[13]=0x76;
	
	if(RC_Key_Read( RC_KEY_Z) == 1)
	{
			VERSION_DATA[0]=0x78;
			for(int i=0;i<12;i++)
			VERSION_DATA[i+1]=0;
			VERSION_DATA[13]=0x76;
	}
	
	HAL_UART_Transmit(&huart1, (uint8_t *)&VERSION_DATA, 14, 0xffff);///<普通串口发送数据
}