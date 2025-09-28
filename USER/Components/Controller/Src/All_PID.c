#include "All_PID.h"
#include "pid_regulator.h"
/*******************PIDTYPE_CREATE***********************/

pid_type_def CHASSIS_WHELL_PID_STRUCT[4]={0};//轮组PID

pid_type_def CHASSIS_FOLLOW_STRUCT={0};//底盘跟随PID

pid_type_def GIMBAL_FRIC_STRUCT[2]={0};//摩擦轮PID

pid_type_def GIMBAL_FEED_PID_POS_STRUCT={0};//拨弹PID
pid_type_def GIMBAL_FEED_PID_SPD_STRUCT={0};

pid_type_def GIMBAL_PITCH_POS_STRUCT={0};
pid_type_def GIMBAL_PITCH_SPD_STRUCT={0};//pitchPID

pid_type_def GIMBAL_YAW_PID_POS_STRUCT={0};//yawPID
pid_type_def GIMBAL_YAW_PID_SPD_STRUCT={0};

pid_type_def LowThroughFliter={0};

/*******************模糊PIDTYPE_CREATE***********************/

PID_Regulator_t GIMBAL_FRIC_STRUCT_MH[2];
PID_Regulator_t GIMBAL_FEED_PID_POS_STRUCT_MH;
PID_Regulator_t GIMBAL_FEED_PID_SPD_STRUCT_MH;
PID_Regulator_t GIMBAL_PITCH_SPD_STRUCT_MH;
PID_Regulator_t GIMBAL_YAW_PID_POS_STRUCT_MH;
PID_Regulator_t GIMBAL_YAW_PID_SPD_STRUCT_MH;

/**********************PID_PARA_CREATE*********************/

const float CHASSIS_WHELL_PID_PARA[3]={CHASSIS_WHELL_KP,CHASSIS_WHELL_KI,CHASSIS_WHELL_KD};//底盘para

const float CHASSIS_FOLLOW_PARA[3]={CHASSIS_FOLLOW_KP,CHASSIS_FOLLOW_KI,CHASSIS_FOLLOW_KD};//跟随para

const float GIMBAL_FRIC_PARA[3]={GIMBAL_FRIC_KP,GIMBAL_FRIC_KI,GIMBAL_FRIC_KD};//摩擦轮para

const float GIMBAL_FEED_PID_POS_PARA[3]={GIMBAL_FEED__POS_KP,GIMBAL_FEED__POS_KI,GIMBAL_FEED__POS_KD};//拨弹para
const float GIMBAL_FEED_PID_SPD_PARA[3]={GIMBAL_FEED__SPD_KP,GIMBAL_FEED__SPD_KI,GIMBAL_FEED__SPD_KD};

const float GIMBAL_PITCH_SPD_PARA[3]={GIMBAL_PITCH_SPD_KP,GIMBAL_PITCH_SPD_KI,GIMBAL_PITCH_SPD_KD};//pitchpara
const float GIMBAL_PITCH_POS_PARA[3]={GIMBAL_PITCH_POS_KP,GIMBAL_PITCH_POS_KI,GIMBAL_PITCH_POS_KD};

const float GIMBAL_YAW_PID_POS_PARA[3]={GIMBAL_YAW_POS_KP,GIMBAL_YAW_POS_KI,GIMBAL_YAW_POS_KD};//yawpara
const float GIMBAL_YAW_PID_SPD_PARA[3]={GIMBAL_YAW_SPD_KP,GIMBAL_YAW_SPD_KI,GIMBAL_YAW_SPD_KD};
const float LPF1_PARA[3]={LPF1_KP,LPF1_KI,LPF1_Kd};
/*********************test***********************/
float testkp1,testki1,testkd1;
float testkp2,testki2,testkd2;
/**
  * @brief     Chassis Whell PID Init
  * @param     void
  * @retval    none
  */
void Chassis_Whell_PID_Init(void)
{
    int i=0;
    for(i=0;i<4;i++)
        PID_init(&CHASSIS_WHELL_PID_STRUCT[i],\
        PID_POSITION,\
        CHASSIS_WHELL_PID_PARA,\
        CHASSIS_WHELL_MAX,\
        CHASSIS_WHELL_I_MAX);
}

/**
  * @brief     Chassis_Follow_PID_Init
  * @param     void
  * @retval    none
  */
void Chassis_Follow_PID_Init(void)
{
    PID_init(&CHASSIS_FOLLOW_STRUCT,\
        PID_POSITION,\
        CHASSIS_FOLLOW_PARA,\
        CHASSIS_FOLLOW_MAX,\
        CHASSIS_FOLLOW_I_MAX);
}

/**
  * @brief     Gimbal_Fric_PID_Init
  * @param     void
  * @retval    none
  */
void Gimbal_Fric_PID_Init(void)
{
    int i=0;
    for(i=0;i<2;i++)
    PID_init(&GIMBAL_FRIC_STRUCT[i],\
        PID_POSITION,\
        GIMBAL_FRIC_PARA,\
        GIMBAL_FRIC_MAX,\
        GIMBAL_FRIC_I_MAX);
}

/**
  * @brief     Gimbal_Feed_PID_Init
  * @param     void
  * @retval    none
  */
void Gimbal_Feed_PID_Init(void)
{

    PID_init(&GIMBAL_FEED_PID_POS_STRUCT,\
        PID_POSITION,\
        GIMBAL_FEED_PID_POS_PARA,\
        GIMBAL_FEED__POS_MAX,\
        GIMBAL_FEED__POS_I_MAX);    
    PID_init(&GIMBAL_FEED_PID_SPD_STRUCT,\
        PID_POSITION,\
        GIMBAL_FEED_PID_SPD_PARA,\
        GIMBAL_FEED__SPD_MAX,\
        GIMBAL_FEED__SPD_I_MAX);    
}

/**
  * @brief     Gimbal_Pitch_PID_Init
  * @param     void
  * @retval    none
  */
void Gimbal_Pitch_PID_Init(void)
{

    PID_init(&GIMBAL_PITCH_SPD_STRUCT,\
        PID_POSITION,\
        GIMBAL_PITCH_SPD_PARA,\
        GIMBAL_PITCH__SPD_MAX,\
        GIMBAL_PITCH__SPD_I_MAX);
	    PID_init(&GIMBAL_PITCH_POS_STRUCT,\
        PID_POSITION,\
        GIMBAL_PITCH_POS_PARA,\
        GIMBAL_PITCH__POS_MAX,\
        GIMBAL_PITCH__POS_I_MAX);
}

/**
  * @brief     Gimbal_Yaw_PID_Init
  * @param     void
  * @retval    none
  */
void Gimbal_Yaw_PID_Init(void)
{
    PID_init(&GIMBAL_YAW_PID_POS_STRUCT,\
        PID_POSITION,\
        GIMBAL_YAW_PID_POS_PARA,\
        GIMBAL_YAW_POS_MAX,\
        GIMBAL_YAW_POS_I_MAX);    
    PID_init(&GIMBAL_YAW_PID_SPD_STRUCT,\
        PID_POSITION,\
        GIMBAL_YAW_PID_SPD_PARA,\
        GIMBAL_YAW_SPD_MAX,\
        GIMBAL_YAW_SPD_I_MAX);    
}
/**
  * @brief     所有PID初始�?
  * @param     void
  * @retval    none
  */
void PID_Init(void)
{
    Chassis_Whell_PID_Init();
    Chassis_Follow_PID_Init();
    Gimbal_Fric_PID_Init();
    Gimbal_Feed_PID_Init();
    Gimbal_Pitch_PID_Init();
    Gimbal_Yaw_PID_Init();
}

/**
  * @brief     模糊Gimbal_Fric_PID_Init
  * @param     void
  * @retval    none
  */
void MH_Gimbal_Fric_PID_Init(void)
{
    uint8_t i=0;
    for(i=0;i<2;i++)
    {
      GIMBAL_FRIC_STRUCT_MH->componentKd=1;
    }
}

void LowThroughFliter_Init(void)
{
        PID_init(&LowThroughFliter,\
        PID_POSITION,\
        LPF1_PARA,\
        LPF1_MAX,\
        LPF1_I_MAX);
}