///**
//  ****************************(C) COPYRIGHT 2019 DJI****************************
//  * @file       gimbal_task.c/h
//  * @brief      gimbal control task, because use the euler angle calculated by
//  *             gyro sensor, range (-pi,pi), angle set-point must be in this
//  *             range.gimbal has two control mode, gyro mode and enconde mode
//  *             gyro mode: use euler angle to control, encond mode: use enconde
//  *             angle to control. and has some special mode:cali mode, motionless
//  *             mode.
//  *             �����̨��������������̨ʹ�������ǽ�����ĽǶȣ��䷶Χ�ڣ�-pi,pi��
//  *             �ʶ�����Ŀ��ǶȾ�Ϊ��Χ���������ԽǶȼ���ĺ�������̨��Ҫ��Ϊ2��
//  *             ״̬�������ǿ���״̬�����ð��������ǽ������̬�ǽ��п��ƣ�����������
//  *             ״̬��ͨ����������ı���ֵ���Ƶ�У׼�����⻹��У׼״̬��ֹͣ״̬�ȡ�
//  * @note
//  * @history
//  *  Version    Date            Author          Modification
//  *  V1.0.0     Dec-26-2018     RM              1. done
//  *  V1.1.0     Nov-11-2019     RM              1. add some annotation
//  *
//  @verbatim
//  ==============================================================================
//
//  ==============================================================================
//  @endverbatim
//  ****************************(C) COPYRIGHT 2019 DJI****************************
//  */
//
//#include "gimbal_task.h"
//#include "BoDan.h"
//#include "main.h"
//#include "gimbal.h"
//#include "cmsis_os.h"
////#include "6020.h"
//#include "arm_math.h"
//#include "CAN_receive.h"
//#include "user_lib.h"
//#include "detect_task.h"
//#include "remote_control.h"
//#include "INS_task.h"
//#include "shoot.h"
//#include "pid.h"
//
//
////motor enconde value format, range[0-8191]
////�������ֵ���� 0��8191
//#define ecd_format(ecd)         \
//    {                           \
//        if ((ecd) > ECD_RANGE)  \
//            (ecd) -= ECD_RANGE; \
//        else if ((ecd) < 0)     \
//            (ecd) += ECD_RANGE; \
//    }
//
//#define gimbal_total_pid_clear(gimbal_clear)                                                   \
//    {                                                                                          \
//        gimbal_PID_clear(&(gimbal_clear)->gimbal_yaw_motor.gimbal_motor_absolute_angle_pid);   \
//        gimbal_PID_clear(&(gimbal_clear)->gimbal_yaw_motor.gimbal_motor_relative_angle_pid);   \
//        PID_clear(&(gimbal_clear)->gimbal_yaw_motor.gimbal_motor_gyro_pid);                    \
//                                                                                               \
//        gimbal_PID_clear(&(gimbal_clear)->gimbal_pitch_motor.gimbal_motor_absolute_angle_pid); \
//        gimbal_PID_clear(&(gimbal_clear)->gimbal_pitch_motor.gimbal_motor_relative_angle_pid); \
//        PID_clear(&(gimbal_clear)->gimbal_pitch_motor.gimbal_motor_gyro_pid);                  \
//    }
//
//#if INCLUDE_uxTaskGetStackHighWaterMark
//uint32_t gimbal_high_water;
//#endif
//
//
///**
//  * @brief          gimbal task, osDelay GIMBAL_CONTROL_TIME (1ms)
//  * @param[in]      pvParameters: null
//  * @retval         none
//  */
///**
//  * @brief          ��̨���񣬼�� GIMBAL_CONTROL_TIME 1ms
//  * @param[in]      pvParameters: ��
//  * @retval         none
//  */
//
//void gimbal_task(void const *pvParameters)
//{
//    //�ȴ������������������������
//    //wait a time
//    vTaskDelay(GIMBAL_TASK_INIT_TIME);
//    //gimbal init
//			gimbal_PID_Init();
//    //��̨��ʼ��
//  //YAW_6020_PID_init();
//    //shoot init
//    //�����ʼ��
//			BoDan_Init();
//    //wait for all motor online
//    //�жϵ���Ƿ�����
////    while (toe_is_error(YAW_GIMBAL_MOTOR_TOE) || toe_is_error(PITCH_GIMBAL_MOTOR_TOE))
////    {
////        vTaskDelay(GIMBAL_CONTROL_TIME);
////        //gimbal_feedback_update(&gimbal_control);             //��̨���ݷ���
////    }
//
//    while (1)
//    {
//
//		//gimbal_ctrl();
//
//			BoDan_ctrl();
//			Gimbal_Control_Func();
////        shoot_can_set_current = shoot_control_loop();        //����������ѭ��
//
//
//#if YAW_TURN
//        yaw_can_set_current = -gimbal_control.gimbal_yaw_motor.given_current;
//#else
//        //yaw_can_set_current = gimbal_control.gimbal_yaw_motor.given_current;
//#endif
//
//#if PITCH_TURN
//        //pitch_can_set_current = -gimbal_control.gimbal_pitch_motor.given_current;
//#else
////        pitch_can_set_current = gimbal_control.gimbal_pitch_motor.given_current;
//#endif
//
////        if (!(toe_is_error(YAW_GIMBAL_MOTOR_TOE) && toe_is_error(PITCH_GIMBAL_MOTOR_TOE) && toe_is_error(TRIGGER_MOTOR_TOE)))
////        {
////            if (toe_is_error(DBUS_TOE))
////            {
////                CAN_cmd_gimbal(0, 0, 0, 0);
////            }
////            else
////            {
////               // CAN_cmd_gimbal(yaw_can_set_current, pitch_can_set_current, shoot_can_set_current, 0);
////            }
////        }
//
//#if GIMBAL_TEST_MODE
//        J_scope_gimbal_test();
//#endif
//
//        vTaskDelay(GIMBAL_CONTROL_TIME);
//
//#if INCLUDE_uxTaskGetStackHighWaterMark
//        gimbal_high_water = uxTaskGetStackHighWaterMark(NULL);
//#endif
//    }
//}
//
//
