/**
  ******************************************************************************
  * @file           : Control_Task.c
  * @brief          : Control task
  * @author         : GrassFan Wang
  * @date           : 2025/01/22
  * @version        : v1.1
  ******************************************************************************
  * @attention      : None
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "Control_Task.h"
#include "cmsis_os.h"
#include "bsp_uart.h"
#include "Remote_Control.h"
#include "PID.h"
#include "Motor.h"
#include "arm_math.h"
#include "gpio.h"

static void Control_Init(Control_Info_Typedef *Control_Info);
static void Control_Measure_Update(Control_Info_Typedef *Control_Info);
static void Control_Target_Update(Control_Info_Typedef *Control_Info);
static void Control_Info_Update(Control_Info_Typedef *Control_Info);
Control_Info_Typedef Control_Info;
//                                  K8P   KI   KD  Alpha Deadband  I_MAX   Output_MAX
static float Chassis_PID_Param[7] = {25.f,0.f,0.f,10.9f,  1.0f,      5000.f,  1200.f};

PID_Info_TypeDef Chassis_PID;
int Counter = 0;
void Control_task(void const * argument)
{
  /* USER CODE BEGIN Control_Task */
  TickType_t Control_Task_SysTick = 0;
  
	Control_Init(&Control_Info);
 /* Infinite loop */
	for(;;)
  {
      if (!remote_ctrl.rc_lost) {
          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);  // 例如点亮 LED 表示连接
      } else {
          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);    // 熄灭 LED 表示断开
      }
		Control_Task_SysTick = osKernelSysTick();

	    Control_Measure_Update(&Control_Info);
		Control_Target_Update(&Control_Info);
        Control_Info_Update(&Control_Info);
//        float send0 = (float)(Control_Info.SendValue[0]);
//        USART_Vofa_Justfloat_Transmit(Control_Info.Measure.Chassis_Angle,Control_Info.Target.Chassis_Angle,send0);
		osDelay(1);
  }
}
  /* USER CODE END Control_Task */




static void Control_Init(Control_Info_Typedef *Control_Info){

  PID_Init(&Chassis_PID,PID_POSITION,Chassis_PID_Param);

}

static void Control_Measure_Update(Control_Info_Typedef *Control_Info){

  Control_Info->Measure.Chassis_Angle = Chassis_Motor[0].Data.Angle;

}

static void Control_Target_Update(Control_Info_Typedef *Control_Info){
    Counter ++;
//  Control_Info->Target.Chassis_Angle = 100 * sin((float) Counter / 1000.0f * 3.0f);
    Control_Info->Target.Chassis_Angle = 100;
//    Control_Info->Target.Chassis_Velocity = -100;


}

static void Control_Info_Update(Control_Info_Typedef *Control_Info){
  
   PID_Calculate(&Chassis_PID, Control_Info->Target.Chassis_Angle, Control_Info->Measure.Chassis_Angle);

	 Control_Info->SendValue[0] = (int16_t)(Chassis_PID.Output);
	
}

static float FivePower(float NowTime,float UseTime){
     
	  float Time = (NowTime/UseTime);
	     
    return 10*powf(Time,3) - 15*powf(Time,4) + 6*powf(Time,5);


}
	