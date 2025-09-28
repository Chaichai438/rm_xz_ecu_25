/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       usb_task.c/h
  * @brief      usb outputs the error message.usb输出错误信息
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Nov-11-2019     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */
#include "usb_task.h"

#include "cmsis_os.h"

#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <stdarg.h>
#include "string.h"
#include "chassis.h"
#include "detect_task.h"
#include "voltage_task.h"
#include "report.h"
#include "gimbal.h"
#include "pid_regulator.h"
#include "chassis_ctrl.h"
#include "referee_st.h"
#include "remote_control.h"
#include "bsp_usart.h"
#include "st_version.h"
#include "send_float.h"
void usb_printf(const char *fmt,...);

static uint8_t usb_buf[256];
static const char status[2][7] = {"OK", "ERROR!"};
const error_t *error_list_usb_local;

extern PID_Regulator_t PITCH_SPD_VISION_PID,YAW_SPD_VISION_PID;
extern PID_Regulator_t PITCH_POS_VISION_PID,YAW_POS_VISION_PID;

extern Game_info game_info;	
extern RC_ctrl_t rc_ctrl;
extern PID_Regulator_t CHASSIS_SPD_PID[4];
extern PID_Regulator_t SHOOT_SPD_PID[2];

void usb_task(void const * argument)
{
    MX_USB_DEVICE_Init();
    error_list_usb_local = get_error_list_point();


    while(1)
    {
        osDelay(100);
		/*
        usb_printf(
"******************************\r\n\
voltage percentage:%d%% \r\n\
DBUS:%s\r\n\
chassis motor1:%s\r\n\
chassis motor2:%s\r\n\
chassis motor3:%s\r\n\
chassis motor4:%s\r\n\
yaw motor:%s\r\n\
pitch motor:%s\r\n\
trigger motor:%s\r\n\
gyro sensor:%s\r\n\
accel sensor:%s\r\n\
mag sensor:%s\r\n\
referee usart:%s\r\n\
******************************\r\n",
            get_battery_percentage(), 
            status[error_list_usb_local[DBUS_TOE].error_exist],
            status[error_list_usb_local[CHASSIS_MOTOR1_TOE].error_exist],
            status[error_list_usb_local[CHASSIS_MOTOR2_TOE].error_exist],
            status[error_list_usb_local[CHASSIS_MOTOR3_TOE].error_exist],
            status[error_list_usb_local[CHASSIS_MOTOR4_TOE].error_exist],
            status[error_list_usb_local[YAW_GIMBAL_MOTOR_TOE].error_exist],
            status[error_list_usb_local[PITCH_GIMBAL_MOTOR_TOE].error_exist],
            status[error_list_usb_local[TRIGGER_MOTOR_TOE].error_exist],
            status[error_list_usb_local[BOARD_GYRO_TOE].error_exist],
            status[error_list_usb_local[BOARD_ACCEL_TOE].error_exist],
            status[error_list_usb_local[BOARD_MAG_TOE].error_exist],
            status[error_list_usb_local[REFEREE_TOE].error_exist]);			fdb反馈  ref期望
			*/
			//usb_report(4,YAW_POS_VISION_PID.ref ,YAW_POS_VISION_PID.fdb,YAW_SPD_VISION_PID.ref,YAW_SPD_VISION_PID.fdb);
				//usb_report(4,YAW_POS_PID.fdb,YAW_POS_PID.ref,PITCH_POS_PID.fdb,PITCH_POS_PID.ref);
		//	usb_report(4,CHASSIS_SPD_PID[0].ref ,CHASSIS_SPD_PID[0].fdb,CHASSIS_SPD_PID[1].ref,CHASSIS_SPD_PID[1].fdb);
//			usb_report(4,SHOOT_SPD_PID[0].ref ,SHOOT_SPD_PID[0].fdb,CHASSIS_SPD_PID[1].ref,CHASSIS_SPD_PID[1].fdb);
//			send_txt(YAW_POS_VISION_PID.fdb,YAW_POS_VISION_PID.ref,0,0);
//			SendCurve(YAW_POS_VISION_PID.fdb,YAW_POS_VISION_PID.ref,0,0);
//		usb_printf("%d\r\n",rc_ctrl.key.v);

    }

}

void usb_printf(const char *fmt,...)
{
    static va_list ap;
    uint16_t len = 0;

    va_start(ap, fmt);

    len = vsprintf((char *)usb_buf, fmt, ap);

    va_end(ap);


    CDC_Transmit_FS(usb_buf, len);
}
