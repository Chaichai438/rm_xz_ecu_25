//#include "mocalun.h"
//#include "remote_control.h"
//#include "main.h"
//#include  "tim.h"
//#include "CAN_receive.h"
//#include "bsp_fric.h"
//#include "pid.h"
//#include "CAN_receive.h"
//#include "user_lib.h"
//#include "shoot.h"
//extern RC_ctrl_t rc_ctrl;
//static fp32 pwm,temp=0;
//static int M2006_set; 
//pid_type_def motor_pid;              //声明PID数据结构体
//const motor_measure_t *motor_date;//声明电机结构体指针
//const fp32 PID[3]={10.0,0,0};

//shoot_control_t shoot_control_3508;          //射击数据

//void mcl(void)//斜坡开启摩擦轮
//{
//	if(rc_ctrl.rc.s[0]==3)
//	{
//		ramp_calc(&shoot_control_3508.fric1_ramp, SHOOT_FRIC_PWM_ADD_VALUE);
//		ramp_calc(&shoot_control_3508.fric2_ramp, SHOOT_FRIC_PWM_ADD_VALUE);
//	}
//	if(rc_ctrl.rc.s[0]==2)
//	{
//	    ramp_calc(&shoot_control_3508.fric1_ramp, -SHOOT_FRIC_PWM_ADD_VALUE);
//        ramp_calc(&shoot_control_3508.fric2_ramp, -SHOOT_FRIC_PWM_ADD_VALUE);
//	}
//	shoot_control_3508.fric_pwm1 = (uint16_t)(shoot_control_3508.fric1_ramp.out);
//    shoot_control_3508.fric_pwm2 = (uint16_t)(shoot_control_3508.fric2_ramp.out);
//	fric1_on((uint16_t)(shoot_control_3508.fric_pwm1));
//	fric2_on((uint16_t)(shoot_control_3508.fric_pwm2));
////	int i;
////	if(rc_ctrl.rc.s[0]==3 && temp<300)
////	{
//	
////		for(i=0;i<30;i++)
////		  {
////				temp+=20;
////				pwm = 1000.0f + temp;
////				fric1_on((uint16_t)(pwm));
////				fric2_on((uint16_t)(pwm));
////				HAL_Delay(100);	
////			}
////	}
////	if(rc_ctrl.rc.s[0]==2&&temp>0)
////		for(i=0;i<30;i++)
////			{
////				temp-=20;
////				pwm = 1000.0f + temp;
////				fric1_on((uint16_t)(pwm));
////				fric2_on((uint16_t)(pwm));
////				HAL_Delay(100);	
////			}
//}
//void ammunition()
//{
//	PID_init(&motor_pid,PID_POSITION,PID,10000,3000);
//	motor_date = get_trigger_motor_measure_point();
//}
//float bd(int speed_set)//拨弹2006电机
//{
//	if(rc_ctrl.rc.s[0]==1)
//		M2006_set=PID_calc(&motor_pid,motor_date->speed_rpm,speed_set);
//	if(rc_ctrl.rc.s[0]==3||rc_ctrl.rc.s[0]==2)
//		M2006_set=PID_calc(&motor_pid,motor_date->speed_rpm,0);
//	return M2006_set;
////	CAN_cmd_gimbal(0,0,M2006_set,0);
//}
////void bd(int speed_set)//拨弹2006电机
////{
////	if(rc_ctrl.rc.s[0]==1)
////	{
////		M3508_set=PID_calc(&motor_pid_edc,motor_date->angle,speed_set);
////		M3508_spd=PID_calc(&motor_pid_edc,motor_date->speed_rpm,M3508_set);
////		CAN_cmd_gimbal(0,0,M3508_spd,0);
////	}
//////	if(rc_ctrl.rc.s[0]==3||rc_ctrl.rc.s[0]==2)
//////		M2006_set=PID_calc(&motor_pid,motor_date->speed_rpm,0);
//////	return M2006_set;
////}
//void fric_off1(void)
//{
////    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 2000);
////    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 2000);
////	HAL_Delay(100);
////	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 1000);
////    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 1000);
//	ramp_init(&shoot_control_3508.fric1_ramp, SHOOT_CONTROL_TIME * 0.001f, DOWN, OFF);
//    ramp_init(&shoot_control_3508.fric2_ramp, SHOOT_CONTROL_TIME * 0.001f, DOWN, OFF);
//}
//void fanzhuan()//拨弹反转
//{
//	
//}
