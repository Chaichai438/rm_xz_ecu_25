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
#include "INS_Task.h"
#include "Detect_Task.h"
#include "SMC_EGA.h"

Control_Info_Typedef Control_Info={
        .Gimbal = {
                .Measure.Yaw_Angle = &INS_Info.Yaw_Angle,
                .Measure.Pitch_Angle = &INS_Info.Angle[2],
                .Measure.Yaw_Gyro = &INS_Info.Yaw_Gyro,
                .Measure.Pitch_Gyro = &INS_Info.Gyro[0],
        },
};


static void Check_Damiao_Motor_Online();
static void Control_Task_Init(void);
//static void Control_Init(Control_Info_Typedef *Control_Info);
//static void Control_Measure_Update(Control_Info_Typedef *Control_Info);
static void Control_Target_Update(Control_Info_Typedef *Control_Info);
static void Control_Info_Update(Control_Info_Typedef *Control_Info);
static void Control_Mode_Update(Control_Info_Typedef *Control_Info);

LowPassFilter PitchTorque_LPF;
float angle_vel;
bool Press_L_Flag = 0;
PID_Info_TypeDef Yaw_PID[2];
PID_Info_TypeDef Pitch_PID[2];

Control_Info_Typedef Control_Info;
SMC_t YawSMC;
//                                  KP   KI   KD  Alpha Deadband  I_MAX   Output_MAX
static float Pitch_Angle_PID_Param[7] ={10.0f,0.001f,0,0,0.f,0,20};
static float Pitch_Velocity_PID_Param[7] ={5.0f,0.05f,0,0.f,0.01f,5,10};


//static float Pitch_Angle_PID_Param[6] ={80.f,0.f,0,0,0,20};
//static float Pitch_Velocity_PID_Param[6] ={-3.f,0.1f,0,0,5,10};


float Yaw_Err;
float yaw,pit,rol;
PID_Info_TypeDef Chassis_PID;
int Counter = 0;




void Control_task(void const * argument) {
    /* USER CODE BEGIN Control_Task */
    TickType_t Control_Task_SysTick = 0;
    Control_Task_Init();
//    DM_Motor_Command(&FDCAN2_TxFrame,&DM_8009_Motor[4],Motor_Enable);
    DM_Motor_Command(&FDCAN2_TxFrame,&DM_8009_Motor[4],Motor_Enable);
    /* Infinite loop */


    for (;;) {
        Control_Task_SysTick = osKernelSysTick();

        Control_Mode_Update(&Control_Info);
        Control_Target_Update(&Control_Info);
        Control_Info_Update(&Control_Info);
//        float send0 = (float)(Control_Info.SendValue[0]);Gimbal.Target.Yaw_Angle
//        USART_Vofa_Justfloat_Transmit(Control_Info.Gimbal.Target.Yaw_Angle ,*Control_Info.Gimbal.Measure.Yaw_Angle,Control_Info.SendValue[Yaw]);
        USART_Vofa_Justfloat_Transmit(Control_Info.Gimbal.Target.Pitch_Gyro,Control_Info.Gimbal.Target.Pitch_Angle ,*Control_Info.Gimbal.Measure.Pitch_Angle );
        if (remote_ctrl.rc.s[1] == 1) {
            static bool R_Flag = 0;
            if (remote_ctrl.key.set.R == 1 && R_Flag == 0) {
                Control_Info.Cover_Switch = !Control_Info.Cover_Switch;
                R_Flag = 1;
            } else if (remote_ctrl.key.set.R == 0) R_Flag = 0;
        } else if (remote_ctrl.rc.s[1] != 1) {
            if (SignBit(remote_ctrl.rc.ch[4]) == -1) {
                Control_Info.Cover_Switch = 1;
            } else if (remote_ctrl.rc.ch[4] == 0) {
                Control_Info.Cover_Switch = 0;
            }
        }
//        Check_Damiao_Motor_Online();

        DJI_6020_Motor_CAN_TxMessage(&FDCAN1_TxFrame,&Chassis_Motor[4],Control_Info.SendValue[Yaw]);
        DM_Motor_CAN_TxMessage(&FDCAN2_TxFrame,&DM_8009_Motor[4],0,0,0,0,Control_Info.Pit_SendValue);
//        DM_Motor_CAN_TxMessage(&FDCAN1_TxFrame,&DM_8009_Motor[4],0,0,0,0,Control_Info.Pit_SendValue);

        osDelayUntil(&Control_Task_SysTick,1);
    }




}

  /* USER CODE END Control_Task */

//初始化---------------------------------------------------------------------------------
  static void Control_Task_Init(void){

      PID_Init(&Pitch_PID[0],PID_POSITION,Pitch_Angle_PID_Param);
      PID_Init(&Pitch_PID[1],PID_POSITION,Pitch_Velocity_PID_Param);
//--------------------------------------------------------------------------------------------------
      SMC_Para_Init(&YawSMC,20, 88, 0, 0.05f, 25000, 0.8f, 0.45f);




  }


static void Control_Init(Control_Info_Typedef *Control_Info){

//  PID_Init(&Chassis_PID,PID_POSITION,Chassis_PID_Param);

}

//static void Control_Measure_Update(Control_Info_Typedef *Control_Info){
//
//  Control_Info->Measure.Chassis_Angle = Chassis_Motor[0].Data.Angle;
//
//}

static void Control_Target_Update(Control_Info_Typedef *Control_Info){

    if(Control_Info->Gimbal.Mode != GIMBAL_OFF){


        if(Control_Info->Gimbal.Mode == GIMBAL_IMU) {

            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
//            Control_Info->Gimbal.Target.Yaw_Angle   -= remote_ctrl.rc.ch[0]*0.0003f + remote_ctrl.mouse.x * 0.0008f;
//
            Control_Info->Gimbal.Target.Pitch_Angle += remote_ctrl.rc.ch[1]*0.00001f - remote_ctrl.mouse.y * 0.000023f;

            Control_Info->Gimbal.Target.Yaw_Angle -= remote_ctrl.rc.ch[0] * 0.0005f;


//            float pitch_changes = ((float)remote_ctrl.rc.ch[1]) * (0.01f / 660.0f);
//            Control_Info->Gimbal.Target.Pitch_Angle += pitch_changes;
            if(Control_Info->Gimbal.Target.Pitch_Angle > 0.425f) Control_Info->Gimbal.Target.Pitch_Angle = 0.425f;
            if(Control_Info->Gimbal.Target.Pitch_Angle < -0.425f) Control_Info->Gimbal.Target.Pitch_Angle = -0.425f;



//yaw限幅--------------------------------------------------
            if (Control_Info->Gimbal.Target.Yaw_Angle > 180) Control_Info->Gimbal.Target.Yaw_Angle -= 360.f;
            else if (Control_Info->Gimbal.Target.Yaw_Angle < -180) Control_Info->Gimbal.Target.Yaw_Angle += 360.f;

        }
    }
  }



static void Control_Info_Update(Control_Info_Typedef *Control_Info){

    if(Control_Info->Gimbal.Mode == GIMBAL_IMU || Control_Info->Gimbal.Mode == GIMBAL_VISION){

        Control_Info->Gimbal.Yaw_Err = Control_Info->Gimbal.Target.Yaw_Angle -  *Control_Info->Gimbal.Measure.Yaw_Angle;

        if( Control_Info->Gimbal.Yaw_Err > 180.f )         Control_Info->Gimbal.Yaw_Err -= 360.f;
        else if( Control_Info->Gimbal.Yaw_Err < -180.f)    Control_Info->Gimbal.Yaw_Err += 360.f;
//=======================================================================yaw

        YawSMC.ref = Control_Info->Gimbal.Target.Yaw_Angle;

        SMC_Tick(*Control_Info->Gimbal.Measure.Yaw_Angle,*Control_Info->Gimbal.Measure.Yaw_Gyro,&YawSMC);
        Control_Info->SendValue[Yaw] = YawSMC.u;
//----------------------------------------------------------------------------------------------------------------------------------------------------------
        Control_Info->Gimbal.Target.Pitch_Gyro = f_PID_Calculate(&Pitch_PID[0],Control_Info->Gimbal.Target.Pitch_Angle,*Control_Info->Gimbal.Measure.Pitch_Angle);
        //角速度限幅
        if(Control_Info->Gimbal.Target.Pitch_Gyro > 3.0f) Control_Info->Gimbal.Target.Pitch_Gyro = 3.0f;
        if(Control_Info->Gimbal.Target.Pitch_Gyro < -3.0f) Control_Info->Gimbal.Target.Pitch_Gyro = -3.0f;
        Control_Info->Pit_SendValue = f_PID_Calculate(&Pitch_PID[1],Control_Info->Gimbal.Target.Pitch_Gyro, *Control_Info->Gimbal.Measure.Pitch_Gyro);

        Control_Info->Gimbal.Target.Last_Yaw_Angle = Control_Info->Gimbal.Target.Yaw_Angle;



    }else{
        Control_Info->SendValue[Yaw] = 0;
        Control_Info->Pit_SendValue = 0;
    }



}


static void Control_Mode_Update(Control_Info_Typedef *Control_Info){

    if(remote_ctrl.rc.s[0] == 3  || remote_ctrl.rc.s[0] == 1) {

        if (remote_ctrl.mouse.press_r != 1) {

            Control_Info->Gimbal.Mode = GIMBAL_IMU;

        } else Control_Info->Gimbal.Mode = GIMBAL_VISION;}

        else{

            Control_Info->Gimbal.Mode = GIMBAL_OFF;

        }}

static void Check_Damiao_Motor_Online(){

    if(Damiao_Pitch_Motor.lost == 0){
        if( Damiao_Pitch_Motor.Online_cnt>0 ){
            Damiao_Pitch_Motor.Online_cnt--;
            if( Damiao_Pitch_Motor.Online_cnt<200){
                Damiao_Pitch_Motor.lost = 1;
            }
        }
    }else if(Damiao_Pitch_Motor.lost == 1){

        Damiao_Pitch_Motor.Online_cnt = 0;

    }


}

