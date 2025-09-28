#include "gimbal_ctrl.h"
#include "gimbal.h"
#include "remote_control.h"
//#include "judge_19_data.h"
#include "chassis_ctrl.h"
#include "usart.h"
//#include "report.h"
#include "detect_task.h"

int VISION_YAW=0,VISION_PITCH=0,VISION_YAW_LAST=0,VISION_PITCH_LAST=0;
int v_yaw_change,v_pitch_change;
extern RC_ctrl_t rc_ctrl;
extern float last_pos;
extern TIM_HandleTypeDef htim1;
//int gimbal_motor_info[1].angle_set =5900;
/**
  * @brief          遥控器的死区判断，因为遥控器的拨杆在中位的时候，不一定为0，
  * @param          输入的遥控器值
  * @param          输出的死区处理后遥控器值
  * @param          死区值
  */
#define rc_deadband_limit(input, output, dealine)        \
    {                                                    \
        if ((input) > (dealine) || (input) < -(dealine)) \
        {                                                \
            (output) = (input);                          \
        }                                                \
        else                                             \
        {                                                \
            (output) = 0;                                \
        }                                                \
    }


void Gimbal_Init(void)
{
	Gimbal_pid_init();
}

int mode_channel=3,last_channel=3;
extern float last_yaw_set,last_pitch_set;
//float pitch_changes;

//void Gimbal_Ctrl(void)
//{
//	
//		float yaw_change;
//	static int i[4]={0};

//	int v_yaw_t=0,v_pitch_t=0;
//	
//    static int16_t yaw_channel = 0, pitch_channel = 0;
//	
////遥控器的死区判断，因为遥控器的拨杆在中位的时候，不一定为0，	
//    rc_deadband_limit(rc_ctrl.rc.ch[0], yaw_channel, 15);
//    rc_deadband_limit(rc_ctrl.rc.ch[1], pitch_channel, 15);
//	
//	//遥控器映射
//		pitch_changes 	= ((float)rc_ctrl.rc.ch[1]) * ( 2.0f / 660.0f);		
//		yaw_change 		= -((float)rc_ctrl.rc.ch[0]) * ( 2.0f / 660.0f);
//	if(rc_ctrl.rc.s[1] == 3)
//	{
//		pitch_changes = -rc_ctrl.mouse.y * 0.03;  //键盘控制开陀螺后云台运动
//		yaw_change   = -rc_ctrl.mouse.x * 0.08;	
//	}

//	
//	if(yaw_change>YAW_CHANGEMAX)yaw_change=YAW_CHANGEMAX;
//	if(yaw_change<-YAW_CHANGEMAX)yaw_change=-YAW_CHANGEMAX;
//	
//	gimbal_motor_info[0].angle_set += yaw_change;
////	gimbal_motor_info[1].angle_set += pitch_changes;

//	if( gimbal_motor_info[1].real_angle  > PITCH_LIMIT_DOWN && gimbal_motor_info[1].real_angle < PITCH_LIMIT_UP)
//	{
//		
//	gimbal_motor_info[1].angle_set += pitch_changes;
//		
//}


//	

////	
//	if( gimbal_motor_info[1].real_angle < PITCH_LIMIT_DOWN)
//{
//	if(pitch_changes < 0)
//		
//	gimbal_motor_info[1].angle_set += pitch_changes;
//	
//	else 
//			gimbal_motor_info[1].angle_set += 0;
//		

//}
//	if( gimbal_motor_info[1].real_angle  > PITCH_LIMIT_UP)
//{
//	if(pitch_changes > 0)
//		
//	gimbal_motor_info[1].angle_set += pitch_changes;
//	
//	else 
//			gimbal_motor_info[1].angle_set += 0;
//}	
//	if( gimbal_motor_info[1].angle  > 0 && ( gimbal_motor_info[1].angle  < 200))   //一圈8192 超过8192重新从0开始，新增限制
//	{
//		if(pitch_changes < 0)
//		gimbal_motor_info[1].angle_set += pitch_changes;
//		else 
//			gimbal_motor_info[1].angle_set += 0;
//	}
////	if(toe_is_error(DBUS_TOE))
////		Gimbal_Motor_Speed_Set(0,0);
////	else
//	Gimbal_Motor_Speed_Set( gimbal_motor_info[0].angle_set/10.0f, gimbal_motor_info[1].angle_set/10.0f);
//}


// 修改 Gimbal_Ctrl 函数
void Gimbal_Ctrl(void) {
    float yaw_change;
    static int16_t yaw_channel = 0, pitch_channel = 0;
    
    // 死区处理
    rc_deadband_limit(rc_ctrl.rc.ch[0], yaw_channel, 15);
    rc_deadband_limit(rc_ctrl.rc.ch[1], pitch_channel, 15);
    
    // Yaw控制保持不变
    yaw_change = -2.5*((float)rc_ctrl.rc.ch[0]) * (2.0f / 660.0f);
    yaw_change = constrain_float(yaw_change, -YAW_CHANGEMAX, YAW_CHANGEMAX);
    gimbal_motor_info[0].angle_set += yaw_change;
    
    // Pitch控制由Gimbal_Motor_Speed_Set函数根据模式处理
    
    // 发送控制命令
    Gimbal_Motor_Speed_Set(
        gimbal_motor_info[0].angle_set / 10.0f, // yaw设定
        0 // pitch设定不再使用，由达妙4310单独控制
    );
}
	

///**
//  * @brief          云台陀螺仪控制，电机是陀螺仪角度控制，
//  * @param[out]     yaw: yaw轴角度控制，为角度的增量 单位 rad
//  * @param[out]     pitch:pitch轴角度控制，为角度的增量 单位 rad
//  * @param[in]      gimbal_control_set:云台数据指针
//  * @retval         none
//  */
//static void gimbal_absolute_angle_control(fp32 *yaw, fp32 *pitch, gimbal_control_t *gimbal_control_set)
//{
//    if (yaw == NULL || pitch == NULL || gimbal_control_set == NULL)
//    {
//        return;
//    }

//    static int16_t yaw_channel = 0, pitch_channel = 0;

//    rc_deadband_limit(gimbal_control_set->gimbal_rc_ctrl->rc.ch[YAW_CHANNEL], yaw_channel, RC_DEADBAND);
//    rc_deadband_limit(gimbal_control_set->gimbal_rc_ctrl->rc.ch[PITCH_CHANNEL], pitch_channel, RC_DEADBAND);

//    *yaw = yaw_channel * YAW_RC_SEN - gimbal_control_set->gimbal_rc_ctrl->mouse.x * YAW_MOUSE_SEN;
//    *pitch = pitch_channel * PITCH_RC_SEN + gimbal_control_set->gimbal_rc_ctrl->mouse.y * PITCH_MOUSE_SEN;


//    {
//        static uint16_t last_turn_keyboard = 0;
//        static uint8_t gimbal_turn_flag = 0;
//        static fp32 gimbal_end_angle = 0.0f;

//        if ((gimbal_control_set->gimbal_rc_ctrl->key.v & TURN_KEYBOARD) && !(last_turn_keyboard & TURN_KEYBOARD))
//        {
//            if (gimbal_turn_flag == 0)
//            {
//                gimbal_turn_flag = 1;
//                //保存掉头的目标值
//                gimbal_end_angle = rad_format(gimbal_control_set->gimbal_yaw_motor.absolute_angle + PI);
//            }
//        }
//        last_turn_keyboard = gimbal_control_set->gimbal_rc_ctrl->key.v ;

//        if (gimbal_turn_flag)
//        {
//            //不断控制到掉头的目标值，正转，反装是随机
//            if (rad_format(gimbal_end_angle - gimbal_control_set->gimbal_yaw_motor.absolute_angle) > 0.0f)
//            {
//                *yaw += TURN_SPEED;
//            }
//            else
//            {
//                *yaw -= TURN_SPEED;
//            }
//        }
//        //到达pi （180°）后停止
//        if (gimbal_turn_flag && fabs(rad_format(gimbal_end_angle - gimbal_control_set->gimbal_yaw_motor.absolute_angle)) < 0.01f)
//        {
//            gimbal_turn_flag = 0;
//        }
//    }
//}
