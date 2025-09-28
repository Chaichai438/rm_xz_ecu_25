/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       gimbal_task.c/h
  * @brief      gimbal control task, because use the euler angle calculated by
  *             gyro sensor, range (-pi,pi), angle set-point must be in this 
  *             range.gimbal has two control mode, gyro mode and enconde mode
  *             gyro mode: use euler angle to control, encond mode: use enconde
  *             angle to control. and has some special mode:cali mode, motionless
  *             mode.
  *             完成云台控制任务，由于云台使用陀螺仪解算出的角度，其范围在（-pi,pi）
  *             故而设置目标角度均为范围，存在许多对角度计算的函数。云台主要分为2种
  *             状态，陀螺仪控制状态是利用板载陀螺仪解算的姿态角进行控制，编码器控制
  *             状态是通过电机反馈的编码值控制的校准，此外还有校准状态，停止状态等。
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *  V1.1.0     Nov-11-2019     RM              1. add some annotation
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "gimbal_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "arm_math.h"
#include "user_lib.h"
#include "detect_task.h"
#include "remote_control.h"
#include "INS_task.h"
#include "shoot.h"
#include "pid.h"
#include "gimbal_ctrl.h"
#include "ammunition_feed_ctrl.h"
#include "bsp_fric.h"
#include "gimbal.h"
#include "referee_st.h"
#include "shoot_fric.h"
#include "bsp_laser.h"

//motor enconde value format, range[0-8191]
//电机编码值规整 0—8191
#define ecd_format(ecd)         \
    {                           \
        if ((ecd) > ECD_RANGE)  \
            (ecd) -= ECD_RANGE; \
        else if ((ecd) < 0)     \
            (ecd) += ECD_RANGE; \
    }

#define gimbal_total_pid_clear(gimbal_clear)                                                   \
    {                                                                                          \
        gimbal_PID_clear(&(gimbal_clear)->gimbal_yaw_motor.gimbal_motor_absolute_angle_pid);   \
        gimbal_PID_clear(&(gimbal_clear)->gimbal_yaw_motor.gimbal_motor_relative_angle_pid);   \
        PID_clear(&(gimbal_clear)->gimbal_yaw_motor.gimbal_motor_gyro_pid);                    \
                                                                                               \
        gimbal_PID_clear(&(gimbal_clear)->gimbal_pitch_motor.gimbal_motor_absolute_angle_pid); \
        gimbal_PID_clear(&(gimbal_clear)->gimbal_pitch_motor.gimbal_motor_relative_angle_pid); \
        PID_clear(&(gimbal_clear)->gimbal_pitch_motor.gimbal_motor_gyro_pid);                  \
    }

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t gimbal_high_water;
#endif


/**
  * @brief          gimbal task, osDelay GIMBAL_CONTROL_TIME (1ms) 
  * @param[in]      pvParameters: null
  * @retval         none
  */
/**
  * @brief          云台任务，间隔 GIMBAL_CONTROL_TIME 1ms
  * @param[in]      pvParameters: 空
  * @retval         none
  */

void gimbal_task(void const *pvParameters)
{
		extern Game_info game_info;
    //等待陀螺仪任务更新陀螺仪数据
    //wait a time
    vTaskDelay(GIMBAL_TASK_INIT_TIME);
	  DM4310_Init();
    DM4310_Enable();
    //gimbal init
    //云台初始化
//		 uint8_t enable_cmd[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC};
//    for(int i=0; i<5; i++) {
//        CAN1_Send_Msg(0x201, enable_cmd, 8); // ID=0x201 (0x200+1)
//        vTaskDelay(10);
//    }

		Gimbal_Init();
    //shoot init
		Ammunition_Init();
		Shoot_fric_pid_init();
    //wait for all motor online
    //判断电机是否都上线
//    while (toe_is_error(YAW_GIMBAL_MOTOR_TOE) || toe_is_error(PITCH_GIMBAL_MOTOR_TOE))
//    {
//        vTaskDelay(GIMBAL_CONTROL_TIME);
//        //gimbal_feedback_update(&gimbal_control);             //云台数据反馈
//    }

    while (1)
    {

			Ammunition_Ctrl();
			Gimbal_Ctrl();
			laser_on(); 					 //红外线

#if YAW_TURN
        yaw_can_set_current = -gimbal_control.gimbal_yaw_motor.given_current;
#else
        //yaw_can_set_current = gimbal_control.gimbal_yaw_motor.given_current;
#endif

#if PITCH_TURN
        //pitch_can_set_current = -gimbal_control.gimbal_pitch_motor.given_current;
#else
        pitch_can_set_current = gimbal_control.gimbal_pitch_motor.given_current;
#endif

//        if (!(toe_is_error(YAW_GIMBAL_MOTOR_TOE) && toe_is_error(PITCH_GIMBAL_MOTOR_TOE) && toe_is_error(TRIGGER_MOTOR_TOE)))
//        {
//            if (toe_is_error(DBUS_TOE))
//            {
//                CAN_cmd_gimbal(0, 0, 0, 0);
//            }
//            else
//            {
//               // CAN_cmd_gimbal(yaw_can_set_current, pitch_can_set_current, shoot_can_set_current, 0);
//            }
//        }

#if GIMBAL_TEST_MODE
        J_scope_gimbal_test();
#endif

        vTaskDelay(GIMBAL_CONTROL_TIME);

#if INCLUDE_uxTaskGetStackHighWaterMark
        gimbal_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}


