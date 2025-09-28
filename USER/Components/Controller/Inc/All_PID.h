#ifndef  __ALL__PID_H
#define  __ALL__PID_H
#include "pid.h"

/**************************底盘轮子PID参数*************************/
#define CHASSIS_WHELL_KP 5.0f     		//轮子KP
#define CHASSIS_WHELL_KD 0.5f   		//轮子
#define CHASSIS_WHELL_KI 0.0000f     		//轮子

#define CHASSIS_WHELL_MAX 15000.0f
#define CHASSIS_WHELL_I_MAX 1500.0f

/**************************底盘轮子备用PID参数*************************/
//#define CHASSIS_WHELL_KP_SP 10.0f 			//如果哪个轮子响应有较大差别，单独搞个
//#define CHASSIS_WHELL_KD_SP 10.0f 			//如果哪个轮子响应有较大差别，单独搞个
//#define CHASSIS_WHELL_KI_SP 0.001f 			//如果哪个轮子响应有较大差别，单独搞个

/***********************底盘跟随PID参数*************************/
#define CHASSIS_FOLLOW_KP 0.0045f     		//底盘跟随KP
#define CHASSIS_FOLLOW_KD 0.0f     		//底盘跟随
#define CHASSIS_FOLLOW_KI 0.0f     		//底盘跟随

#define CHASSIS_FOLLOW_MAX 5000.0f
#define CHASSIS_FOLLOW_I_MAX 1500.0f

/***********************摩擦轮PID参数*************************/
#define GIMBAL_FRIC_KP 15.0f						//摩擦轮KP
#define GIMBAL_FRIC_KD 1.0f						//摩擦轮
#define GIMBAL_FRIC_KI 0.0f						//摩擦轮

#define GIMBAL_FRIC_MAX 15000.0f
#define GIMBAL_FRIC_I_MAX 1500.0f

/***********************拨弹双环PID参数*************************/
#define GIMBAL_FEED__POS_KP 0.17f      //拨弹
#define GIMBAL_FEED__POS_KD 0.0f      //拨弹
#define GIMBAL_FEED__POS_KI 0.0f    //拨弹

#define GIMBAL_FEED__POS_MAX 15000.0f
#define GIMBAL_FEED__POS_I_MAX 1500.0f

#define GIMBAL_FEED__SPD_KP 12.0f      //拨弹
#define GIMBAL_FEED__SPD_KD 13.0f      //拨弹
#define GIMBAL_FEED__SPD_KI 0.0f      //拨弹

#define GIMBAL_FEED__SPD_MAX 16500.0f
#define GIMBAL_FEED__SPD_I_MAX 1500.0f
/**************************PITCH_PID速度环参数**********************/
#define GIMBAL_PITCH_SPD_KP 12.0f //15.0f			//PITCH_KP
#define GIMBAL_PITCH_SPD_KD 1.0f			//PITCH
#define GIMBAL_PITCH_SPD_KI 1.0f			//PITCH

#define GIMBAL_PITCH__SPD_MAX 15000.0f
#define GIMBAL_PITCH__SPD_I_MAX 7500.0f
/**************************PITCH_PID参数位置环**********************/
#define GIMBAL_PITCH_POS_KP 900.0f    //1200.0f			//PITCH位置轴备用
#define GIMBAL_PITCH_POS_KD 230.0f			//PITCH位置轴备用
#define GIMBAL_PITCH_POS_KI 0.0f			//PITCH位置轴备用


#define GIMBAL_PITCH__POS_MAX 15000.0f
#define GIMBAL_PITCH__POS_I_MAX 3.0f
/***********************YAW双环PID参数*************************/
#define GIMBAL_YAW_POS_KP  1.7f//0.08       //1.7f				//YAWKP
#define GIMBAL_YAW_POS_KD  45.0f//0       //45.0f				//YAW
#define GIMBAL_YAW_POS_KI  0.f//0       //0.0f				//YAW

#define GIMBAL_YAW_POS_MAX 15000.0f
#define GIMBAL_YAW_POS_I_MAX 20.0f

#define GIMBAL_YAW_SPD_KP  100 //250  //100.0f				//YAW
#define GIMBAL_YAW_SPD_KD  10 //10     //10.0f				//YAW
#define GIMBAL_YAW_SPD_KI  0	// 0.7     //6.0f			//YAW

#define GIMBAL_YAW_SPD_MAX 15000.0f
#define GIMBAL_YAW_SPD_I_MAX 13500.0f


/*************************模糊PID参数*****************************/

/********************摩擦轮PID参数***********************/
#define MH_FRIC_KP 10.0f      
#define MH_FRIC_KI 0.001f      
#define MH_FRIC_KD 10.0f      
#define MH_FRIC_OUT_MAX 15000.0f
#define MH_FRIC_COMPONENT_KP_MAX 10.0f      
#define MH_FRIC_COMPONENT_KI_MAX 1500.0f
#define MH_FRIC_COMPONENT_KD_MAX 10.0f 
/************************拨弹PID参数*********************/
#define MH_FEED_POS_KP 10.0f      
#define MH_FEED_POS_KI 0.001f      
#define MH_FEED_POS_KD 10.0f      
#define MH_FEED_POS_OUT_MAX 15000.0f
#define MH_FEED_POS_COMPONENT_KP_MAX 10.0f      
#define MH_FEED_POS_COMPONENT_KI_MAX 1500.0f
#define MH_FEED_POS_COMPONENT_KD_MAX 10.0f 

#define MH_FEED_SPD_KP 10.0f      
#define MH_FEED_SPD_KI 0.001f      
#define MH_FEED_SPD_KD 10.0f      
#define MH_FEED_SPD_OUT_MAX 15000.0f
#define MH_FEED_SPD_COMPONENT_KP_MAX 10.0f      
#define MH_FEED_SPD_COMPONENT_KI_MAX 1500.0f
#define MH_FEED_SPD_COMPONENT_KD_MAX 10.0f 

//PITCH_PID参数
#define MH_PITCH_SPD_KP 10.0f      
#define MH_PITCH_SPD_KI 0.001f      
#define MH_PITCH_SPD_KD 10.0f      
#define MH_PITCH_SPD_OUT_MAX 15000.0f
#define MH_PITCH_SPD_COMPONENT_KP_MAX 10.0f      
#define MH_PITCH_SPD_COMPONENT_KI_MAX 1500.0f
#define MH_PITCH_SPD_COMPONENT_KD_MAX 10.0f 
//YAW_PID参数
#define MH_YAW_POS_KP 10.0f      
#define MH_YAW_POS_KI 0.001f      
#define MH_YAW_POS_KD 10.0f      


/******************/
#define LPF1_KP 0
#define LPF1_KI 0
#define LPF1_Kd 0

#define LPF1_MAX 15000.0f
#define LPF1_I_MAX 0
/*************************FUNC_DECLAR*****************************/

/**
  * @brief     Chassis Whell PID Init
  * @param     void
  * @retval    none
  */
void Chassis_Whell_PID_Init(void);

/**
  * @brief     Chassis_Follow_PID_Init
  * @param     void
  * @retval    none
  */
void Chassis_Follow_PID_Init(void);

/**
  * @brief     Gimbal_Fric_PID_Init
  * @param     void
  * @retval    none
  */
void Gimbal_Fric_PID_Init(void);

/**
  * @brief     Gimbal_Feed_PID_Init
  * @param     void
  * @retval    none
  */
void Gimbal_Feed_PID_Init(void);

/**
  * @brief     Gimbal_Pitch_PID_Init
  * @param     void
  * @retval    none
  */
void Gimbal_Pitch_PID_Init(void);

/**
  * @brief     Gimbal_Yaw_PID_Init
  * @param     void
  * @retval    none
  */
void Gimbal_Yaw_PID_Init(void);

/**
  * @brief     所有PID初始化
  * @param     void
  * @retval    none
  */
void PID_Init(void);


void LowThroughFliter_Init(void);

#endif
