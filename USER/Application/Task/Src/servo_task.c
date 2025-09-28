/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       servo_task.c/h
  * @brief      
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Oct-21-2019     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "servo_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "bsp_servo_pwm.h"
#include "remote_control.h"
#include "CAN_receive.h"
#include "referee_st.h"
#include "referee_usart_task_st.h"
#define SERVO_MIN_PWM   500
#define SERVO_MAX_PWM   2500

#define PWM_DETAL_VALUE 10

#define SERVO1_ADD_PWM_KEY  KEY_PRESSED_OFFSET_Z
#define SERVO2_ADD_PWM_KEY  KEY_PRESSED_OFFSET_X
#define SERVO3_ADD_PWM_KEY  KEY_PRESSED_OFFSET_C
#define SERVO4_ADD_PWM_KEY  KEY_PRESSED_OFFSET_V

#define SERVO_MINUS_PWM_KEY KEY_PRESSED_OFFSET_SHIFT

//const RC_ctrl_t *servo_rc;
//const static uint16_t servo_key[4] = {SERVO1_ADD_PWM_KEY, SERVO2_ADD_PWM_KEY, SERVO3_ADD_PWM_KEY, SERVO4_ADD_PWM_KEY};
//uint16_t servo_pwm[4] = {SERVO_MIN_PWM, SERVO_MIN_PWM, SERVO_MIN_PWM, SERVO_MIN_PWM};
extern Game_info game_info;	

/**
  * @brief          servo_task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
/**
  * @brief          舵机任务
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void servo_task(void const * argument)
{
	static uint8_t robot_color,robot_color_id;
  static uint8_t bulletSpd_one;
	static uint16_t C_Power_Lim=70;
   // servo_rc = get_remote_control_point();

    while(1)
    {
//        for(uint8_t i = 0; i < 4; i++)
//        {

//            if( (servo_rc->key.v & SERVO_MINUS_PWM_KEY) && (servo_rc->key.v & servo_key[i]))
//            {
//                servo_pwm[i] -= PWM_DETAL_VALUE;
//            }
//            else if(servo_rc->key.v & servo_key[i])
//            {
//                servo_pwm[i] += PWM_DETAL_VALUE;
//            }

//            //limit the pwm
//           //限制pwm
//            if(servo_pwm[i] < SERVO_MIN_PWM)
//            {
//                servo_pwm[i] = SERVO_MIN_PWM;
//            }
//            else if(servo_pwm[i] > SERVO_MAX_PWM)
//            {
//                servo_pwm[i] = SERVO_MAX_PWM;
//            }

//            servo_pwm_set(servo_pwm[i], i);
//        }

/******用于转发裁判系统*******/
		robot_color=game_info.game_robot_state.robot_id;
		if(game_info.game_robot_state.chassis_power_limit<200&&game_info.game_robot_state.chassis_power_limit>10)
			C_Power_Lim = game_info.game_robot_state.chassis_power_limit;
		if(robot_color==101||robot_color==104||robot_color==105)		//蓝
	{
		robot_color_id=0;
	}
		if(robot_color==1||robot_color==4||robot_color==5)					//红
	{
		robot_color_id=1;
	}
	
	bulletSpd_one = (int8_t)(game_info.shoot_data.bullet_speed);
	
	
	CAN_cmd_referee(robot_color_id,bulletSpd_one,C_Power_Lim);

        osDelay(9);
    }
}


