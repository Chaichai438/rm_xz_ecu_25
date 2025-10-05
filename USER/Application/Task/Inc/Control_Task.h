/**
  ******************************************************************************
  * @file           : Control_Task.c
  * @brief          : Control task
  * @author         : Yan Yuanbin
  * @date           : 2023/04/27
  * @version        : v1.0
  ******************************************************************************
  * @attention      : None
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CONTROL_TASK_H
#define CONTROL_TASK_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
#include "PID.h"

#define SignBit(x)  ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
/**
 * @brief typedef structure that contains the information of chassis control
*/


typedef enum{
    GIMBAL_OFF,
    GIMBAL_Calibration,
    GIMBAL_IMU,
    GIMBAL_VISION,
    GIMBAL_MODE_NUM,
}Gimbal_Mode_e;

typedef struct
{

    Gimbal_Mode_e Mode;
    bool Gimbal_Output;
    float Yaw_Err;
    struct{
        float Last_Pitch_Angle;
        float Last_Yaw_Angle;
        float Pitch_Angle;
        float Yaw_Angle;
        float Pitch_Gyro;
        float Yaw_Gyro;
    }Target;

    struct{
        float *Pitch_Angle;
        float *Yaw_Angle;
        float *Pitch_Gyro;
        float *Yaw_Gyro;
    }Measure;

    struct{
        float Min;
        float Max;
    }Limit_Pitch;



}Gimbal_Info_Typedef;

typedef struct
{


    uint8_t trigger_Buf;

    bool FIRE_SINGLE_ENABLE;

    float Center_Angle;
    bool Turn_Head_Flag;
    uint16_t Turn_Time;
    bool Gimbal_Turn_Begin;

    int8_t stuck_flag;

    bool Cover_Switch;
    int8_t Chassis_Week_Flag;
    int16_t SendValue[5];
    float  Pit_SendValue;
    Gimbal_Info_Typedef Gimbal;
}Control_Info_Typedef;

void Control_task(void const * argument);
extern LowPassFilter PitchTorque_LPF;
extern Control_Info_Typedef Control_Info;
#endif //CONTROL_TASK_H