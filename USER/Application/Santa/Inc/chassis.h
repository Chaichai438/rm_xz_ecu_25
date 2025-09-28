#ifndef  __CHASSIS_H
#define  __CHASSIS_H

#define CHASSIS_VX_MAX 9.0f
#define CHASSIS_A 0.3f
#define CHASSIS_B 0.3f 
#define CHASSIS_VYAW_MAX 1.5f
#define CHASSIS_FOLLOW_ANGLE_ALLOWANCE	50		//µ×ÅÌ¸úËæ½Ç¶ÈÓàÁ¿
#define CHASSIS_OPEN_FLAG 1
void Chassis_Ctrl(void);
void chassis_3508_spd(void);
void chassis_init(void);
float Chassis_Follow( void);
void Chassis_Power_Limit(void);
void Chassis_Ctrl_k(void);
#endif

