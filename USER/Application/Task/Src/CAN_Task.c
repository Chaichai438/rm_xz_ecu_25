/**
  ******************************************************************************
  * @file           : CAN_Task.c
  * @brief          : CAN task
  * @author         : GrassFam Wang
  * @date           : 2025/1/22
  * @version        : v1.1
  ******************************************************************************
  * @attention      : None
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "CAN_Task.h"
#include "Control_Task.h"
#include "INS_Task.h"
#include "Motor.h"
#include "bsp_can.h"
#include "Remote_Control.h"
#include "Control_Task.h"
#include "usart.h"


/* USER CODE BEGIN Header_CAN_Task */
/**
* @brief Function implementing the StartCANTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CAN_Task */
uint8_t rxbuf[8];
 void CAN_task(void const * argument)
{


  TickType_t CAN_Task_SysTick = 0;
//    DJI_M3508_Send_Single_Current(&FDCAN1_TxFrame,&Chassis_Motor[0], -200);
//    osDelay(30);

//	DM_Motor_Command(&FDCAN1_TxFrame,&DM_8009_Motor[4],Motor_Enable);
//  osDelay(20);
//	DM_Motor_Command(&FDCAN2_TxFrame,&DM_8009_Motor[1],Motor_Enable);
//  osDelay(30);
//  DM_Motor_Command(&FDCAN2_TxFrame,&DM_8009_Motor[2],Motor_Enable);
//  osDelay(30);
//	DM_Motor_Command(&FDCAN2_TxFrame,&DM_8009_Motor[4],Motor_Enable);
//  osDelay(30);
	for(;;)
  {
		CAN_Task_SysTick = osKernelSysTick();
//        FDCAN2_RxFifo1RxHandler_1(&DJI_Yaw_Motor.FDCANFrame.RxIdentifier,rxbuf);
//	 // CAN-FD
//		DM_Motor_CAN_TxMessage(&DJI_Yaw_Motor->F,&DM_8009_Motor[0],0,0,0,0,10);
//        DJI_6020_Motor_CAN_TxMessage(&FDCAN2_TxFrame,&Chassis_Motor[4],Control_Info.SendValue[Yaw]);
//        DM_Motor_CAN_TxMessage(&FDCAN1_TxFrame,&DM_8009_Motor[4],0,0,0,0,0);
//        DJI_6020_Motor_CAN_TxMessage(&FDCAN2_TxFrame,&DJI_Yaw_Motor,200);
//        DJI_M3508_Send_Single_Current(&FDCAN1_TxFrame,&Chassis_Motor[0],800);
//		DM_Motor_CAN_TxMessage(&FDCAN2_TxFrame,&DM_8009_Motor[1],0,0,0,0,0);
//	    DM_Motor_CAN_TxMessage(&FDCAN1_TxFrame,&DM_8009_Motor[1],0,0,0,0,2);
//	    DM_Motor_CAN_TxMessage(&FDCAN2_TxFrame,&DM_8009_Motor[2],0,0,0,0,0);

//
	 if(CAN_Task_SysTick % 2 == 0){


	 }
		osDelay(1);
  }

}


