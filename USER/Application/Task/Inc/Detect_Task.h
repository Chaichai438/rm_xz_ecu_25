/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Detect_Task.h
  * @brief          : Detect task
  * @author         : GrassFan Wang
  * @date           : 2025/01/22
  * @version        : v1.0
  ******************************************************************************
  * @attention      : None
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DETECT_TASK_H
#define DETECT_TASK_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "config.h"

typedef struct
{
    bool IF_Aiming_Enable;  /*!< vision aiming enable flag */

    bool IF_Fire_Accept;    /*!< fire acception */

    float target_Pitch;   /*!< target pitch angle in radians */
    float target_Yaw; 	/*!< target yaw angle in radians */

}Vision_Info_Typedef;


extern Vision_Info_Typedef Vision_Info;
extern void Detect_task(void const * argument);

#endif //DETECT_TASK_H

