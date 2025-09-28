//#include "6020.h"
//#include "remote_control.h"
//#include "pid.h"
//#include "CAN_receive.h"
//#include "gimbal_task.h"
//#include "gyro.h"
//#include "mocalun.h"
////#include "gimbal.h"
//#include "mocalun.h"
//#include "Motor_ALL.h"
//#include "All_PID.h"
//extern pid_type_def GIMBAL_PITCH_SPD_STRUCT;
//extern pid_type_def GIMBAL_YAW_PID_POS_STRUCT;
//extern pid_type_def GIMBAL_YAW_PID_SPD_STRUCT;
//extern gimbal_motor_back Motor_Pitch_Info;
//extern gimbal_motor_back Motor_Yaw_Info;

//fp32 M6020_set_yaw_speed,M6020_set_yaw_ecd,M6020_set_pitch_speed,M6020_set_pitch_ecd;
//pid_type_def motor_pid6020_yaw_spd,motor_pid6020_yaw_ecd,GYRO_yaw_spd,GYRO_yaw_ecd,GYRO_pitch_spd,GYRO_pitch_ecd;            //声明PID数据结构体
//const motor_measure_t *motor_date6020_yaw,*motor_date6020_pitch;   //声明电机结构体指针需要修改

//extern volatile float GYRO_YAW_ANGLE_GET_X,GYRO_PITCH_ANGLE_GET_X,GYRO_G_Z_XHH,GYRO_G_X_XHH;//陀螺仪传回的数据
//extern RC_ctrl_t rc_ctrl;
//float angel_set_yaw=0,angel_set_pitch=0;//遥控器映射值->陀螺仪变换值
//float angel_last_set_yaw=0,angel_last_set_pitch=0;
//float yaw_angle,pitch_angle;       			//遥控器映射值->陀螺仪变换值
//float set_2006=0;
//int mood=0;
//float yaw_change,pitch_change;//遥控器控制的变化量
//#define rc_deadband_limit(input, output, dealine)        \
//    {                                                    \
//        if ((input) > (dealine) || (input) < -(dealine)) \
//        {                                                \
//            (output) = (input);                          \
//        }                                                \
//        else                                             \
//        {                                                \
//            (output) = 0;                                \
//        }                                                \
//    }

//void YAW_6020_PID_init(void)   //pid初始化
//{
//    Gimbal_Pitch_PID_Init();
//    Gimbal_Yaw_PID_Init();
//	pitch_angle=gimbal_motor_info[1].real_angle;
//	yaw_angle=gimbal_motor_info[0].real_angle;
//}
//void gimbal_6020(fp32 ecd6020_set_yaw,fp32 ecd6020_set_pitch)//6020电机
//{ 
//		M6020_set_yaw_ecd=PID_calc(&GYRO_yaw_ecd,GYRO_YAW_ANGLE_GET_X,ecd6020_set_yaw/20.0);
//		M6020_set_yaw_speed=PID_calc(&GYRO_yaw_spd,GYRO_G_Z_XHH,M6020_set_yaw_ecd);		
//		M6020_set_pitch_ecd=PID_calc(&GYRO_pitch_ecd,GYRO_PITCH_ANGLE_GET_X,ecd6020_set_pitch/20);
//		M6020_set_pitch_speed=PID_calc(&GYRO_pitch_spd,GYRO_G_X_XHH,M6020_set_pitch_ecd);	
////		set_2006=bd(-1000);
//		CAN_cmd_gimbal(M6020_set_yaw_speed,M6020_set_pitch_speed,set_2006,0);

//}


//void gimbal_ctrl(void)   //遥控器控制6020
//{
//	static int diff;
//	yaw_change 	= ((float)rc_ctrl.rc.ch[0]) * ( REMOTE / 660.0f);	
//	pitch_change = ((float)rc_ctrl.rc.ch[1]) * ( REMOTE / 660.0f);	
//	if(yaw_change>2.0f)yaw_change=2.0f;
//	if(yaw_change<-2.0f)yaw_change=-2.0f;
//	if(pitch_change>4.0f)pitch_change=4.0f;
//	if(pitch_change<-4.0f)pitch_change=-4.0f;
//	angel_set_yaw += yaw_change;
//	angel_set_pitch += pitch_change;
//	if(angel_set_pitch<1090)angel_set_pitch=1090;
//	if(angel_set_pitch>8000)angel_set_pitch=8000;
////	angel_set_yaw=0;
//	gimbal_6020(angel_set_yaw,angel_set_pitch);
//	
//}

